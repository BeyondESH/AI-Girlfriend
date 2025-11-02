#include "test.h"

#include "threadmgr.h"
#include <QDebug>
#include <httpmgr.h>
#include "global.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "audiomgr.h"
#include "jsonmgr.h"


void Test::test_threadMgr()
{
    // 获取 ThreadMgr
    ThreadMgr threadMgr;

    // 提交一个计算任务
    auto futureInt = threadMgr.submit([]() -> int {
        qDebug() << "Running task in thread:" << QThread::currentThread();
        return 42;
    });

    // 提交一个 void 任务
    threadMgr.submitVoid([]() {
        qDebug() << "Void task running in thread:" << QThread::currentThread();
    });

    // 等待结果
    futureInt.waitForFinished();
    qDebug() << "Future result =" << futureInt.result();
}

void Test::test_http_get()
{
    HttpMgr *httpMgr=new HttpMgr();
    httpMgr->get(QUrl("https://www.baidu.com"),ReqId::ID_TEST);
}

void Test::test_post_chat()
{
    QJsonObject rootObj,messageObj;
    rootObj["model"]="qwen3:8b";
    rootObj["stream"]=false;
    messageObj["role"]="user";
    messageObj["content"]="请以用户女友的身份回答：你吃饭了吗";
    QJsonArray messageArray;
    messageArray.append(messageObj);
    rootObj["messages"]=messageArray;
    QJsonDocument jsonDoc(rootObj);
    QByteArray data=jsonDoc.toJson(QJsonDocument::Indented);
    HttpMgr *httpMgr=new HttpMgr();
    httpMgr->post(QUrl("http://localhost:11434/api/chat"),ReqId::ID_SEND_CHAT,data);
}

void Test::test_audioMgr()
{
    AudioMgr *audioMgr=new AudioMgr;
    audioMgr->start();
}

void Test::test_jsonMgr()
{
    QJsonObject jsonObj;
    QJsonObject jsonObj2;
    jsonObj["role"]="user";
    jsonObj["num"]=100;
    jsonObj["null"];
    jsonObj2["isSucess"]=true;
    jsonObj["jsonObj2"]=jsonObj2;
    JsonMgr::printJsonObject(jsonObj);
}
