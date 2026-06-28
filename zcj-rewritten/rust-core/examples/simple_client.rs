//! simple_client.rs
//! 简单客户端示例
//!
//! 运行方式：
//!   cargo run --example simple_client -- --server 127.0.0.1:8443

use aikong_core::protocol::Command;
use aikong_core::network;
use clap::Parser;
use tracing_subscriber;

#[derive(Parser, Debug)]
struct Args {
    #[arg(short, long, default_value = "127.0.0.1:8443")]
    server: String,
}

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    tracing_subscriber::fmt::init();

    let args = Args::parse();

    println!("连接到服务器: {}", args.server);

    // 示例1：请求屏幕捕获
    let capture_cmd = Command {
        cmd_type: "capture_screen".to_string(),
        target: None,
        payload: vec![],
        timestamp: std::time::SystemTime::now()
            .duration_since(std::time::UNIX_EPOCH)
            .unwrap()
            .as_secs(),
    };

    let response = network::connect_and_send_command(&args.server, capture_cmd).await?;
    println!("\u5c4f幕捕获响应: status={}, message={}", response.status, response.message);

    // 示例2：执行命令
    let exec_cmd = Command {
        cmd_type: "execute_cmd".to_string(),
        target: None,
        payload: b"whoami".to_vec(),
        timestamp: std::time::SystemTime::now()
            .duration_since(std::time::UNIX_EPOCH)
            .unwrap()
            .as_secs(),
    };

    let exec_response = network::connect_and_send_command(&args.server, exec_cmd).await?;
    println!("\u547d令执行响应: {}", exec_response.message);

    Ok(())
}