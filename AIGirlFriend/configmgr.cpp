#include "configmgr.h"

ConfigMgr &ConfigMgr::instance()
{
    static ConfigMgr instance;
    return instance;
}

ConfigMgr::ConfigMgr() {}

QString ConfigMgr::ttsServerUrl() const
{
    return _ttsServerUrl;
}

void ConfigMgr::setTtsServerUrl(const QString &newTtsServerUrl)
{
    if (_ttsServerUrl == newTtsServerUrl)
        return;
    _ttsServerUrl = newTtsServerUrl;
    emit ttsServerUrlChanged();
}

QString ConfigMgr::llmModelName() const
{
    return _llmModelName;
}

void ConfigMgr::setLlmModelName(const QString &newLlmModelName)
{
    if (_llmModelName == newLlmModelName)
        return;
    _llmModelName = newLlmModelName;
    emit llmModelNameChanged();
}

QString ConfigMgr::llmServerUrl() const
{
    return _llmServerUrl;
}

void ConfigMgr::setLlmServerUrl(const QString &newLlmServerUrl)
{
    if (_llmServerUrl == newLlmServerUrl)
        return;
    _llmServerUrl = newLlmServerUrl;
    emit llmServerUrlChanged();
}

QString ConfigMgr::asrServerUrl() const
{
    return _asrServerUrl;
}

void ConfigMgr::setAsrServerUrl(const QString &newAsrServerUrl)
{
    if (_asrServerUrl == newAsrServerUrl)
        return;
    _asrServerUrl = newAsrServerUrl;
    emit asrServerUrlChanged();
}

QString ConfigMgr::uid() const
{
    return _uid;
}

void ConfigMgr::setUid(const QString &newUid)
{
    if (_uid == newUid)
        return;
    _uid = newUid;
    emit uidChanged();
}

QString ConfigMgr::password() const
{
    return _password;
}

void ConfigMgr::setPassword(const QString &newPassword)
{
    if (_password == newPassword)
        return;
    _password = newPassword;
    emit passwordChanged();
}

QString ConfigMgr::userName() const
{
    return _userName;
}

void ConfigMgr::setUserName(const QString &newUserName)
{
    if (_userName == newUserName)
        return;
    _userName = newUserName;
    emit userNameChanged();
}
