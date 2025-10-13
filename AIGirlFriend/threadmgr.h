#ifndef THREADMGR_H
#define THREADMGR_H

#include <QObject>
#include <QThreadPool>
#include <QFuture>
#include <QRunnable>
#include <QPromise>

class ThreadMgr : public QObject
{
    Q_OBJECT
public:
    explicit ThreadMgr(QObject *parent = nullptr);
    ~ThreadMgr() override;
    void setMaxThreadCount(int n);
    int MaxThreadCount() const;
    void stop();

    // 提交有返回值的异步任务
    template<class Func>
    auto submit(Func && func)
        ->QFuture<std::invoke_result_t<Func>>{
        using Ret = std::invoke_result_t<Func>;// 推导任务返回类型
        using RunnableType= RunnableWithPromise<Ret>;// 选择对应的任务包装类
        auto *runnable = new RunnableType(func);
        _pool.start(runnable);
        return runnable->future();
    }

    // 提交无返回值任务
    void submitVoid(std::function<void()>func,int priority=0);
private:
    QThreadPool _pool;
    std::atomic_bool _isStop;

    template<class T>
    class RunnableWithPromise : public QRunnable {
    public:
        explicit RunnableWithPromise(std::function<T()>func):_func(std::move(func))
        {
            setAutoDelete(true);
        }
        void run() override
        {
            if constexpr (std::is_void_v<T>){
                _func();
                _promise.finish();
            }else{
                T res =_func();
                _promise.addResult(std::move(res));
                _promise.finish();
            }
        }
        QFuture<T> future() const
        {
            return _promise.future();
        }
    private:
        std::function<T()> _func;
        mutable QPromise<T> _promise;
    };

    template<class T>
    class RunnableVoid:public QRunnable{
    public:
        explicit RunnableVoid(std::function<void()>func):_func(std::move(func))
        {
            setAutoDelete(true);
        }
        void run() override
        {
            _func();
        }
    private:
        std::function<void()> _func;
    };
signals:

};

#endif // THREADMGR_H
