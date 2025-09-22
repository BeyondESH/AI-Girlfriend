#include "test.h"

#include "threadmgr.h"
#include <QDebug>

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
