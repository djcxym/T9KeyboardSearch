#include"T9SearchHelper.hpp"
#include<sstream>
#include"../Converter/Converter.hpp"

T9SearchHelper::T9SearchHelper(const std::string& name)
    :mName(name)
{
}

T9SearchHelper::~T9SearchHelper()
{
}

const std::vector<std::vector<PBCodeUnit>> T9SearchHelper::SplitNameIntoCodeUnits()
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
    std::string name = mName;
    while (name.size() > 0)
    {
        std::string originStr;
        bool isChinese;
        int size = GetFirstCodeUnitOfName(name, originStr, isChinese);
        name.erase(0, size);

        std::vector<PBCodeUnit> cusOfAChar;
        if (isChinese)
        {
            PBCodeUnit cu;
            std::string polyphone = Converter::ConvertKanji2Pinyin(originStr);
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
    Combination(cusOfAll, 0, res);
    return res;
}

int T9SearchHelper::GetFirstCodeUnitOfName(const std::string& name, std::string &codeStr, bool &isChinese)
{
    int bytes;//字节数
    if (name[0] & 0x80)
    {
        int size = 1;
        char tmp = name[0];
        tmp <<= 1;
        do {
            tmp <<= 1;
            ++size;
        } while (tmp & 0x80);

        bytes = size;
        codeStr = name.substr(0, size);
        isChinese = true;
    }
    else
    {
        size_t index = 0;
        while (index < name.size() && !(name[index] & 0x80))
        {
            codeStr += name[index];
            index++;
            bytes = index;
        }
        isChinese = false;
    }

    return bytes;
}

void T9SearchHelper::Combination(const std::vector<std::vector<PBCodeUnit>>& vec, size_t level, std::vector<std::vector<PBCodeUnit>>& res)
{
    std::vector<PBCodeUnit> vcu;
    if (level < vec.size())
    {
        for (size_t i = 0; i < vec[level].size(); i++)
        {
            indexes[level] = i;
            Combination(vec, level + 1, res);
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