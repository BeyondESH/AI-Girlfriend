#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "audiomgr.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,_audioMgr(new AudioMgr(this))
    ,_gateWay(new GateWay(this))
{
    ui->setupUi(this);
    connect(_audioMgr,&AudioMgr::signal_handlePcmData,_gateWay,&GateWay::slot_handlePcmData);
    connect(_audioMgr,&AudioMgr::signal_endAsrRecord,_gateWay,&GateWay::slot_endAsrRecord);
    connect(_gateWay,&GateWay::signal_tts_finished,_audioMgr,&AudioMgr::slot_tts_finished);
    // _audioMgr->test();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendAsrConfig()
{
    _gateWay->wsSendAsrConfig();
}

void MainWindow::on_pushButtonRecord_clicked()
{
    auto state = _audioMgr->state();
    qDebug() << "当前音频状态:" << state;
    switch (state) {
    case QtAudio::StoppedState:
        sendAsrConfig();
        _audioMgr->recordAsr();
        ui->pushButtonRecord->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause));
        break;
    case QtAudio::ActiveState:
        _audioMgr->suspend();
        ui->pushButtonRecord->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
        break;
    case QtAudio::SuspendedState:
        _audioMgr->resume();
        ui->pushButtonRecord->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause));
        break;
    default:
        _audioMgr->asrStop();
        ui->pushButtonRecord->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
        break;
    }
}
