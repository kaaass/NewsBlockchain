#include "sha256.h"

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

//静态数据成员初始化

vector<uint32_t> Sha256::ori_mes_digest =
{
        0x6a09e667, 0xbb67ae85, 0x3c6ef372,
        0xa54ff53a, 0x510e527f, 0x9b05688c,
        0x1f83d9ab, 0x5be0cd19

};

vector<uint32_t> Sha256::constvalue =
{
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2

};


//方法实现

UInt32 Sha256::run(const ByteBuffer& buffer)
{
    unsigned int nleft = buffer.size();//得到字符串的长度，以char为单位。

    std::string str = "";
    std::string ans;

    for (int i = 0; i < nleft; i++)
    {
        str += buffer[i];
    }
    str += "\0";

    ans = toHexCode(str);

    unsigned short int key = 0;
    for (int i = 0; i < 8; i++)
    {
        key *= 16;
        key += s16toi(ans[4 * i]);

    }

    return key;
}

int Sha256::s16toi(char c)
{
    int ans;

    switch (c)
    {
    case '0':
        ans = 0;
        break;

    case '1':
        ans = 1;
        break;

    case '2':
        ans = 2;
        break;

    case '3':
        ans = 3;
        break;

    case '4':
        ans = 4;
        break;

    case '5':
        ans = 5;
        break;

    case '6':
        ans = 6;
        break;

    case '7':
        ans = 7;
        break;

    case '8':
        ans = 8;
        break;

    case '9':
        ans = 9;
        break;

    case 'a':
    case 'A':
        ans = 10;
        break;

    case 'b':
    case 'B':
        ans = 11;
        break;

    case 'c':
    case 'C':
        ans = 12;
        break;

    case 'd':
    case 'D':
        ans = 13;
        break;

    case 'e':
    case 'E':
        ans = 14;
        break;

    case 'f':
    case 'F':
        ans = 15;
        break;

    default:
        ans = 0;
        break;
    }

    return ans;

}

string Sha256::toHexCode(const string& message)
{
    //判断信息是否为空
    if (!message.empty())
    {
        //信息非空，
        //处理信息
        vector<uint8_t> deal_mes;
        for (auto i = message.begin(); i != message.end(); i++)
        {
            deal_mes.push_back(static_cast<uint8_t>(*i));
        }

        //生成sha256的加密信息
        vector<uint8_t> code;
        deal(deal_mes, &code);

        //转换为16进制数输出
        ostringstream out;
        out << hex << setiosflags(ios::uppercase);
        for (auto i = code.begin(); i != code.end(); i++)
        {
            out << setw(2) << setfill('0')
                << static_cast<unsigned short>(*i);
        }

        return out.str();

    }
    else
    {
        return "";
    }
}

int Sha256::deal(const vector<uint8_t>& message, vector<uint8_t>* _digest)
{
    if (!message.empty() && _digest)
    {
        //文本预处理
        vector<uint8_t> mes = message;
        datapreprocess(&mes);

        //分解成512bit为一组（块），扩展64*32
        vector<vector<uint8_t>> result;
        devide64(mes, &result);
        vector<uint32_t> message_digest(ori_mes_digest);

        //循环加密
        vector<uint32_t> words;
        for (const auto& chunk : result)
        {
            expand48(chunk, &words);
            hashtocode64(words, &message_digest);
        }

        //将结果转换成16进制
        finalhashvalue(message_digest, _digest);

        return 1;
    }
    else
    {
        return 0;
    }


}

int Sha256::datapreprocess(vector<uint8_t>* message) const
{
    if (message)
    {
        uint64_t original_bit_size = message->size() * 8;

        //检测是否通过超过448位
        int remainder = message->size() % 64;
        if (remainder < 56)
        {
            message->push_back(0x80);
            for (int i = 1; i < 56 - remainder; i++)
            {
                message->push_back(0x00);
            }
        }
        else
        {
            if (remainder == 56)
            {
                message->push_back(0x80);
                for (int i = 1; i < 64; i++)
                {
                    message->push_back(0x00);
                }
            }
            else
            {
                message->push_back(0x80);
                for (int i = 1; i < 64 - remainder + 56; i++)
                {
                    message->push_back(0x00);
                }
            }
        }

        //填充文本要满足512bit的倍数
        //在最后64位bit处填写原文本的长度
        for (int i = 1; i <= 8; i++)
        {
            int temp = 64 - 8 * i;
            uint8_t c = static_cast<uint8_t>(original_bit_size >> temp);
            message->push_back(c);
        }

        return 1;
    }
    else
    {
        return 0;
    }


}

int Sha256::devide64(const vector<uint8_t>& message, vector<vector<uint8_t>>* heap64) const
{
    if (heap64 && 0 == message.size() % 64)
    {
        heap64->clear();

        int quotient = message.size() / 64;
        for (int i = 0; i < quotient; i++)
        {
            int temp_i = 64 * i;

            vector<uint8_t> temp(message.begin() + temp_i, message.begin() + temp_i + 64);
            heap64->push_back(temp);
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

int Sha256::expand48(const vector<uint8_t>& heap64, vector<uint32_t>* result) const
{
    if (result && 64 == heap64.size())
    {
        result->resize(64);

        auto& words = *result;
        for (int i = 0; i < 16; i++)
        {
            words[i] = (static_cast<uint32_t>(heap64[i * 4]) << 24)
                | (static_cast<uint32_t>(heap64[i * 4 + 1]) << 16)
                | (static_cast<uint32_t>(heap64[i * 4 + 2]) << 8)
                | static_cast<uint32_t>(heap64[i * 4 + 3]);
        }
        for (int i = 16; i < 64; i++)
        {
            words[i] = small_sigma1(words[i - 2])
                + words[i - 7]
                + small_sigma0(words[i - 15])
                + words[i - 16];
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

int Sha256::hashtocode64(const vector<uint32_t>& result, vector<uint32_t>* code64) const
{
    if (code64 && 8 == code64->size() && 64 == result.size())
    {
        vector<uint32_t> d = *code64;

        for (int i = 0; i < 64; i++)
        {
            uint32_t temp1 = d[7] + big_sigma1(d[4]) + ch(d[4], d[5], d[6]) + constvalue[i] + result[i];
            uint32_t temp2 = big_sigma0(d[0]) + maj(d[0], d[1], d[2]);

            d[7] = d[6];
            d[6] = d[5];
            d[5] = d[4];
            d[4] = d[3] + temp1;
            d[3] = d[2];
            d[2] = d[1];
            d[1] = d[0];
            d[0] = temp1 + temp2;
        }

        for (int i = 0; i < 8; i++)
        {
            (*code64)[i] += d[i];
        }

        return 1;
    }
    else
    {
        return 0;
    }
}

int Sha256::finalhashvalue(const vector<uint32_t>& input, vector<uint8_t>* output) const
{
    if (output)
    {
        output->clear();

        for (auto it = input.begin(); it != input.end(); ++it)
        {
            for (int i = 0; i < 4; i++)
            {
                int temp = 24 - 8 * i;
                output->push_back(static_cast<uint8_t>((*it) >> temp));
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }
}