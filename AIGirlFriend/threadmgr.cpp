#include "threadmgr.h"

ThreadMgr::~ThreadMgr()
{
    stop();
}

void ThreadMgr::setMaxThreadCount(int n)
{
    _pool.setMaxThreadCount(n);
}

int ThreadMgr::MaxThreadCount() const
{
    return _pool.maxThreadCount();
}

void ThreadMgr::stop()
{
    if(_isStop==false){
        _isStop=true;
        _pool.clear();
    }
     _pool.waitForDone();
}

void ThreadMgr::submitVoid(std::function<void ()> func, int priority)
{
    using RunnableType=RunnableVoid<void>;

}

ThreadMgr::ThreadMgr(QObject *parent)
    : QObject{parent},_isStop(false)
{
    _pool.setMaxThreadCount(QThread::idealThreadCount());
}
