#include "CrcHash.h"

#include <cstdlib>
#include <cstring>
//
UInt32 CrcHash::run(const ByteBuffer& buffer) {

	char const * str = reinterpret_cast<char const *>(buffer.data());

    unsigned int        nleft = buffer.size();//得到字符串的长度，以char为单位。
    unsigned long long  sum = 0;
    unsigned short int* w = (unsigned short int*)str; //将char*转换为short int*
    unsigned short int  answer = 0;

    /*
     * Our algorithm is simple, using a 32 bit accumulator (sum), we add
     * sequential 16 bit words to it, and at the end, fold back all the
     * carry bits from the top 16 bits into the lower 16 bits.
     */
    while (nleft > 1) {
        sum += *w++;
        nleft -= 2; //nleft是以char型计数的，是short int的2倍
    }
    /*
     * mop up an odd byte, if necessary(处理奇数情况)
     */
    if (1 == nleft) {
        *(unsigned char*)(&answer) = *(unsigned char*)w;//去w的低8bit
        sum += answer;
    }
    /*
     * add back carry outs from top 16 bits to low 16 bits
     * add hi 16 to low 16
     */
    sum = (sum >> 16) + (sum & 0xFFFF);
    /* add carry */
    sum += (sum >> 16);
    /* truncate to 16 bits */
    answer = ~sum;

    return (answer & 0xFFFFFFFF);

}
