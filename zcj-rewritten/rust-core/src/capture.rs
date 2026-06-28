//! capture.rs
//! 安全屏幕捕获与输入捕获抽象层
//!
//! 注意：此实现为授权、透明版本。
//! 不包含任何隐蔽技术。
//! 建议使用 Qt 或系统原生 API 实现实际捕获。

use anyhow::Result;
use serde::{Serialize, Deserialize};

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct CaptureFrame {
    pub timestamp: u64,
    pub width: u32,
    pub height: u32,
    pub format: String,      // "jpeg", "png", "raw"
    pub data: Vec<u8>,       // 压缩后的图像数据
}

/// 安全屏幕捕获接口（骨架）
/// 实际实现建议：
/// - Windows: DXGI Desktop Duplication 或 BitBlt
/// - Linux: X11 / Wayland / PipeWire
/// - 或直接使用 Qt QScreen::grabWindow
pub async fn capture_screen() -> Result<CaptureFrame> {
    // TODO: 真实实现
    tracing::info!("执行安全屏幕捕获（授权模式）");

    // 示例返回空帧
    Ok(CaptureFrame {
        timestamp: std::time::SystemTime::now()
            .duration_since(std::time::UNIX_EPOCH)
            .unwrap()
            .as_secs(),
        width: 1920,
        height: 1080,
        format: "jpeg".to_string(),
        data: vec![], // 实际应调用图像库压缩
    })
}

/// 输入事件捕获（只在授权情况下使用）
pub async fn start_input_capture() -> Result<()> {
    tracing::warn!("输入捕获已启动 - 请确保已获得授权");
    // TODO: 实现合法的输入监控
    Ok(())
}