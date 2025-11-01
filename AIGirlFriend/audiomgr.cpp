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
    _sampleRate=audioFormat.sampleRate();
    // qDebug()<<sampleRate;
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
    _ioDevice=_audioSource->start();
    connect(_ioDevice,&QIODevice::readyRead,[this](){
        qint64 bytesReady=_audioSource->bytesAvailable();
        QByteArray pcmData=_ioDevice->read(bytesReady);
        emit signal_handlePcmData(pcmData);
    });
}

void AudioMgr::stop()
{
    _audioSource->stop();
}

int AudioMgr::sampleRate() const
{
    return _sampleRate;
}
