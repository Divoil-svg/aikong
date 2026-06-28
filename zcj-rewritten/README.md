# zcj-rewritten 现代化重构版

## 项目说明

本目录是对原仓库 `zcj/` 文件夹的完整重写版本。

**原项目特点**：
- AI 辅助修改的 C/C++ Windows 远程控制工具
- 包含远程访问、屏幕捕获、键盘记录、进程注入等模块
- Visual Studio 解决方案 (all.sln)

**重构目标**：
- 使用 **Qt (C++)** 构建现代化、跨平台 GUI
- 使用 **Rust** 实现核心安全层（网络通信、数据处理、内存安全）
- **C++** 负责高性能逻辑与 Qt 集成
- **C** 用于必要的低层 FFI 辅助（可选）
- 支持与您的 NVIDIA DGX Spark 集群集成本地 AI 模型，实现捕获数据智能分捐

## 重要安全声明

** 此重构版仅用于教育、授权渗透测试和合法远程管理场景 **

- 必须获得目标系统的** 明确授权 **后才能使用任何监控功能
- 禁止用于任何非法入侵、未授权数据获取或恶意行为
- 重构版已移除或抽象化了高风险隐蔽技术（如无授权注入、隐蔽键盘记录等）
- 推荐在测试环境中使用，并配合完善的审计日志和权限控制

如果您不同意以上声明，请勿继续使用本代码。

## 技术架构

```
aikong / zcj-rewritten/
├── README.md
├── rust-core/                 # Rust 核心库（安全通信、数据流、AI 集成）
│   ├── Cargo.toml
│   └── src/
│       ├── lib.rs
│       ├── network.rs          # QUIC/TLS 异步通信
│       ├── capture.rs          # 安全屏幕捕获抽象
│       └── ai_analyzer.rs      # 与本地 LLM 集成
├── qt-client/                # Qt C++ GUI 客户端
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── MainWindow.h
│   ├── MainWindow.cpp
│   └── RemoteDesktopView.h
├── common/                   # 共享协议定义
│   └── protocol.rs
└── build/                    # 构建脚本与示例配置
```

## 快速开始

### 1. 前置要求
- Rust 1.80+ (with tokio, quinn)
- Qt 6.5+ 开发环境 (Qt Creator 或 CMake)
- CMake 3.20+
- C++20

### 2. 构建 Rust 核心
```bash
cd zcj-rewritten/rust-core
cargo build --release
```

### 3. 构建 Qt GUI
```bash
cd zcj-rewritten/qt-client
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt6
make
./aikong-qt-client
```

## 功能列表（安全版）

- 安全连接管理 (mTLS + 身份验证)
- 实时远程桌面查看 (WebRTC-like 或压缩图像流)
- 命令执行与系统信息获取
- 文件上传/下载
- 捕获数据智能分析 (集成本地 uncensored LLM)
- 完善日志与审计

## 与您的 AI 硬件集成

您可以将捕获的屏幕图像、日志送到本地 DGX Spark 集群上的 uncensored 模型进行分析，例如：
- OCR 提取屏幕文本
- 异常行为检测
- 自动生成渗透测试报告

## 进一步开发

本目录当前为骨架版。如需完整实现某个模块或添加新功能，请告诉我具体需求，我会继续在此目录中追加文件。

**版权**：仅供学习与授权测试使用。违反法律后果自负。