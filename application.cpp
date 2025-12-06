#include "application.h"

Application::Application(QObject *parent)
    : QObject{parent}
    ,_audioMgr(new AudioMgr(this))
    ,_gateWay(new GateWay(this))
{
    connect(_audioMgr,&AudioMgr::signal_handlePcmData,_gateWay,&GateWay::slot_handlePcmData);
    connect(_audioMgr,&AudioMgr::signal_endAsrRecord,_gateWay,&GateWay::slot_endAsrRecord);
    connect(_gateWay,&GateWay::signal_tts_finished,_audioMgr,&AudioMgr::slot_tts_finished);
    connect(_gateWay,&GateWay::signal_receive_llm,this,&Application::signal_receive_llm);
    connect(_gateWay,&GateWay::signal_asr_text,this,&Application::signal_asr_text);
    // _audioMgr->test();
}

Application::~Application()
{

}

void Application::sendAsrConfig()
{
    _gateWay->wsSendAsrConfig();
}

void Application::sendChatMessage(QString text)
{
    _gateWay->sendllmMessage(text,ReqId::CHAT_LLM);
}

void Application::startVoiceRecord()
{
    auto state = _audioMgr->state();
    if (state == QtAudio::StoppedState) {
        sendAsrConfig();
        _audioMgr->recordAsr();
    }
}

void Application::stopVoiceRecord()
{
    auto state = _audioMgr->state();
    if (state == QtAudio::ActiveState || state == QtAudio::SuspendedState) {
        _audioMgr->asrStop();
    }
}

void Application::toggleVoiceRecord()
{
    on_pushButtonRecord_clicked();
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
