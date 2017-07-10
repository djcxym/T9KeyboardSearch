#include "Converter.hpp"
#include<vector>

static const std::vector<std::string> UNICODE2PINYIN =
{
    #include "../Unicode2PinyinNoTone.inc"
};

const std::string Converter::ConvertString2NumberString(const std::string& str)
{
    const std::string alphanum = "22233344455566677778889999";
    std::string res = "";
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        char tmp = str[i];
        if (tmp >= 'a' && tmp <= 'z')
        {
            res += alphanum[tmp - 'a'];
        }
        else if (tmp >= 'A' && tmp <= 'Z')
        {
            res += alphanum[tmp - 'A'];
        }
        else
        {
            res += tmp;
        }
    }

    return res;
}

const std::string Converter::ConvertKanji2Pinyin(const std::string& aChineseChar)
{
    int unicode = ConvertUtf8ToUnicode(aChineseChar);
    if (0x3007 == unicode)
    {
        return "ling2";
    }
    int index = (unicode - 0x4E00) % UNICODE2PINYIN.size();
    return UNICODE2PINYIN[index];
}

int Converter::ConvertUtf8ToUnicode(const std::string& aChineseChar)
{
    unsigned char* p = (unsigned char*)aChineseChar.c_str();
    int uni;
    if (*p >= 0xFC)//11111100
    {
        uni  = (p[0] & 0x01) << 30;
        uni |= (p[1] & 0x3F) << 24;
        uni |= (p[2] & 0x3F) << 18;
        uni |= (p[3] & 0x3F) << 12;
        uni |= (p[4] & 0x3F) << 6;
        uni |= (p[5] & 0x3F);
    }
    else if (*p >= 0xF8)//11111000
    {
        uni  = (p[0] & 0x03) << 24;
        uni |= (p[1] & 0x3F) << 18;
        uni |= (p[2] & 0x3F) << 12;
        uni |= (p[3] & 0x3F) << 6;
        uni |= (p[4] & 0x3F);
    }
    else if (*p >= 0xF0)//11110000
    {
        uni  = (p[0] & 0x07) << 18;
        uni |= (p[1] & 0x3F) << 12;
        uni |= (p[2] & 0x3F) << 6;
        uni |= (p[3] & 0x3F);
    }
    else if (*p >= 0xE0)//11100000
    {
        uni  = (p[0] & 0x0F) << 12;
        uni |= (p[1] & 0x3F) << 6;
        uni |= (p[2] & 0x3F);
    }
    else if (*p >= 0xC0)//11000000
    {
        uni  = (p[0] & 0x1F) << 6;
        uni |= (p[1] & 0x3F);
    }
    else
    {
        uni = p[0];
    }

    return uni;
}