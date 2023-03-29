#include "MD5.h"

MD5::MD5()
{
	tempA = A;
	tempB = B;
	tempC = C;
	tempD = D;
	strlength = 0;
}

std::string MD5::Encode(std::string src)
{
	std::vector<size_t> rec = padding(src);
	for (size_t i = 0; i < strlength / 16; i++) {
		size_t num[16];
		for (int j = 0; j < 16; j++) {
			num[j] = rec[i * 16 + j];
		}
		iterateFunc(num, 16);
	}
	return format(tempA) + format(tempB) + format(tempC) + format(tempD);
}

void MD5::iterateFunc(size_t* X, int size)
{
	size_t a = tempA,
		b = tempB,
		c = tempC,
		d = tempD,
		rec = 0,
		g, k;
	const size_t s[] = { 7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
						   5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
						   4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
						   6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21 };
	const size_t T[] = {
	0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
	0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
	0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,
	0x6b901122,0xfd987193,0xa679438e,0x49b40821,
	0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,
	0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
	0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,
	0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
	0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,
	0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
	0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
	0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
	0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,
	0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
	0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,
	0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391 };
	for (int i = 0; i < 64; i++) {
		if (i < 16) {
			// F迭代
			g = F(b, c, d);
			k = i;
		}
		else if (i < 32) {
			// G迭代
			g = G(b, c, d);
			k = (1 + 5 * i) % 16;
		}
		else if (i < 48) {
			// H迭代
			g = H(b, c, d);
			k = (5 + 3 * i) % 16;
		}
		else {
			// I迭代
			g = I(b, c, d);
			k = (7 * i) % 16;
		}
		rec = d;
		d = c;
		c = b;
		b = b + shift(a + g + X[k] + T[i], s[i]);
		a = rec;
	}
	tempA += a;
	tempB += b;
	tempC += c;
	tempD += d;
}

std::vector<size_t> MD5::padding(std::string src)
{
	size_t num = ((src.length() + 8) / 64) + 1;
	std::vector<size_t> rec(num * 16);
	strlength = num * 16;
	for (size_t i = 0; i < src.length(); i++) {
		// 一个size_t对应4个字节，保存4个字符信息
		rec[i >> 2] |= (int)(src[i]) << ((i % 4) * 8);
	}
	// 补充1000...000
	rec[src.length() >> 2] |= (0x80 << ((src.length() % 4) * 8));
	// 填充原文长度
	rec[rec.size() - 2] = (src.length() << 3);
	return rec;
}

std::string MD5::format(size_t num)
{
	std::string res = "";
	const char str16[] = "0123456789abcdef";
	size_t base = 1 << 8;
	for (int i = 0; i < 4; i++) {
		std::string tmp = "";
		size_t b = (num >> (i * 8)) % base & 0xff;
		for (int j = 0; j < 2; j++) {
			tmp = str16[b % 16] + tmp;
			b /= 16;
		}
		res += tmp;
	}
	return res;
}
