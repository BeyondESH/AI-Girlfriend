#ifndef CONFIGMGR_H
#define CONFIGMGR_H

#include <QObject>
#include <QSettings>

class ConfigMgr:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged FINAL)
    Q_PROPERTY(QString uid READ uid WRITE setUid NOTIFY uidChanged FINAL)
    Q_PROPERTY(QString asrServerUrl READ asrServerUrl WRITE setAsrServerUrl NOTIFY asrServerUrlChanged FINAL)
    Q_PROPERTY(QString llmServerUrl READ llmServerUrl WRITE setLlmServerUrl NOTIFY llmServerUrlChanged FINAL)
    Q_PROPERTY(QString llmModelName READ llmModelName WRITE setLlmModelName NOTIFY llmModelNameChanged FINAL)
    Q_PROPERTY(QString ttsServerUrl READ ttsServerUrl WRITE setTtsServerUrl NOTIFY ttsServerUrlChanged FINAL)

public:
    static ConfigMgr& instance();
    QString userName() const;
    void setUserName(const QString &newUserName);

    QString password() const;
    void setPassword(const QString &newPassword);

    QString uid() const;
    void setUid(const QString &newUid);

    QString asrServerUrl() const;
    void setAsrServerUrl(const QString &newAsrServerUrl);

    QString llmServerUrl() const;
    void setLlmServerUrl(const QString &newLlmServerUrl);

    QString llmModelName() const;
    void setLlmModelName(const QString &newLlmModelName);

    QString ttsServerUrl() const;
    void setTtsServerUrl(const QString &newTtsServerUrl);

private:
    ConfigMgr();
    ~ConfigMgr();
    ConfigMgr(const ConfigMgr&) = delete;
    ConfigMgr& operator=(const ConfigMgr&) = delete;

    QSettings *_settings;

    // 用户信息
    QString _userName;
    QString _password;
    QString _uid;

    // 服务器配置
    QString _asrServerUrl;
    QString _llmServerUrl;
    QString _llmModelName;
    QString _ttsServerUrl;


    // 默认值
    static constexpr const char* DEFAULT_ASR_URL = "ws://localhost:10096";
    static constexpr const char* DEFAULT_LLM_URL = "http://localhost:11434/api/chat";
    static constexpr const char* DEFAULT_MODEL = "qwen3:8b";



signals:

    void userNameChanged();

    void passwordChanged();

    void uidChanged();

    void asrServerUrlChanged();

    void llmServerUrlChanged();

    void llmModelNameChanged();

    void ttsServerUrlChanged();

public slots:

};

#endif // CONFIGMGR_H
