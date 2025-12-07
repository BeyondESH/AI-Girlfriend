# AI-Girlfriend (你的女孩)

一个基于 Qt 6 QML 开发的 AI 虚拟伴侣桌面应用程序，集成了语音识别 (ASR)、大语言模型 (LLM) 和语音合成 (TTS) 功能，支持文字聊天和语音对话两种交互模式。

## 功能特性

### 核心功能
- **文字聊天**: 与 AI 进行文字对话，支持实时流式响应
- **语音对话**: 支持语音输入，AI 语音回复，实现真正的语音交互体验
- **语音克隆**: 支持自定义 TTS 语音样本，可克隆任意声音
- **对话历史**: 自动保存对话记录，支持多会话管理
- **个性化定制**: 可自定义 AI 角色名称、性格、系统提示词等

### 服务状态监控
- 实时监控 Ollama、FunASR、CosyVoice 服务状态
- 自动重连机制，确保服务稳定性

### 界面特性
- 现代化 UI 设计，支持 FluentWinUI3 风格
- 响应式布局，自适应窗口大小
- 平滑动画效果，提升用户体验

## 技术架构

### 前端
- **Qt 6.8+** - 跨平台 GUI 框架
- **QML** - 声明式 UI 开发
- **Qt Quick Controls 2** - 现代化控件库

### 后端服务
- **Ollama** - 本地大语言模型服务 (默认端口: 11434)
- **FunASR** - 阿里开源语音识别服务 (默认端口: 10096)
- **CosyVoice** - 语音合成/克隆服务 (默认端口: 50000)

### 通信协议
- **WebSocket** - ASR 实时语音流传输
- **HTTP REST API** - LLM 和 TTS 服务调用

## 项目结构

```
AI-Girlfriend/
├── main.cpp              # 应用程序入口
├── Main.qml              # 主界面
├── application.cpp/h     # 应用程序核心逻辑
├── audiomgr.cpp/h        # 音频管理 (录音/播放)
├── configmgr.cpp/h       # 配置管理 (单例模式)
├── gateway.cpp/h         # 网络通信网关
├── CMakeLists.txt        # CMake 构建配置
├── res.qrc               # Qt 资源文件
├── Components/           # QML 组件
│   ├── ChatMessageListModel.qml
│   ├── HistoryListModel.qml
│   ├── MessageBubble.qml
│   └── SideButton.qml
├── Pages/                # QML 页面
│   ├── ChatPage.qml      # 文字聊天页面
│   ├── VoiceChatPage.qml # 语音对话页面
│   ├── SettingPage.qml   # 设置页面
│   └── StatePage.qml     # 服务状态页面
├── js/                   # JavaScript 工具函数
│   └── addListModel.js
├── img/icon/             # 图标资源
└── sample/               # 语音样本文件
    ├── huiyuanai.WAV
    ├── huiyuanai2.WAV
    └── xuan.wav
```

## 环境要求

### 开发环境
- Qt 6.8 或更高版本
- CMake 3.16+
- MSVC 2022 (Windows) 或 GCC/Clang (Linux/macOS)

### 运行时依赖
- Ollama 服务 (本地 LLM)
- FunASR WebSocket 服务 (语音识别)
- CosyVoice HTTP 服务 (语音合成)

## 构建说明

### Windows (MSVC)

```bash
# 创建构建目录
mkdir build
cd build

# 配置项目 (确保 Qt 在 PATH 中)
cmake .. -G "Ninja" -DCMAKE_PREFIX_PATH="C:/Qt/6.10.0/msvc2022_64"

# 编译
cmake --build .
```

### 使用 Qt Creator

1. 打开 `CMakeLists.txt` 作为项目文件
2. 选择合适的 Qt Kit (Qt 6.8+ MSVC 2022 64-bit)
3. 点击构建并运行

## 配置说明

### 默认配置

| 配置项 | 默认值 |
|--------|--------|
| 用户名称 | 用户 |
| AI 名称 | 雅萱 |
| ASR 服务地址 | ws://localhost:10096 |
| LLM 服务地址 | http://localhost:11434/api/chat |
| TTS 服务地址 | http://localhost:50000/inference_zero_shot |
| LLM 模型 | qwen3:8b |

### 自定义配置

所有配置项均可在应用程序的"设置"页面中修改，包括：

- **用户信息**: 用户名称、用户描述
- **AI 角色**: AI 名称、性格描述、系统提示词
- **服务器配置**: ASR/LLM/TTS 服务地址、模型名称
- **语音克隆**: TTS 提示文本、语音样本文件

## 后端服务部署

### Ollama

```bash
# 安装 Ollama
# Windows: 从 https://ollama.ai 下载安装程序

# 下载并运行模型
ollama run qwen3:8b
```

### FunASR

```bash
# 使用 Docker 部署
docker run -d \
  --name funasr \
  -p 10096:10096 \
  registry.cn-hangzhou.aliyuncs.com/funasr_repo/funasr:funasr-runtime-sdk-online-cpu-0.1.10

# 或参考官方文档: https://github.com/alibaba-damo-academy/FunASR
```

### CosyVoice

```bash
# 参考 CosyVoice 官方仓库部署
# https://github.com/FunAudioLLM/CosyVoice

# 启动推理服务，确保监听端口 50000
python webui.py --port 50000
```

## 使用说明

1. **启动后端服务**: 确保 Ollama、FunASR、CosyVoice 服务已启动
2. **运行应用程序**: 启动 AI-Girlfriend 应用
3. **检查服务状态**: 在"状态"页面确认所有服务在线
4. **开始对话**: 
   - 文字聊天：在聊天页面输入文字发送
   - 语音对话：在语音页面点击录音按钮开始对话

## 开发者信息

- **组织**: BeyondESH
- **域名**: beyondesh.top
- **应用名称**: 你的女孩

## 许可证

本项目仅供学习和研究使用。

## 更新日志

### v0.1
- 初始版本发布
- 实现文字聊天功能
- 实现语音对话功能
- 支持语音克隆
- 添加服务状态监控
- 添加个性化设置功能
