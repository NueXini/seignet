#pragma once
#include <string>
#include <vector>

class MD5
{
public:
	MD5();
    // ���뺯��
    std::string Encode(std::string src);
    
private:
	size_t tempA, tempB, tempC, tempD, strlength;
    const size_t A = 0x67452301;
    const size_t B = 0xefcdab89;
    const size_t C = 0x98badcfe;
    const size_t D = 0x10325476;

    // F����
    size_t F(size_t b, size_t c, size_t d) {
        return (b & c) | ((~b) & d);
    }
    // G����
    size_t G(size_t b, size_t c, size_t d) {
        return (b & d) | (c & (~d));
    }
    // H����
    size_t H(size_t b, size_t c, size_t d) {
        return b ^ c ^ d;
    }
    // I����
    size_t I(size_t b, size_t c, size_t d) {
        return c ^ (b | (~d));
    }
    // ��λ��������
    size_t shift(size_t a, size_t n) {
        return (a << n) | (a >> (32 - n));
    }
    // ѭ��ѹ��
    void iterateFunc(size_t* X, int size = 16);
    // ����ַ���
    std::vector<size_t> padding(std::string src);
    // �������
    std::string format(size_t num);
};

