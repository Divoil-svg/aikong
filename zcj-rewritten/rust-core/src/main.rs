//! aikong-server
//! 可运行的安全远程管理服务器示例
//!
//! 使用方式：
//!   cargo run --bin aikong-server -- --listen 0.0.0.0:8443

use aikong_core::{network, protocol};
use clap::Parser;
use tracing_subscriber;

#[derive(Parser, Debug)]
#[command(author, version, about = "AIKong Modern Remote Admin Server")]
struct Args {
    /// 监听地址
    #[arg(short, long, default_value = "0.0.0.0:8443")]
    listen: String,
}

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    tracing_subscriber::fmt::init();

    let args = Args::parse();

    println!("启动 AIKong 安全服务器 ...");
    println!("监听地址: {}", args.listen);
    println!("警告: 请确保此服务器只在授权环境中使用!");

    network::start_server(&args.listen).await?;

    Ok(())
}