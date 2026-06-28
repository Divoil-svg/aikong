# AI 集成指南

本文档说明如何将 **AIKong** 与您的 **NVIDIA DGX Spark** 集群 + 本地 uncensored LLM 集成，实现捕获数据的智能分析。

## 1. 架构概览

```
[Qt GUI] <--> [Rust Core] <--> [DGX Spark 集群]
     |                 |
     |                 +--> 本地 LLM 分析模型
     +--> 屏幕捕获、日志、命令结果
```

## 2. 推荐集成方式

### 方式A：通过 Rust 调用本地 OpenAI-compatible API

```rust
// 在 rust-core 中添加
use reqwest;

async fn analyze_with_llm(capture_data: &[u8]) -> anyhow::Result<String> {
    let client = reqwest::Client::new();
    // 发送到您的 DGX 上的 vLLM / Ollama / LM Studio
    let res = client.post("http://your-dgx-ip:8000/v1/chat/completions")
        .json(&json!({
            "model": "your-uncensored-model",
            "messages": [{
                "role": "user",
                "content": "分析以下屏幕捕获内容，检测异常行为..."
            }]
        }))
        .send().await?;
    // 解析结果
    Ok(res.text().await?)
}
```

### 方式B：使用 vLLM + Tensor Parallel (推荐您的双 DGX Spark)

在您的 DGX 上部署模型后，通过 Rust 的 `reqwest` 或 Python 桥接调用。

## 3. 实用场景

- 屏幕捕获后自动 OCR + 敏感信息检测
- 命令执行结果智能总结
- 异常行为检测与报告生成
- 渗透测试建议自动生成

## 4. 安全提醒

- 认证 Token 必须使用
- 仅在授权网络内部部署
- 建议对 LLM 输出进行人工审核

如需更多集成代码示例，请告诉我。