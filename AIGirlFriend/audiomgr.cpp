#include "audiomgr.h"
#include <QMediaDevices>
#include <QAudioDevice>
#include <QDebug>

AudioMgr::AudioMgr(QObject *parent)
    : QObject{parent},_audioSource(nullptr),_ioDevice(nullptr)
{
    QAudioDevice audioDevice=QMediaDevices::defaultAudioInput();
    if (audioDevice.isNull()) {
        qDebug() << "无默认音频设备";
        return;
    }

    QAudioFormat audioFormat=audioDevice.preferredFormat();
    // qDebug()<<audioDevice.description();
    //设置采样率
    audioFormat.setSampleRate(16000);
    _sampleRate=audioFormat.sampleRate();
    qDebug()<<_sampleRate;
    _audioSource=new QAudioSource(audioDevice,audioFormat);
}

AudioMgr::~AudioMgr()
{
    if(_audioSource!=nullptr){
        this->stop();
        _audioSource->deleteLater();
        _audioSource=nullptr;
    }
}

void AudioMgr::start()
{
    if(_audioSource == nullptr) {
        qDebug() << "音频源未初始化";
        return;
    }

    if(_ioDevice != nullptr) {
        qDebug() << "已经在录制中";
        return;
    }

    _ioDevice=_audioSource->start();
    if(_ioDevice == nullptr) {
        qDebug() << "启动录制失败";
        return;
    }

    connect(_ioDevice,&QIODevice::readyRead,[this](){
        qint64 bytesReady=_ioDevice->bytesAvailable();
        QByteArray pcmData=_ioDevice->read(bytesReady);
        emit signal_handlePcmData(pcmData);
    });
}

void AudioMgr::stop()
{
    _audioSource->stop();
    emit signal_endRecord();
}

int AudioMgr::sampleRate() const
{
    return _sampleRate;
}
