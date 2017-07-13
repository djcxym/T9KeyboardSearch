#include "Converter.hpp"
#include<vector>
#include<sstream>

static const std::vector<std::string> UNICODE2PINYIN =
{
    #include "../Unicode2PinyinNoTone.inc"
};

std::vector<int> Converter::indexes;

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
    int index = (unicode - 0x4E00);
    if (index < 0 || index >= 20902)
    {
        return aChineseChar;
    }
    return UNICODE2PINYIN[index];
}

const std::string Converter::convertMultiPinyin(const std::string& aString)
{
    std::string str = aString;
    std::string pinyin;

    while (str.size() > 0)
    {
        std::string codeStr;
        bool isChinese;
        int size = GetFirstKindOfString(str, codeStr, isChinese);
        str.erase(0, size);
        if (isChinese)
        {
            std::string polyphone = ConvertKanji2Pinyin(codeStr);
            std::istringstream iss(polyphone);
            std::string pyPart;
            iss >> pyPart;
            pinyin += pyPart + " ";
        }
        else
        {
            const std::string drop = " ";
            // trim right
            codeStr.erase(codeStr.find_last_not_of(drop) + 1);
            // trim left
            codeStr.erase(0, codeStr.find_first_not_of(drop));

            if (codeStr.size() != 0)
            {
                pinyin += codeStr + " ";
            }
        }
    }
    //delete the last space
    if (pinyin.size() > 0)
    {
        pinyin.erase(pinyin.end() - 1);
    }
    return pinyin;
}

const  std::vector<std::vector<PBCodeUnit>> Converter::ConvertString2CodeUnits(const std::string& aString)
{
    /*
    cusOfAll is container of PBCodeUnit of the whole name, since we support polyphone, so the structure of cusOfAll is:
    eg:"重庆"=>[["chong","zhong"],["qing"]]
    cusOfAChar is container of a Chinese char or a alpha string, the structure is:
    eg:"重"=>["chong","zhong"]
    res is all the possible combination pinyin of a name, the structure is:
    eg:"重庆"=>[["chong","qing"],["zhong","qing"]]
    **/
    std::vector<std::vector<PBCodeUnit>> cusOfAll;
    std::string str = aString;
    while (str.size() > 0)
    {
        std::string originStr;
        bool isChinese;
        int size = GetFirstKindOfString(str, originStr, isChinese);
        str.erase(0, size);

        std::vector<PBCodeUnit> cusOfAChar;
        if (isChinese)
        {
            PBCodeUnit cu;
            std::string polyphone = ConvertKanji2Pinyin(originStr);
            std::istringstream iss(polyphone);
            std::string pinyin;
            while (iss >> pinyin)
            {
                cu.isChinese = true;
                cu.oriStr = originStr;
                cu.pyStr = pinyin;
                cusOfAChar.push_back(cu);
            }
        }
        else
        {
            PBCodeUnit cu;
            cu.isChinese = false;
            cu.oriStr = originStr;
            cu.pyStr = originStr;
            cusOfAChar.push_back(cu);
        }
        cusOfAll.push_back(cusOfAChar);
    }

    indexes = std::vector<int>(cusOfAll.size());

    std::vector<std::vector<PBCodeUnit>> res;
    CombinationCodeUnits(cusOfAll, 0, res);
    return res;
}

int Converter::GetFirstKindOfString(const std::string& aString, std::string &codeStr, bool &isChinese)
{
    int size;//字节数
    if (aString[0] & 0x80)
    {
        size = GetSizeOfUtf8Char(aString);
        codeStr = aString.substr(0, size);
        isChinese = true;
    }
    else
    {
        size_t index = 0;
        while (index < aString.size() && !(aString[index] & 0x80))
        {
            codeStr += aString[index];
            index++;
        }
        size = index;
        isChinese = false;
    }

    return size;
}

int Converter::GetSizeOfUtf8Char(const std::string& aChar)
{
    int size = 0;
    char c = aChar[0];
    while (c & 0x80)
    {
        size++;
        c <<= 1;
    }

    return size;
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

void Converter::CombinationCodeUnits(const std::vector<std::vector<PBCodeUnit>>& vec, size_t level, std::vector<std::vector<PBCodeUnit>>& res)
{
    std::vector<PBCodeUnit> vcu;
    if (level < vec.size())
    {
        for (size_t i = 0; i < vec[level].size(); i++)
        {
            indexes[level] = i;
            CombinationCodeUnits(vec, level + 1, res);
        }
    }
    else
    {
        for (size_t i = 0; i < vec.size(); i++)
        {
            vcu.push_back(vec[i][indexes[i]]);
        }
        res.push_back(vcu);
    }
}