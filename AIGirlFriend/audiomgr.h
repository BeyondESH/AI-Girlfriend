#ifndef AUDIOMGR_H
#define AUDIOMGR_H

#include <QObject>
#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioSource>
class AudioMgr : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int sampleRate READ sampleRate)
public:
    explicit AudioMgr(QObject *parent = nullptr);
    ~AudioMgr();
    Q_INVOKABLE  void start();
    Q_INVOKABLE  void stop();
    int sampleRate() const;

signals:
    void signal_handlePcmData(const QByteArray &pcmData);
private:
    QAudioSource *_audioSource;
    QIODevice * _ioDevice;
    int _sampleRate;
};

#endif // AUDIOMGR_H
