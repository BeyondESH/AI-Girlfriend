#include "application.h"

Application::Application(QObject *parent)
    : QObject{parent}
    ,_audioMgr(new AudioMgr(this))
    ,_gateWay(new GateWay(this))
{
    connect(_audioMgr,&AudioMgr::signal_handlePcmData,_gateWay,&GateWay::slot_handlePcmData);
    connect(_audioMgr,&AudioMgr::signal_endAsrRecord,_gateWay,&GateWay::slot_endAsrRecord);
    connect(_gateWay,&GateWay::signal_tts_finished,_audioMgr,&AudioMgr::slot_tts_finished);
    // _audioMgr->test();
}

Application::~Application()
{

}

void Application::sendAsrConfig()
{
    _gateWay->wsSendAsrConfig();
}

void Application::on_pushButtonRecord_clicked()
{
    auto state = _audioMgr->state();
    qDebug() << "当前音频状态:" << state;
    switch (state) {
    case QtAudio::StoppedState:
        sendAsrConfig();
        _audioMgr->recordAsr();
        break;
    case QtAudio::ActiveState:
        _audioMgr->suspend();
        break;
    case QtAudio::SuspendedState:
        _audioMgr->resume();
        break;
    default:
        _audioMgr->asrStop();
        break;
    }
}
