# AI-Girlfriend - AI女友语音交互系统

> ⚠️ **重构说明**: 项目已完成架构重构，采用模块化设计。查看 [重构总结](REFACTORING_SUMMARY.md) 了解详情。

一个基于 Qt6 + QML 开发的 AI 语音交互应用，实现了完整的语音识别、大语言模型对话和语音合成功能。

## 📚 文档导航

- **[架构设计](ARCHITECTURE.md)** - 详细的架构说明和模块职责
- **[使用指南](USAGE.md)** - 如何使用新架构开发
- **[迁移指南](MIGRATION.md)** - 从旧代码迁移到新架构
- **[重构总结](REFACTORING_SUMMARY.md)** - 重构的改进和最佳实践
- **[快速参考](QUICK_REFERENCE.md)** - 常用代码片段和API速查

## 📋 项目简介

AI-Girlfriend 是一个智能语音交互系统，可以通过语音与 AI 女友进行自然对话。系统采用模块化设计，集成了 ASR（语音识别）、LLM（大语言模型）和 TTS（语音合成）三大核心功能。

## ✨ 主要特性

- 🎙️ **实时语音识别**：基于 FunASR 实现高精度语音转文字
- 💬 **智能对话**：集成 Ollama 本地大语言模型，支持多轮对话
- 🔊 **语音合成**：TTS 语音合成功能（待集成）
- 🎨 **现代化界面**：基于 Qt6 QML 的美观用户界面
- ⚡ **异步处理**：多线程架构，保证界面流畅性
- 🔌 **WebSocket 通信**：实时双向数据传输

## 🛠️ 技术栈

### 前端
- **Qt 6.10.0**：跨平台应用框架
- **QML**：声明式 UI 开发
- **Qt Quick**：现代化 UI 组件

### 后端服务
- **FunASR**：阿里达摩院开源的语音识别服务（Docker 部署）
- **Ollama**：本地大语言模型服务
- **WebSocket**：实时通信协议

### 核心组件
- **Qt Multimedia**：音频采集与处理
- **Qt WebSockets**：WebSocket 客户端
- **Qt Network**：HTTP 请求处理

## 📦 项目结构

```
AIGirlfriend/
├── core/                       # 核心架构层
│   ├── eventbus.h/cpp          # 事件总线（发布-订阅）
│   └── servicelocator.h/cpp    # 服务定位器（IoC容器）
├── interfaces/                 # 接口抽象层
│   ├── iservice.h              # 服务基类接口
│   ├── iaudioservice.h         # 音频服务接口
│   ├── iasrservice.h           # ASR服务接口
│   ├── illmservice.h           # LLM服务接口
│   └── imessageservice.h       # 消息服务接口
├── services/                   # 服务实现层
│   ├── audioservice.h/cpp      # 音频服务实现
│   ├── asrservice.h/cpp        # ASR服务实现
│   ├── llmservice.h/cpp        # LLM服务实现
│   └── messageservice.h/cpp    # 消息服务实现
├── threadmgr.h/cpp             # 线程池管理器
├── configmgr.h/cpp             # 配置管理器（支持JSON）
├── jsonmgr.h/cpp               # JSON工具类
├── EventBus.h                  # 事件和枚举定义（State、ErrorCode、ReqId）
├── main.cpp                    # 应用入口（服务初始化）
├── mainwindow.h/cpp/ui         # 主窗口（纯UI控制器）
├── config.json                 # 配置文件
└── [legacy]                    # 旧代码（兼容保留）
    ├── audiomgr.h/cpp
    ├── websocketmgr.h/cpp
    ├── httpmgr.h/cpp
    └── messagemgr.h/cpp
```

### 架构层次图
```
┌─────────────────────────────────────────┐
│          UI Layer (MainWindow)          │  ← 仅负责UI交互
└──────────────┬──────────────────────────┘
               │ ServiceLocator
┌──────────────▼──────────────────────────┐
│       Service Locator (IoC容器)         │  ← 管理服务生命周期
└──────────────┬──────────────────────────┘
               │
┌──────────────▼──────────────────────────┐
│         Service Layer (服务层)           │  ← 业务逻辑实现
│  AudioService │ AsrService │ LlmService  │
│  MessageService │ ConfigMgr              │
└──────────────┬──────────────────────────┘
               │ EventBus
┌──────────────▼──────────────────────────┐
│        Event Bus (事件总线)              │  ← 模块间通信
└──────────────┬──────────────────────────┘
               │
┌──────────────▼──────────────────────────┐
│    Infrastructure (基础设施层)           │  ← 工具类
│  ThreadMgr │ ConfigMgr │ JsonMgr        │
└─────────────────────────────────────────┘
```

## 🚀 快速开始

### 环境要求

- **Qt 6.10.0+**（需要 Multimedia、WebSockets 模块）
- **CMake 3.16+**
- **MSVC 2022**（Windows）或 GCC/Clang（Linux/macOS）
- **Docker**（用于部署 FunASR 服务）
- **Ollama**（本地运行大语言模型）

