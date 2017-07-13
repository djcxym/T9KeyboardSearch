#ifndef CONVERTUTILS_HPP_
#define CONVERTUTILS_HPP_
#include<string>
#include<vector>
#include"../PBCodeUnit.hpp"

class Converter
{
public:
    /*Convert a string to number string according to T9 keyboard*/
    static const std::string ConvertString2NumberString(const std::string& aString);

    /*Convert a Utf8 Chinese character to pinyin*/
    static const std::string ConvertKanji2Pinyin(const std::string& aChineseChar);

    /*Convert a Utf8 string to pinyin*/
    static const std::string convertMultiPinyin(const std::string& aString);

    /*Convert a string into code units*/
    static const  std::vector<std::vector<PBCodeUnit>> ConvertString2CodeUnits(const std::string& aString);

    /*Get the size in byte of a Utf8 character*/
    static int GetSizeOfUtf8Char(const std::string& aChar);

    /*Get the first kind of string. eg:"北京"=>"北", "bei京"=>"bei"*/
    static int GetFirstKindOfString(const std::string& aString, std::string &codeStr, bool &isChinese);

private:
    /*Convert a Utf8 Chinese character to Unicode number*/
    static int ConvertUtf8ToUnicode(const std::string& aChineseChar);

    /**/
    static void CombinationCodeUnits(const std::vector<std::vector<PBCodeUnit>>& vec, size_t level, std::vector<std::vector<PBCodeUnit>>& res);

private:
    static std::vector<int> indexes;
};

#endif