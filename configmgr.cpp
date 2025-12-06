#include "configmgr.h"

// 默认系统提示词
const QString ConfigMgr::DEFAULT_SYSTEM_PROMPT = R"(你是一个名叫%1的AI助手。请以友好、自然的方式与用户%2对话。
你的性格特点：%3
请用第一人称回答问题，保持对话简洁自然。)";

// 默认AI性格描述
const QString ConfigMgr::DEFAULT_AI_PERSONALITY = R"(温柔体贴、善解人意、偶尔会有些俏皮。喜欢关心对方的日常生活，会在合适的时候给予鼓励和支持。说话时会用一些语气词如"嘛"、"呢"、"呐"来增加亲和力。)";

ConfigMgr &ConfigMgr::instance()
{
    static ConfigMgr instance;
    return instance;
}

ConfigMgr::ConfigMgr()
    : _settings(new QSettings(this))
    , _currentVoiceSample(0)
{
    loadConfig();
}

ConfigMgr::~ConfigMgr()
{
    saveConfig();
}

// 用户信息
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

QString ConfigMgr::userDescription() const
{
    return _userDescription;
}

void ConfigMgr::setUserDescription(const QString &newUserDescription)
{
    if (_userDescription == newUserDescription)
        return;
    _userDescription = newUserDescription;
    emit userDescriptionChanged();
}

// AI角色设定
QString ConfigMgr::aiName() const
{
    return _aiName;
}

void ConfigMgr::setAiName(const QString &newAiName)
{
    if (_aiName == newAiName)
        return;
    _aiName = newAiName;
    emit aiNameChanged();
}

QString ConfigMgr::aiPersonality() const
{
    return _aiPersonality;
}

void ConfigMgr::setAiPersonality(const QString &newAiPersonality)
{
    if (_aiPersonality == newAiPersonality)
        return;
    _aiPersonality = newAiPersonality;
    emit aiPersonalityChanged();
}

QString ConfigMgr::systemPrompt() const
{
    return _systemPrompt;
}

void ConfigMgr::setSystemPrompt(const QString &newSystemPrompt)
{
    if (_systemPrompt == newSystemPrompt)
        return;
    _systemPrompt = newSystemPrompt;
    emit systemPromptChanged();
}

// 服务器配置
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

// TTS语音克隆设置
QString ConfigMgr::ttsPromptText() const
{
    return _ttsPromptText;
}

void ConfigMgr::setTtsPromptText(const QString &newTtsPromptText)
{
    if (_ttsPromptText == newTtsPromptText)
        return;
    _ttsPromptText = newTtsPromptText;
    emit ttsPromptTextChanged();
}

QString ConfigMgr::ttsPromptWav() const
{
    return _ttsPromptWav;
}

void ConfigMgr::setTtsPromptWav(const QString &newTtsPromptWav)
{
    if (_ttsPromptWav == newTtsPromptWav)
        return;
    _ttsPromptWav = newTtsPromptWav;
    emit ttsPromptWavChanged();
}

int ConfigMgr::currentVoiceSample() const
{
    return _currentVoiceSample;
}

void ConfigMgr::setCurrentVoiceSample(int newCurrentVoiceSample)
{
    if (_currentVoiceSample == newCurrentVoiceSample)
        return;
    _currentVoiceSample = newCurrentVoiceSample;
    emit currentVoiceSampleChanged();
}

QString ConfigMgr::customVoiceName() const
{
    return _customVoiceName;
}

void ConfigMgr::setCustomVoiceName(const QString &newCustomVoiceName)
{
    if (_customVoiceName == newCustomVoiceName)
        return;
    _customVoiceName = newCustomVoiceName;
    emit customVoiceNameChanged();
}

QString ConfigMgr::buildFullSystemPrompt() const
{
    if (!_systemPrompt.isEmpty()) {
        return _systemPrompt;
    }
    return DEFAULT_SYSTEM_PROMPT.arg(_aiName, _userName, _aiPersonality);
}