### 1. 部署 FunASR 语音识别服务

使用 Docker 部署 FunASR：

```bash
# 拉取 FunASR 镜像
docker pull registry.cn-hangzhou.aliyuncs.com/funasr_repo/funasr:funasr-runtime-sdk-cpu-0.4.5

# 运行 FunASR 服务（CPU 版本）
docker run -p 10096:10095 -it --privileged=true \
  registry.cn-hangzhou.aliyuncs.com/funasr_repo/funasr:funasr-runtime-sdk-cpu-0.4.5
```

服务启动后，WebSocket 地址为：`ws://localhost:10096`

### 2. 安装 Ollama 并下载模型

```bash
# 安装 Ollama（参考官网：https://ollama.ai）
# Windows: 下载安装包直接安装

# 下载 Qwen 模型
ollama pull qwen3:8b
```

Ollama API 地址：`http://localhost:11434/api/chat`

### 3. 编译项目

```bash
# 克隆项目
git clone https://gitee.com/BeyondESH/AI-Girlfriend.git
cd AI-Girlfriend/AIGirlfriend

# 创建构建目录
mkdir build && cd build

# 配置 CMake（确保已安装Qt 6.10+）
cmake ..

# 编译
cmake --build . --config Debug
```

### 4. 配置应用

创建或编辑 `config.json`（与可执行文件同目录）：

```json
{
  "user": {
    "name": "用户名",
    "uid": "用户ID"
  },
  "server": {
    "asr_url": "ws://localhost:10096",
    "llm_url": "http://localhost:11434/api/chat",
    "llm_model": "qwen3:8b",
    "tts_url": ""
  }
}
```

### 5. 运行应用

```bash
# Windows
.\Debug\appAIGirlfriend.exe

# Linux/macOS
./appAIGirlfriend
```

## 💡 使用说明

1. **启动服务**：确保 FunASR 和 Ollama 服务已启动
2. **配置检查**：确认 `config.json` 中的服务器地址正确
3. **启动应用**：运行 AIGirlFriend.exe
4. **开始对话**：
   - 点击"开始录制"按钮
   - 说出你想说的话
   - 点击"停止录制"
   - 等待AI回复

## 🏗️ 架构特性 (v2.0)

### 1. 解耦架构
- **接口抽象层**：所有服务实现接口，便于替换和测试
- **事件驱动**：通过EventBus通信，模块间零依赖
- **依赖注入**：ServiceLocator管理服务，支持IoC

### 2. 事件总线
```cpp
// 发布事件
EventBus::instance().publish(Events::AUDIO_PCM_DATA, pcmData);

// 订阅事件
EventBus::instance().subscribe(Events::ASR_RESULT, this,
    [](const QVariant& data) {
        QString result = data.toString();
        // 处理识别结果
    });
```

### 3. 服务定位器
```cpp
// 获取服务
auto audioService = ServiceLocator::instance()
    .getService<IAudioService>(ServiceIds::AUDIO_SERVICE);

// 使用服务
audioService->start();
```

## 📚 文档

- [架构文档 (ARCHITECTURE.md)](ARCHITECTURE.md) - 详细的架构设计说明
- [重构指南 (REFACTORING.md)](REFACTORING.md) - v2.0重构说明和迁移指南
- [API文档 (API.md)](API.md) - 接口和服务API说明

## 🔧 开发指南

### 添加新服务

1. 在 `interfaces/` 定义接口
```cpp
class INewService : public IService {
public:
    virtual void doSomething() = 0;
};
```

2. 在 `services/` 实现服务
```cpp
class NewService : public QObject, public INewService {
    Q_OBJECT
public:
    bool initialize() override;
    void start() override;
    void stop() override;
    void doSomething() override;
};
```

3. 注册服务（main.cpp）
```cpp
auto newService = std::make_shared<NewService>();
ServiceLocator::instance().registerService<INewService>(
    "NewService", newService);
```

### 使用事件通信

```cpp
// 定义事件类型
namespace Events {
    constexpr const char* NEW_EVENT = "new.event";
}

// 发布事件
EventBus::instance().publish(Events::NEW_EVENT, data);

// 订阅事件
EventBus::instance().subscribe(Events::NEW_EVENT, this, callback);
```

## 🧪 测试

```bash
# 运行测试（如果有）
cd build
ctest
```

## 🐛 常见问题

### Q: ASR连接失败？
A: 检查FunASR Docker容器是否运行，端口是否正确（10096）

### Q: LLM无响应？
A: 确认Ollama服务运行，模型已下载（`ollama pull qwen3:8b`）

### Q: 编译错误？
A: 确保Qt 6.10+已正确安装，CMake能找到Qt路径

### Q: 配置文件不生效？
A: 确认 `config.json` 与可执行文件在同一目录

## 🚀 路线图

- [x] v1.0 基础功能实现
- [x] v2.0 架构重构（解耦、事件驱动）
- [ ] v2.1 TTS语音合成集成
- [ ] v2.2 对话历史持久化（数据库）
- [ ] v2.3 WebUI界面（QML/Web）
- [ ] v3.0 插件系统
- [ ] v3.1 云端部署支持

