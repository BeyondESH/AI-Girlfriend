#include "audiomgr.h"
#include <QMediaDevices>
#include <QAudioDevice>
#include <QDebug>
#include <QBuffer>
#include <QAudioOutput>
#include <QFile>

AudioMgr::AudioMgr(QObject *parent)
    : QObject{parent},_audioSource(nullptr)
    ,_ioDevice(nullptr)
    ,_state(QAudio::StoppedState)
    ,_audioSink(nullptr)
{
    _audioDevice=QMediaDevices::defaultAudioInput();
    if (_audioDevice.isNull()) {
        qDebug() << "无默认音频设备";
    }
    // qDebug()<<audioDevice.description();

    setupPresets();
}

AudioMgr::~AudioMgr()
{
    asrStop();
    if(_audioSource!=nullptr){
        this->stop();
        _audioSource->deleteLater();
        _audioSource=nullptr;
    }
    if(_audioSink!=nullptr){
        _audioSink->stop();
        _audioSink->deleteLater();
        _audioSink=nullptr;
    }

}

void AudioMgr::recordAsr()
{
    startWithConfig(_configs["asr"]);
}

void AudioMgr::recordClone()
{
    startWithConfig(_configs["clone"]);
}

void AudioMgr::stop()
{
    if(_audioSource!=nullptr) {
        _audioSource->stop();
        _ioDevice = nullptr; // 由 QAudioSource 管理,不需要删除
    }
}

void AudioMgr::asrStop()
{
    if(_audioSource!=nullptr) {
        _audioSource->stop();
        _ioDevice = nullptr; // 由 QAudioSource 管理,不需要删除
    }
    emit signal_endAsrRecord();
}

void AudioMgr::suspend()
{
    _audioSource->suspend();
}

void AudioMgr::resume()
{
    _audioSource->resume();
}

int AudioMgr::sampleRate() const
{
    return _sampleRate;
}

void AudioMgr::setupPresets()
{
    QAudioFormat audioFormat;
    audioFormat.setSampleRate(16000);
    audioFormat.setChannelCount(1);
    audioFormat.setSampleFormat(QAudioFormat::Int16);
    _configs["asr"]=audioFormat;

    audioFormat.setSampleRate(192000);
    audioFormat.setChannelCount(2);
    _configs["clone"]=audioFormat;
}

void AudioMgr::stopAudioOutput()
{
    if(_audioSink!=nullptr){
        _audioSink->stop();
        _audioSink->deleteLater();
        _audioSink=nullptr;
    }
}

void AudioMgr::startWithConfig(const QAudioFormat &audioFormat)
{
    if(_audioSource!=nullptr){
        qDebug()<<"停止之前的录制，开启当前录制";
        stop();
        _audioSource->deleteLater();
        _audioSource=nullptr;
    }

    if(!_audioDevice.isFormatSupported(audioFormat)) {
        qDebug() << "音频格式不支持,使用设备推荐格式";
        QAudioFormat newFormat = _audioDevice.preferredFormat();
    }

     _audioSource = new QAudioSource(_audioDevice, audioFormat);

    if(_audioSource == nullptr) {
        qDebug() << "音频源未初始化";
        return;
    }

    connect(this->_audioSource,&QAudioSource::stateChanged,[this](auto nowState){
        _state=nowState;
    });

    _ioDevice = _audioSource->start();
    if(_ioDevice == nullptr) {
        qDebug() << "启动录制失败";
        return;
    }

    connect(_ioDevice, &QIODevice::readyRead, this, [this](){
        qint64 bytesReady = _ioDevice->bytesAvailable();
        QByteArray pcmData = _ioDevice->read(bytesReady);
        emit signal_handlePcmData(pcmData);
    });
}

void AudioMgr::test()
{
    QFile *sourceFile = new QFile("C:/Users/19896/Downloads/response3.raw", this);
    sourceFile->open(QIODevice::ReadOnly);
    QAudioFormat format;
    format.setSampleRate(24000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice info(QMediaDevices::defaultAudioOutput());
    if (!info.isFormatSupported(format)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }

    _audioSink=new QAudioSink(format, this);
    connect(_audioSink, &QAudioSink::stateChanged,[this](QtAudio::State newState){
        switch (newState) {
        case QtAudio::IdleState:
            // Finished playing (no more data)
            stopAudioOutput();
            break;

        case QtAudio::StoppedState:
            // Stopped for other reasons
            if (_audioSink->error() != QtAudio::NoError) {
                // Error handling
            }
            break;

        default:
            // ... other cases as appropriate
            break;
        }
    });
    _audioSink->start(sourceFile);
}

QByteArray AudioMgr::createWavHeader(qint64 pcmDataSize)
{
    QByteArray header;
    qint32 sampleRate = 24000;
    qint16 channels = 1;
    qint16 bitsPerSample = 16;
    qint32 byteRate = sampleRate * channels * bitsPerSample / 8;
    qint16 blockAlign = channels * bitsPerSample / 8;
    qint32 dataSize = pcmDataSize;
    qint32 fileSize = 36 + dataSize;

    header.append("RIFF");
    header.append(QByteArray::fromRawData(reinterpret_cast<const char*>(&fileSize), 4));
    header.append("WAVEfmt ");
    qint32 fmtChunkSize = 16;
    header.append(QByteArray::fromRawData(reinterpret_cast<const char*>(&fmtChunkSize), 4));
    qint16 audioFormat = 1; // PCM
    header.append(QByteArray::fromRawData(reinterpret_cast<const char*>(&audioFormat), 2));
    header.append(QByteArray::fromRawData(reinterpret_cast<const char*>(&channels), 2));
    header.append(QByteArray::fromRawData(reinterpret_cast<const char*>(&sampleRate), 4));
    header.append(QByteArray::fromRawData(reinterpret_cast<const char*>(&byteRate), 4));
    header.append(QByteArray::fromRawData(reinterpret_cast<const char*>(&blockAlign), 2));
    header.append(QByteArray::fromRawData(reinterpret_cast<const char*>(&bitsPerSample), 2));
    header.append("data");
    header.append(QByteArray::fromRawData(reinterpret_cast<const char*>(&dataSize), 4));
    return header;
}

QAudio::State AudioMgr::state() const
{
    return _state;
}

void AudioMgr::slot_tts_finished(const QByteArray &data)
{
    if (data.isEmpty()) {
        qDebug() << "TTS 数据为空";
        return;
    }
    
    qDebug() << "TTS 数据大小:" << data.size();
    
    QBuffer *buffer = new QBuffer;
    buffer->setData(data);
    buffer->open(QIODevice::ReadOnly);

    QAudioFormat format;
    format.setSampleRate(24000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice info(QMediaDevices::defaultAudioOutput());
    if (!info.isFormatSupported(format)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        delete buffer;
        return;
    }

    if (_audioSink) {
        _audioSink->stop();
        _audioSink->deleteLater();
    }
    
    _audioSink = new QAudioSink(format, this);
    connect(_audioSink, &QAudioSink::stateChanged,[this, buffer](QtAudio::State newState){
        switch (newState) {
        case QtAudio::IdleState:
            // Finished playing (no more data)
            stopAudioOutput();
            delete buffer;
            emit signal_tts_playback_finished();  // 通知播放完成
            break;

        case QtAudio::StoppedState:
            // Stopped for other reasons
            if (_audioSink->error() != QtAudio::NoError) {
                qWarning() << "Audio playback error";
            }
            break;

        default:
            break;
        }
    });
    _audioSink->start(buffer);
}
