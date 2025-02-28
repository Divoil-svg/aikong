#pragma once

#include  <stdint.h>
#include <winsock2.h>

//固定起始报文
#define PACKET_START_SING  (0xffffffff)


//屏幕数据
#define PACKET_TYPE_SCREEN  0x001

//鼠标
#define PACKET_TYPE_MOUSE_EVENT  0x02

//键盘
#define PACKET_TYPE_KEY_EVENT  0x03

//READY 信号
#define PACKET_TYPE_READY  0x100

//READY 信号 回复
#define PACKET_TYPE_READY_PEPLY  0x101


#pragma pack(push, 4)

//起始标志+包类型+长度+正文长度正文  正文
struct PacketHead
{
	//0xffffffff  uint32 max
	uint32_t start_sign;

	//类型
	uint32_t type;

	//正文长度
	uint32_t len;
};


//键盘事件包数据
struct KeybdEventData
{
	int VK;
	bool down;
};

//鼠标包数据
struct MouseEventData
{
	 int dx;
	 int dy;

	unsigned int action;
	//滚轮
	int wheel;
};

//Ready 报文，由被控制端发到控制端，其中有 视频高宽
struct ReadyData
{
	unsigned int video_L;
	unsigned int video_T;

	unsigned int video_w;
	unsigned int video_h;
};


#pragma pack(pop)

