# AI-Girlfriend - AI女友语音交互系统

一个基于 Qt6 + QML 开发的 AI 语音交互应用，实现了完整的语音识别、大语言模型对话和语音合成功能。

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
├── audiomgr.h/cpp          # 音频管理模块（录音、PCM 数据采集）
├── websocketmgr.h/cpp      # WebSocket 管理模块（ASR 连接）
├── httpmgr.h/cpp           # HTTP 请求管理模块（LLM 接口）
├── messagemgr.h/cpp        # 消息管理模块（对话历史）
├── configmgr.h/cpp         # 配置管理模块（服务器配置）
├── threadmgr.h/cpp         # 线程池管理模块（异步任务）
├── jsonmgr.h/cpp           # JSON 工具类（调试输出）
├── global.h/cpp            # 全局定义（请求 ID、错误码）
├── main.cpp                # 应用程序入口
├── Main.qml                # 主界面
└── CMakeLists.txt          # CMake 构建配置
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

# 配置 CMake
cmake ..

# 编译
cmake --build . --config Debug
```

### 4. 运行应用

```bash
# Windows
.\Debug\appAIGirlfriend.exe

# Linux/macOS
./appAIGirlfriend
```

## 💡 使用说明

1. **启动服务**：确保 FunASR 和 Ollama 服务已启动
2. **连接服务器**：应用启动时自动连接 ASR 服务器
3. **开始录音**：点击"开始录制"按钮开始说话
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