void ConfigMgr::saveConfig()
{
    _settings->beginGroup("User");
    _settings->setValue("userName", _userName);
    _settings->setValue("userDescription", _userDescription);
    _settings->endGroup();
    
    _settings->beginGroup("AI");
    _settings->setValue("aiName", _aiName);
    _settings->setValue("aiPersonality", _aiPersonality);
    _settings->setValue("systemPrompt", _systemPrompt);
    _settings->endGroup();
    
    _settings->beginGroup("Server");
    _settings->setValue("asrServerUrl", _asrServerUrl);
    _settings->setValue("llmServerUrl", _llmServerUrl);
    _settings->setValue("llmModelName", _llmModelName);
    _settings->setValue("ttsServerUrl", _ttsServerUrl);
    _settings->endGroup();
    
    _settings->beginGroup("TTS");
    _settings->setValue("ttsPromptText", _ttsPromptText);
    _settings->setValue("ttsPromptWav", _ttsPromptWav);
    _settings->setValue("currentVoiceSample", _currentVoiceSample);
    _settings->setValue("customVoiceName", _customVoiceName);
    _settings->endGroup();
    
    _settings->sync();
}

void ConfigMgr::loadConfig()
{
    _settings->beginGroup("User");
    _userName = _settings->value("userName", DEFAULT_USER_NAME).toString();
    _userDescription = _settings->value("userDescription", "").toString();
    _settings->endGroup();
    
    _settings->beginGroup("AI");
    _aiName = _settings->value("aiName", DEFAULT_AI_NAME).toString();
    _aiPersonality = _settings->value("aiPersonality", DEFAULT_AI_PERSONALITY).toString();
    _systemPrompt = _settings->value("systemPrompt", "").toString();
    _settings->endGroup();
    
    _settings->beginGroup("Server");
    _asrServerUrl = _settings->value("asrServerUrl", DEFAULT_ASR_URL).toString();
    _llmServerUrl = _settings->value("llmServerUrl", DEFAULT_LLM_URL).toString();
    _llmModelName = _settings->value("llmModelName", DEFAULT_MODEL).toString();
    _ttsServerUrl = _settings->value("ttsServerUrl", DEFAULT_TTS_URL).toString();
    _settings->endGroup();
    
    _settings->beginGroup("TTS");
    _ttsPromptText = _settings->value("ttsPromptText", DEFAULT_TTS_PROMPT_TEXT).toString();
    _ttsPromptWav = _settings->value("ttsPromptWav", DEFAULT_TTS_PROMPT_WAV).toString();
    _currentVoiceSample = _settings->value("currentVoiceSample", 0).toInt();
    _customVoiceName = _settings->value("customVoiceName", "自定义语音").toString();
    _settings->endGroup();
}

void ConfigMgr::resetToDefault()
{
    _userName = DEFAULT_USER_NAME;
    _userDescription = "";
    _aiName = DEFAULT_AI_NAME;
    _aiPersonality = DEFAULT_AI_PERSONALITY;
    _systemPrompt = "";
    _asrServerUrl = DEFAULT_ASR_URL;
    _llmServerUrl = DEFAULT_LLM_URL;
    _llmModelName = DEFAULT_MODEL;
    _ttsServerUrl = DEFAULT_TTS_URL;
    _ttsPromptText = DEFAULT_TTS_PROMPT_TEXT;
    _ttsPromptWav = DEFAULT_TTS_PROMPT_WAV;
    _currentVoiceSample = 0;
    _customVoiceName = "自定义语音";
    
    emit userNameChanged();
    emit userDescriptionChanged();
    emit aiNameChanged();
    emit aiPersonalityChanged();
    emit systemPromptChanged();
    emit asrServerUrlChanged();
    emit llmServerUrlChanged();
    emit llmModelNameChanged();
    emit ttsServerUrlChanged();
    emit ttsPromptTextChanged();
    emit ttsPromptWavChanged();
    emit currentVoiceSampleChanged();
    emit customVoiceNameChanged();
    
    saveConfig();
}
