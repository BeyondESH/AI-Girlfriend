#ifndef JSONMGR_H
#define JSONMGR_H

#include <QJsonObject>

class JsonMgr
{
public:
    JsonMgr()=delete;
    static void printJsonObject(const QJsonObject &jsonObj,int level=0);
private:
    static void printJsonValue(const QJsonValue &jsonValue,int level);
    static QString printIndent(int level);
};

#endif // JSONMGR_H
