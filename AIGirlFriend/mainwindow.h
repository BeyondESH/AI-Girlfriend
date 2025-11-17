#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "audiomgr.h"
#include "GateWay.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void sendAsrConfig();
private slots:

    void on_pushButtonRecord_clicked();

private:
    Ui::MainWindow *ui;
    AudioMgr* _audioMgr;
    GateWay* _gateWay;
};

#endif // MAINWINDOW_H