## 📄 许可证

本项目采用 MIT 许可证。详见 [LICENSE](LICENSE) 文件。

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 👥 作者

- **BeyondESH** - *初始工作* - [Gitee](https://gitee.com/BeyondESH)

## 🙏 致谢

- [FunASR](https://github.com/alibaba-damo-academy/FunASR) - 阿里达摩院语音识别
- [Ollama](https://ollama.ai) - 本地大语言模型
- [Qt Framework](https://www.qt.io) - 跨平台应用框架

---

**版本**: v2.0 (重构版)  
**更新日期**: 2025-01-09  
**项目主页**: https://gitee.com/BeyondESH/AI-Girlfriend
4. **结束录音**：点击"结束录制"按钮，系统将识别语音并返回文本
5. **查看结果**：在控制台查看识别结果和 AI 回复

## 🔧 配置说明

### 音频配置

当前音频格式配置（`audiomgr.cpp`）：
- **采样率**：16000 Hz
- **声道数**：单声道（Mono）
- **采样格式**：Int16
- **编码格式**：PCM

### 服务器配置

在 `configmgr.h` 中定义了默认配置：

```cpp
// ASR 服务器地址
static constexpr const char* DEFAULT_ASR_URL = "ws://localhost:10096";

// LLM 服务器地址
static constexpr const char* DEFAULT_LLM_URL = "http://localhost:11434/api/chat";

// 默认模型
static constexpr const char* DEFAULT_MODEL = "qwen3:8b";
```

### FunASR 配置

WebSocket 连接参数（`websocketmgr.cpp`）：

```json
{
  "mode": "2pass",
  "wav_name": "realtime_recording",
  "is_speaking": true,
  "wav_format": "pcm",
  "chunk_size": [5, 10, 5],
  "audio_fs": 16000,
  "itn": true
}
```

## 📊 模块说明

### AudioMgr（音频管理）
- 音频设备初始化和格式配置
- 实时音频数据采集
- PCM 数据流处理
- 信号：`signal_handlePcmData`、`signal_endRecord`

### WebSocketMgr（WebSocket 管理）
- ASR 服务器连接管理
- 实时发送 PCM 音频数据
- 接收识别结果
- 信号：`signal_handleAsrMessage`

### HttpMgr（HTTP 管理）
- 与 Ollama LLM 服务通信
- 支持 GET/POST 请求
- 异步请求处理
- 信号：`signals_http_finished`

### MessageMgr（消息管理）
- 处理 ASR 识别结果
- 管理对话历史（待完善）
- 消息结构定义

### ConfigMgr（配置管理）
- 单例模式管理全局配置
- 支持用户信息和服务器配置
- Qt 属性绑定支持

### ThreadMgr（线程池管理）
- 异步任务提交
- 支持有返回值和无返回值任务
- 基于 QThreadPool 实现

## 🎯 开发路线

### 已完成 ✅
- [x] 音频采集模块
- [x] WebSocket 通信（ASR）
- [x] FunASR 语音识别集成
- [x] HTTP 请求封装
- [x] Ollama LLM 对话集成
- [x] 基础 UI 界面
- [x] 配置管理系统

### 开发中 🚧
- [ ] TTS 语音合成集成
- [ ] 对话历史管理
- [ ] 完善 UI 交互
- [ ] 消息展示界面
- [ ] 流式对话显示

### 计划中 📋
- [ ] 用户配置界面
- [ ] 多语言支持
- [ ] 语音唤醒功能
- [ ] 对话上下文管理
- [ ] 情感分析
- [ ] 语音情绪识别
- [ ] 自定义 AI 人设
- [ ] 对话数据持久化

## 🔍 常见问题

### Q: 无法连接 ASR 服务器？
A: 请确保 Docker 容器正在运行，端口映射正确（10096:10095）

### Q: 录音没有声音？
A: 检查系统麦克风权限，确保选择了正确的音频输入设备

### Q: Ollama 请求失败？
A: 确保 Ollama 服务已启动，并且已下载对应的模型（qwen3:8b）

### Q: 编译错误？
A: 检查 Qt 版本是否为 6.10.0+，确保安装了 Multimedia 和 WebSockets 模块

## 🤝 贡献

欢迎提交 Issue 和 Pull Request！

## 📄 许可证

本项目采用 MIT 许可证，详见 [LICENSE](LICENSE) 文件。

## 🙏 致谢

- [FunASR](https://github.com/alibaba-damo-academy/FunASR) - 阿里达摩院语音识别服务
- [Ollama](https://ollama.ai) - 本地大语言模型运行平台
- [Qt](https://www.qt.io) - 跨平台应用开发框架

## 📧 联系方式

- Gitee: [@BeyondESH](https://gitee.com/BeyondESH)
- 项目地址: https://gitee.com/BeyondESH/AI-Girlfriend

---

⭐ 如果这个项目对你有帮助，欢迎 Star！
