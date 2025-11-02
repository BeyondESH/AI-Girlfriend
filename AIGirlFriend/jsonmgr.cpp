#include "jsonmgr.h"
#include <iostream>
#include <QJsonArray>

void JsonMgr::printJsonObject(const QJsonObject &jsonObj,int level)
{
    std::cout<<printIndent(level).toStdString()<<"{"<<std::endl;
    auto keys=jsonObj.keys();
    for (auto key:keys) {
        std::cout<<"    ";
        std::cout<<printIndent(level).toStdString()<<"\""<<key.toStdString()<<"\" : ";
        printJsonValue(jsonObj[key],level);
    }
    std::cout<<printIndent(level).toStdString()<<"}"<<std::endl;
}

void JsonMgr::printJsonValue(const QJsonValue &jsonValue,int level)
{
    switch (jsonValue.type()) {
    case QJsonValue::Type::String:
        std::cout<<"\""<<jsonValue.toString().toStdString()<<"\""<<std::endl;
        break;
    case QJsonValue::Type::Double:
        std::cout<<jsonValue.toDouble()<<std::endl;
        break;
    case QJsonValue::Type::Null:
        std::cout<<"\"\""<<std::endl;
        break;
    case QJsonValue::Type::Bool:
        std::cout<<jsonValue.toBool()<<std::endl;
        break;
    case QJsonValue::Type::Array:{
        std::cout<<std::endl;
        ++level;
        QJsonArray array=jsonValue.toArray();
        for(int i=0;i<=array.size();++i){
            printJsonValue(array[i],level);
        }
        break;
    }

    case QJsonValue::Type::Object:
        std::cout<<std::endl;
        ++level;
        printJsonObject(jsonValue.toObject(),level);
        break;
    case QJsonValue::Type::Undefined:
        std::cout<<"\"\""<<std::endl;
        break;
    }
}

QString JsonMgr::printIndent(int level)
{
    return QString("    ").repeated(level);
}

