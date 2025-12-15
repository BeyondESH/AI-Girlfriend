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
    Q_PROPERTY(QString customVoiceWav READ customVoiceWav WRITE setCustomVoiceWav NOTIFY customVoiceWavChanged FINAL)
    Q_PROPERTY(QString customVoiceText READ customVoiceText WRITE setCustomVoiceText NOTIFY customVoiceTextChanged FINAL)

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
    QString customVoiceWav() const;
    void setCustomVoiceWav(const QString &newCustomVoiceWav);
    QString customVoiceText() const;
    void setCustomVoiceText(const QString &newCustomVoiceText);
    
    // 应用当前选中的语音样本到 TTS 配置
    Q_INVOKABLE void applyCurrentVoiceSample();
    
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
    QString _customVoiceWav;    // 自定义样本文件路径
    QString _customVoiceText;   // 自定义样本参考文本

    // 默认值
    static constexpr const char* DEFAULT_USER_NAME = "用户";
    static constexpr const char* DEFAULT_AI_NAME = "雅萱";
    static constexpr const char* DEFAULT_ASR_URL = "ws://127.0.0.1:10096";
    static constexpr const char* DEFAULT_LLM_URL = "http://127.0.0.1:11434/api/chat";
    static constexpr const char* DEFAULT_TTS_URL = "http://127.0.0.1:50000/inference_zero_shot";
    static constexpr const char* DEFAULT_MODEL = "qwen3:8b";
    static constexpr const char* DEFAULT_TTS_PROMPT_TEXT = "不过应该没问题吧，毕竟那个暗号，跟夏洛克福尔摩斯故事中出现的跳舞的小人暗号非常地相似，如果是拍下照片，目前正在观察的这位福尔摩斯迷，一定已经解开了，才对";
    static constexpr const char* DEFAULT_TTS_PROMPT_WAV = ":/sample/huiyuanai.WAV";
    
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
    void customVoiceWavChanged();
    void customVoiceTextChanged();
};

#endif // CONFIGMGR_H
