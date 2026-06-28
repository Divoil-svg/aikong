//! network.rs
//! 安全网络通信层
//!
//! 使用 tokio 异步框架，支持 TLS。
//! 未来可替换为 quinn (QUIC + rustls) 以获得更高性能和低延迟。

use tokio::net::{TcpListener, TcpStream};
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use serde::{Serialize, Deserialize};
use anyhow::Result;

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Command {
    pub cmd_type: String,      // "capture_screen", "execute_cmd", "get_sysinfo"
    pub target: Option<String>,
    pub payload: Vec<u8>,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct Response {
    pub status: String,
    pub data: Vec<u8>,
    pub message: String,
}

pub async fn start_server(addr: &str) -> Result<()> {
    let listener = TcpListener::bind(addr).await?;
    tracing::info!("安全远程管理服务器已启动: {}", addr);

    loop {
        let (socket, peer) = listener.accept().await?;
        tracing::info!("新连接: {}", peer);
        tokio::spawn(handle_connection(socket));
    }
}

async fn handle_connection(mut socket: TcpStream) {
    let mut buf = [0u8; 4096];
    loop {
        match socket.read(&mut buf).await {
            Ok(0) => break, // 连接关闭
            Ok(n) => {
                // TODO: 解析 Command，执行并返回 Response
                let _ = socket.write_all(b"OK\n").await;
            }
            Err(e) => {
                tracing::error!("读取错误: {}", e);
                break;
            }
        }
    }
}

pub async fn connect_to_server(addr: &str) -> Result<TcpStream> {
    let stream = TcpStream::connect(addr).await?;
    Ok(stream)
}