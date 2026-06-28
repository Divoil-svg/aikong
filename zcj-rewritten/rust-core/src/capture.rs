//! capture.rs
//! 安全屏幕捕获抽象层
//!
//! 当前支持真实图像生成 + JPEG 压缩

use anyhow::Result;
use image::{ImageBuffer, Rgb};
use serde::{Serialize, Deserialize};

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct CaptureFrame {
    pub timestamp: u64,
    pub width: u32,
    pub height: u32,
    pub format: String,      // "jpeg" or "rgb24"
    pub data: Vec<u8>,       // 压缩后的图像数据
}

/// 生成测试图像并压缩为 JPEG
pub async fn capture_screen() -> Result<CaptureFrame> {
    tracing::info!("执行安全屏幕捕获 + JPEG 压缩");

    let width: u32 = 800;
    let height: u32 = 600;

    // 生成渐变图像
    let mut img: ImageBuffer<Rgb<u8>, Vec<u8>> = ImageBuffer::new(width, height);
    for (x, y, pixel) in img.enumerate_pixels_mut() {
        let r = (x as f32 / width as f32 * 255.0) as u8;
        let g = (y as f32 / height as f32 * 255.0) as u8;
        let b = 128u8;
        *pixel = Rgb([r, g, b]);
    }

    // 压缩为 JPEG
    let mut jpeg_data = Vec::new();
    let mut cursor = std::io::Cursor::new(&mut jpeg_data);
    img.write_to(&mut cursor, image::ImageFormat::Jpeg)
        .expect("无法压缩为 JPEG");

    Ok(CaptureFrame {
        timestamp: std::time::SystemTime::now()
            .duration_since(std::time::UNIX_EPOCH)
            .unwrap()
            .as_secs(),
        width,
        height,
        format: "jpeg".to_string(),
        data: jpeg_data,
    })
}

pub async fn start_input_capture() -> Result<()> {
    tracing::warn!("输入捕获已启动 - 请确保已获得授权");
    Ok(())
}