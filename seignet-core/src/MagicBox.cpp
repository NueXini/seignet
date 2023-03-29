#include "MagicBox.h"

long long MagicBox::magic()
{
	long long uVar1 = xkey[0];
	long long uVar6 = (xkey[3] + uVar1) & 0xffffffffL;
	long long uVar3 = xkey[1];
	long long uVar4 = xkey[4] ^ uVar3;
	long long uVar5 = uVar4 >> 0x10;
	long long uVar2 = uVar6 * 0x10000 & 0xffffffffL | uVar5;
	uVar2 = (uVar5 << 0x18 & 0xffffffffL | uVar2 >> 8) ^ (uVar5 << 0x12 & 0xffffffffL | uVar2 >> 0xe) ^ (uVar6 >> 6 & 0x3ff | uVar2 << 10 & 0xffffffffL) ^ (uVar6 >> 0xe & 3 | uVar2 << 2 & 0xffffffffL) ^ uVar2;
	xkey[0] = box2[(int)(uVar2 & 0xff)] | box1[(int)(uVar2 >> 8 & 0xff)] << 8 & 0xffffffffL | box2[(int)(uVar2 >> 0x10 & 0xff)] << 0x10 & 0xffffffffL | box1[(int)(uVar2 >> 0x18 & 0xff)] << 0x18 & 0xffffffffL;
	uVar6 = uVar6 >> 0x10;
	uVar2 = uVar4 << 0x10 & 0xffffffffL | uVar6;
	uVar2 = (uVar6 << 0x1e & 0xffffffffL | uVar2 >> 2) ^ (uVar6 << 0x16 & 0xffffffffL | uVar2 >> 10) ^ (uVar4 >> 2 & 0x3fff | uVar2 << 0xe & 0xffffffffL) ^ (uVar4 >> 8 & 0xff | uVar2 << 8 & 0xffffffffL) ^ uVar2;
	xkey[1] = box2[(int)(uVar2 & 0xff)] | box1[(int)(uVar2 >> 8 & 0xff)] << 8 & 0xffffffffL | box2[(int)(uVar2 >> 0x10 & 0xff)] << 0x10 & 0xffffffffL | box1[(int)(uVar2 >> 0x18 & 0xff)] << 0x18 & 0xffffffffL;
	return ((xkey[2] ^ uVar1) + uVar3) & 0xffffffffL;
}

std::string MagicBox::generate_uuid()
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(0, 15);

	const char* v = "0123456789abcdef";
	std::string uuid = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";

	int i = 0;

	for (char& c : uuid)
	{
		if (c == 'x' || c == 'y')
		{
			int r = dist(rng);
			c = v[(c == 'x') ? r : (r & 0x3 | 0x8)];
		}
	}

	return uuid;
}

std::string MagicBox::getCurrentTime()
{
	time_t now = time(nullptr);
	char buffer[80];
	struct tm timeinfo;
	localtime_s(&timeinfo, &now);
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
	return buffer;
}

std::string MagicBox::generate_checksum(std::string useragent, std::string clientid, std::string ipv4)
{
	try
	{
		const std::string content = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
			"<request>"
			"<user-agent>" +
			useragent + "</user-agent>" +
			"<client-id>" + clientid + "</client-id>" +
			"<local-time>" + getCurrentTime() + "</local-time>" +
			"<ipv4>" + ipv4 + "</ipv4>" +
			"<gwip></gwip>"
			"<mac></mac>"
			"<ipv6>null</ipv6>"
			"<sysinfo><sysname>Linux</sysname><ifname>wlan0," +
			ipv4 + "</ifname></sysinfo>" +
			"<ostag>Linux</ostag>"
			"<host-name>NueXini</host-name>"
			"</request>";
		MD5 md5;
		return md5.Encode(content);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
}
