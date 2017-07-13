#include"T9Search.hpp"
#include<string>
#include<vector>
#include"../Converter/Converter.hpp"

bool T9Search::Search(const std::string& name, const std::string& key, std::string& hilight)
{
    std::vector<std::vector<PBCodeUnit>> cusOfAll = Converter::ConvertString2CodeUnits(name);

    for (size_t i = 0; i < cusOfAll.size(); i++)
    {
        hilight = "";
        if (FirstCharSearchImpl(0, cusOfAll[i], key, hilight))
        {
            return true;
        }

        hilight = "";
        if (SearchImpl(0, cusOfAll[i], key, hilight))
        {
            return true;
        }
    }
    return false;
}

bool T9Search::SearchImpl(int index, const std::vector<PBCodeUnit>& cus, const std::string& key, std::string &hilight)
{
    std::string searchkey = key;
    bool hasMatched = false;

    for (size_t i = index; i < cus.size(); i++)
    {
        PBCodeUnit cu = cus[i];
        std::string numstr = Converter::ConvertString2NumberString(cu.pyStr);

        if (numstr[0] == searchkey[0])
        {
            hasMatched = true;
            if (cu.isChinese)//是否是中文
            {
                hilight += cu.oriStr;

                if (searchkey.size() <= numstr.size() && searchkey == numstr.substr(0, searchkey.size()))
                {
                    return true;
                }

                if (numstr == searchkey.substr(0, numstr.size()))
                {
                    searchkey.erase(0, numstr.size());
                }
                else
                {
                    searchkey.erase(0, 1);
                }
            }
            else
            {
                if (searchkey.size() <= numstr.size() && searchkey == numstr.substr(0, searchkey.size()))
                {
                    hilight += cu.oriStr.substr(0, searchkey.size());
                    return true;
                }

                if (numstr == searchkey.substr(0, numstr.size()))
                {
                    searchkey.erase(0, numstr.size());
                    hilight += cu.oriStr;
                }
            }
        }
        else
        {
            if (hasMatched)
            {
                hilight = "";
                return SearchImpl(i, cus, key, hilight);
            }
        }
    }

    if (searchkey.size() > 0)
    {
        return false;
    }

    return true;
}

bool T9Search::FirstCharSearchImpl(int index, const std::vector<PBCodeUnit>& cus, const std::string& key, std::string &hilight)
{
    std::string searchkey = key;
    bool hasMatched = false;

    for (size_t i = index; i < cus.size(); i++)
    {
        PBCodeUnit cu = cus[i];
        std::string numstr = Converter::ConvertString2NumberString(cu.pyStr);

        if (numstr[0] == searchkey[0])
        {
            hasMatched = true;
            if (cu.isChinese)//是否是中文
            {
                hilight += cu.oriStr;
                searchkey.erase(0, 1);
            }
            else
            {
                if (searchkey.size() <= numstr.size() && searchkey == numstr.substr(0, searchkey.size()))
                {
                    hilight += cu.oriStr.substr(0, searchkey.size());
                    return true;
                }

                if (numstr == searchkey.substr(0, numstr.size()))
                {
                    searchkey.erase(0, numstr.size());
                    hilight += cu.oriStr;
                }
            }
        }
        else
        {
            if (hasMatched)
            {
                hilight = "";
                return FirstCharSearchImpl(i, cus, key, hilight);
            }
        }
    }

    if (searchkey.size() > 0)
    {
        return false;
    }

    return true;
}

