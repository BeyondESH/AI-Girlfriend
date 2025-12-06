#ifndef CONFIGMGR_H
#define CONFIGMGR_H

#include <QObject>
#include <QSettings>

class ConfigMgr : public QObject
{
    Q_OBJECT
    // 用户信息
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged FINAL)
    Q_PROPERTY(QString userDescription READ userDescription WRITE setUserDescription NOTIFY userDescriptionChanged FINAL)
    
    // AI角色设定
    Q_PROPERTY(QString aiName READ aiName WRITE setAiName NOTIFY aiNameChanged FINAL)
    Q_PROPERTY(QString aiPersonality READ aiPersonality WRITE setAiPersonality NOTIFY aiPersonalityChanged FINAL)
    Q_PROPERTY(QString systemPrompt READ systemPrompt WRITE setSystemPrompt NOTIFY systemPromptChanged FINAL)
    
    // 服务器配置
    Q_PROPERTY(QString asrServerUrl READ asrServerUrl WRITE setAsrServerUrl NOTIFY asrServerUrlChanged FINAL)
    Q_PROPERTY(QString llmServerUrl READ llmServerUrl WRITE setLlmServerUrl NOTIFY llmServerUrlChanged FINAL)
    Q_PROPERTY(QString llmModelName READ llmModelName WRITE setLlmModelName NOTIFY llmModelNameChanged FINAL)
    Q_PROPERTY(QString ttsServerUrl READ ttsServerUrl WRITE setTtsServerUrl NOTIFY ttsServerUrlChanged FINAL)
    
    // TTS语音克隆设置
    Q_PROPERTY(QString ttsPromptText READ ttsPromptText WRITE setTtsPromptText NOTIFY ttsPromptTextChanged FINAL)
    Q_PROPERTY(QString ttsPromptWav READ ttsPromptWav WRITE setTtsPromptWav NOTIFY ttsPromptWavChanged FINAL)
    Q_PROPERTY(int currentVoiceSample READ currentVoiceSample WRITE setCurrentVoiceSample NOTIFY currentVoiceSampleChanged FINAL)
    Q_PROPERTY(QString customVoiceName READ customVoiceName WRITE setCustomVoiceName NOTIFY customVoiceNameChanged FINAL)

public:
    static ConfigMgr& instance();
    
    // 用户信息
    QString userName() const;
    void setUserName(const QString &newUserName);
    QString userDescription() const;
    void setUserDescription(const QString &newUserDescription);
    
    // AI角色设定
    QString aiName() const;
    void setAiName(const QString &newAiName);
    QString aiPersonality() const;
    void setAiPersonality(const QString &newAiPersonality);
    QString systemPrompt() const;
    void setSystemPrompt(const QString &newSystemPrompt);
    
    // 服务器配置
    QString asrServerUrl() const;
    void setAsrServerUrl(const QString &newAsrServerUrl);
    QString llmServerUrl() const;
    void setLlmServerUrl(const QString &newLlmServerUrl);
    QString llmModelName() const;
    void setLlmModelName(const QString &newLlmModelName);
    QString ttsServerUrl() const;
    void setTtsServerUrl(const QString &newTtsServerUrl);
    
    // TTS语音克隆设置
    QString ttsPromptText() const;
    void setTtsPromptText(const QString &newTtsPromptText);
    QString ttsPromptWav() const;
    void setTtsPromptWav(const QString &newTtsPromptWav);
    int currentVoiceSample() const;
    void setCurrentVoiceSample(int newCurrentVoiceSample);
    QString customVoiceName() const;
    void setCustomVoiceName(const QString &newCustomVoiceName);
    
    // 构建完整的系统提示词
    Q_INVOKABLE QString buildFullSystemPrompt() const;
    
    // 保存和加载配置
    Q_INVOKABLE void saveConfig();
    Q_INVOKABLE void loadConfig();
    Q_INVOKABLE void resetToDefault();

private:
    ConfigMgr();
    ~ConfigMgr();
    ConfigMgr(const ConfigMgr&) = delete;
    ConfigMgr& operator=(const ConfigMgr&) = delete;

    QSettings *_settings;

    // 用户信息
    QString _userName;
    QString _userDescription;
    
    // AI角色设定
    QString _aiName;
    QString _aiPersonality;
    QString _systemPrompt;

    // 服务器配置
    QString _asrServerUrl;
    QString _llmServerUrl;
    QString _llmModelName;
    QString _ttsServerUrl;
    
    // TTS语音克隆设置
    QString _ttsPromptText;
    QString _ttsPromptWav;
    int _currentVoiceSample;
    QString _customVoiceName;

    // 默认值
    static constexpr const char* DEFAULT_USER_NAME = "用户";
    static constexpr const char* DEFAULT_AI_NAME = "雅萱";
    static constexpr const char* DEFAULT_ASR_URL = "ws://localhost:10096";
    static constexpr const char* DEFAULT_LLM_URL = "http://localhost:11434/api/chat";
    static constexpr const char* DEFAULT_TTS_URL = "http://localhost:50000/inference_zero_shot";
    static constexpr const char* DEFAULT_MODEL = "qwen3:8b";
    static constexpr const char* DEFAULT_TTS_PROMPT_TEXT = "不过，应该没事吧，如果那个姓黑田的人，真的就是你所怀疑的朗姆，又在那么近的距离看到我这张脸，照理说应该会察觉我就是背叛组织的雪莉，这个时候，应该早就闯进了博士家才对，但是刚才博士传来的简讯，都是在说今天晚餐的事";
    static constexpr const char* DEFAULT_TTS_PROMPT_WAV = ":/sample/huiyuanai2.WAV";
    
    static const QString DEFAULT_SYSTEM_PROMPT;
    static const QString DEFAULT_AI_PERSONALITY;

signals:
    void userNameChanged();
    void userDescriptionChanged();
    void aiNameChanged();
    void aiPersonalityChanged();
    void systemPromptChanged();
    void asrServerUrlChanged();
    void llmServerUrlChanged();
    void llmModelNameChanged();
    void ttsServerUrlChanged();
    void ttsPromptTextChanged();
    void ttsPromptWavChanged();
    void currentVoiceSampleChanged();
    void customVoiceNameChanged();
};

#endif // CONFIGMGR_H
