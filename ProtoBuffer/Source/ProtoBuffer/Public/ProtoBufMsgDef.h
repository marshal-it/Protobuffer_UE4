#pragma once

namespace PBMSG
{
	//网络消息枚举编号
	enum NetMsg
	{
		Login_Request = 0x00001001,
		Login_Response = 0x00001002
	};

	//错误消息枚取
	enum ErrorMsg
	{
		Login_Error = 0
	};
}
