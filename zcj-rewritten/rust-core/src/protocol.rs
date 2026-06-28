//! protocol.rs
//! 共享协议定义
//!
//! 使用 serde 进行序列化，支持 JSON / bincode 等格式

use serde::{Serialize, Deserialize};

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Command {
    pub cmd_type: String,
    pub target: Option<String>,
    pub payload: Vec<u8>,
    pub timestamp: u64,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Response {
    pub status: String,      // "success", "error", "pending"
    pub data: Vec<u8>,
    pub message: String,
    pub timestamp: u64,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct CaptureRequest {
    pub quality: u8,         // 1-100
    pub format: String,      // jpeg / png / webp
    pub region: Option<(u32, u32, u32, u32)>, // x,y,w,h
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct SystemInfo {
    pub hostname: String,
    pub os: String,
    pub cpu_usage: f32,
    pub memory_total: u64,
    pub memory_used: u64,
}

// 未来可添加更多协议类型：FileTransfer, KeyEvent, InjectRequest 等