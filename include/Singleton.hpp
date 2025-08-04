//
// Created by Beyond on 25-8-4.
//

#ifndef SINGLETON_H
#define SINGLETON_H


template<typename T>
class Singleton {
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T& getInstance() {
        static T instance;
        return instance;
    }

protected:
    Singleton()=default;
    virtual ~Singleton() = default;
};



#endif //SINGLETON_H
