#ifndef AUDIOMGR_H
#define AUDIOMGR_H

#include <QObject>
#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioSource>
#include <QMap>
#include <QAudioSink>

class AudioMgr : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int sampleRate READ sampleRate)
public:
    explicit AudioMgr(QObject *parent = nullptr);
    ~AudioMgr();
    void recordAsr();
    void recordClone();
    void stop();
    void asrStop();
    void suspend();
    void resume();
    int sampleRate() const;
    void setupPresets();
    void stopAudioOutput();
    void startWithConfig(const QAudioFormat &audioFormat);
    void test();
    QByteArray createWavHeader(qint64 pcmDataSize);
    QAudio::State state() const;
public slots:
    void slot_tts_finished(const QByteArray &data);
signals:
    void signal_handlePcmData(const QByteArray &pcmData);
    void signal_endAsrRecord();
    void signal_tts_playback_finished();  // TTS播放完成信号
private:
    QAudioSource *_audioSource;
    QIODevice * _ioDevice;
    QAudioSink* _audioSink;
    int _sampleRate;
    QAudioDevice _audioDevice;
    QMap<QString,QAudioFormat> _configs;
    QAudio::State _state;
};

#endif // AUDIOMGR_H
