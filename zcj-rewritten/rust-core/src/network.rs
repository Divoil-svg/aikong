//! network.rs
//! 安全网络通信层 - 已支持结构化命令与捕获帧传输

use tokio::net::{TcpListener, TcpStream};
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use serde_json;
use anyhow::Result;
use crate::protocol::{Command, Response, CaptureFrame};

pub async fn start_server(addr: &str) -> Result<()> {
    let listener = TcpListener::bind(addr).await?;
    tracing::info!("安全远程管理服务器已启动: {}", addr);

    loop {
        let (socket, peer) = listener.accept().await?;
        tracing::info!("新连接来自: {}", peer);
        tokio::spawn(handle_connection(socket));
    }
}

async fn handle_connection(mut socket: TcpStream) {
    let mut buf = vec![0u8; 8192];

    loop {
        match socket.read(&mut buf).await {
            Ok(0) => {
                tracing::info!("客户端断开连接");
                break;
            }
            Ok(n) => {
                let data = &buf[..n];
                if let Ok(cmd) = serde_json::from_slice::<Command>(data) {
                    let response = process_command(cmd).await;
                    let response_json = serde_json::to_vec(&response).unwrap();
                    let _ = socket.write_all(&response_json).await;
                }
            }
            Err(e) => {
                tracing::error!("读取错误: {}", e);
                break;
            }
        }
    }
}

async fn process_command(cmd: Command) -> Response {
    match cmd.cmd_type.as_str() {
        "capture_screen" => {
            // TODO: 调用真实捕获
            let frame = CaptureFrame {
                timestamp: std::time::SystemTime::now()
                    .duration_since(std::time::UNIX_EPOCH)
                    .unwrap()
                    .as_secs(),
                width: 1920,
                height: 1080,
                format: "jpeg".to_string(),
                data: vec![], // 实际应填入压缩图像
            };
            let data = serde_json::to_vec(&frame).unwrap();
            Response {
                status: "success".to_string(),
                data,
                message: "屏幕捕获成功".to_string(),
                timestamp: std::time::SystemTime::now()
                    .duration_since(std::time::UNIX_EPOCH)
                    .unwrap()
                    .as_secs(),
            }
        }
        "execute_cmd" => {
            Response {
                status: "success".to_string(),
                data: b"Command executed (demo)".to_vec(),
                message: format!("命令 '{}' 已执行". , cmd.payload.iter().map(|&b| b as char).collect::<String>()),
                timestamp: std::time::SystemTime::now()
                    .duration_since(std::time::UNIX_EPOCH)
                    .unwrap()
                    .as_secs(),
            }
        }
        _ => Response {
            status: "error".to_string(),
            data: vec![],
            message: "未知命令类型".to_string(),
            timestamp: std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .unwrap()
                .as_secs(),
        },
    }
}

pub async fn connect_and_send_command(addr: &str, cmd: Command) -> Result<Response> {
    let mut stream = TcpStream::connect(addr).await?;
    let cmd_json = serde_json::to_vec(&cmd)?;
    stream.write_all(&cmd_json).await?;

    let mut buf = vec![0u8; 8192];
    let n = stream.read(&mut buf).await?;
    let response: Response = serde_json::from_slice(&buf[..n])?;
    Ok(response)
}