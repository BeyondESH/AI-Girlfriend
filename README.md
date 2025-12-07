# AI-Girlfriend（你的女孩）

一个基于Qt6和QML开发的AI虚拟女友应用程序，支持文字聊天和语音对话功能。通过集成本地大语言模型（LLM）、自动语音识别（ASR）和文本转语音（TTS）技术，为用户提供自然流畅的AI对话体验。

## ✨ 主要特性

- 🎨 **现代化UI界面**：采用FluentWinUI3风格的Qt Quick界面，美观流畅
- 💬 **智能文字聊天**：支持与AI进行多轮对话，具备上下文记忆能力
- 🎤 **语音对话功能**：支持实时语音识别和AI语音回复
- 🎵 **语音克隆技术**：支持自定义TTS语音音色
- 👤 **可定制AI人设**：自定义AI角色名称、性格特点和系统提示词
- 📝 **对话历史管理**：自动保存对话记录，支持创建、加载和删除对话
- ⚙️ **灵活的配置系统**：支持配置用户信息、AI设定和服务器连接
- 🔌 **服务状态监控**：实时检测ASR、LLM和TTS服务的在线状态

## 🛠️ 技术栈

- **开发框架**：Qt 6.8+ (Qt Quick, QML)
- **编程语言**：C++17
- **UI风格**：FluentWinUI3
- **主要模块**：
  - Qt Quick - UI界面
  - Qt Multimedia - 音频录制与播放
  - Qt WebSockets - ASR服务通信
  - Qt Network - HTTP请求处理

## 📋 前置要求

### 系统要求
- Windows 10/11, macOS 10.15+, 或 Linux
- CMake 3.16+
- Qt 6.8+

### Qt组件
确保安装以下Qt组件：
- Qt Quick
- Qt Core
- Qt Multimedia
- Qt WebSockets
- Qt Quick Controls 2

### 后端服务

应用需要以下后端服务支持：

1. **大语言模型服务（LLM）**
   - 推荐使用 [Ollama](https://ollama.ai/)
   - 默认地址：`http://localhost:11434/api/chat`
   - 默认模型：`qwen3:8b`

2. **语音识别服务（ASR）**
   - 支持FunASR或其他兼容的ASR服务
   - 默认地址：`ws://localhost:10096`

3. **文本转语音服务（TTS）**
   - 支持GPT-SoVITS等语音合成服务
   - 默认地址：`http://localhost:50000/inference_zero_shot`

## 🚀 安装步骤

### 1. 克隆仓库

```bash
git clone https://github.com/BeyondESH/AI-Girlfriend.git
cd AI-Girlfriend
```

### 2. 安装Qt和依赖

确保已安装Qt 6.8或更高版本，包含所需的模块。

### 3. 构建项目

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### 4. 运行应用

```bash
# Linux/macOS
./appAI-Girlfriend

# Windows
appAI-Girlfriend.exe
```

## 📖 使用说明

### 首次启动

1. 启动应用后，首先进入**设置页面**配置服务器连接
2. 在"状态"页面检查所有服务是否在线
3. 配置完成后即可开始对话

### 文字聊天

1. 在侧边栏选择**聊天**页面
2. 在输入框输入消息并发送
3. AI将基于设定的人设进行回复
4. 支持多轮对话，具备上下文记忆

### 语音对话

1. 在侧边栏选择**语音对话**页面
2. 点击麦克风按钮开始录音
3. 说话完毕后松开按钮，系统将自动识别并生成AI回复
4. AI回复将自动转换为语音播放

### 对话管理

- **新建对话**：点击对话列表旁的"+"按钮
- **切换对话**：从历史列表中选择已有对话
- **删除对话**：点击对话项的删除按钮
- **清空当前对话**：在设置中选择清空选项

## ⚙️ 配置指南

### 用户信息设置

在设置页面可以配置：
- **用户名称**：AI对话中如何称呼你
- **用户描述**：关于你的背景信息（可选）

### AI角色设定

- **AI名称**：AI的角色名字（默认：雅萱）
- **AI性格**：描述AI的性格特点和行为方式
- **系统提示词**：自定义AI的行为规则和角色设定

### 服务器配置

- **ASR服务器**：语音识别服务地址
- **LLM服务器**：大语言模型API地址
- **LLM模型**：使用的具体模型名称
- **TTS服务器**：语音合成服务地址

### 语音克隆设置

- **提示文本**：用于语音克隆的参考文本
- **提示音频**：参考语音文件路径
- **语音样本**：选择预设的语音样本（内置3个样本）
- **自定义语音名称**：为自定义语音设置名称

## 📁 项目结构

```
AI-Girlfriend/
├── main.cpp                # 应用程序入口
├── application.h/cpp       # 应用主逻辑
├── audiomgr.h/cpp         # 音频管理模块
├── gateway.h/cpp          # 网络通信网关
├── configmgr.h/cpp        # 配置管理模块
├── Main.qml               # 主窗口UI
├── Components/            # UI组件
│   ├── SideButton.qml
│   ├── MessageBubble.qml
│   ├── ChatMessageListModel.qml
│   └── HistoryListModel.qml
├── Pages/                 # 页面
│   ├── ChatPage.qml       # 文字聊天页面
│   ├── VoiceChatPage.qml  # 语音对话页面
│   ├── SettingPage.qml    # 设置页面
│   └── StatePage.qml      # 状态页面
├── js/                    # JavaScript工具
├── img/                   # 图标资源
├── sample/                # 语音样本
└── CMakeLists.txt         # CMake构建配置
```

## 🔧 开发说明

### 编码规范

- C++代码遵循Qt编码规范
- 使用驼峰命名法
- 私有成员变量以下划线开头

### 构建系统

项目使用CMake作为构建系统，支持跨平台编译。

### 依赖管理

所有Qt依赖通过`find_package`自动查找，确保系统已安装Qt开发环境。

## 🤝 贡献指南

欢迎提交问题和拉取请求！

1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启拉取请求

## 📝 常见问题

### Q: 应用启动后无法连接服务？
A: 请检查ASR、LLM和TTS服务是否正常运行，并确认配置的服务器地址正确。

### Q: 如何更换AI的声音？
A: 在设置页面的语音克隆设置中，选择不同的语音样本或上传自定义音频样本。

### Q: 支持哪些大语言模型？
A: 支持任何兼容OpenAI聊天API格式的模型，推荐使用Ollama部署的本地模型。

### Q: 对话历史保存在哪里？
A: 对话历史保存在系统应用数据目录的`conversations`文件夹中。

## 📄 许可证

本项目采用 [MIT License](LICENSE) 开源许可证。

## 👨‍💻 作者

**BeyondESH**
- GitHub: [@BeyondESH](https://github.com/BeyondESH)
- Website: [beyondesh.top](https://beyondesh.top)

## 🙏 致谢

感谢所有为本项目做出贡献的开发者，以及以下开源项目：
- [Qt](https://www.qt.io/) - 跨平台应用框架
- [Ollama](https://ollama.ai/) - 本地LLM运行环境
- [FunASR](https://github.com/alibaba-damo-academy/FunASR) - 语音识别
- [GPT-SoVITS](https://github.com/RVC-Boss/GPT-SoVITS) - 语音合成

---

⭐ 如果这个项目对你有帮助，欢迎给个Star！
