#ifndef CONVERTUTILS_HPP_
#define CONVERTUTILS_HPP_
#include<string>

class Converter
{
public:
    /*Convert a string to number string according to T9 keyboard*/
    static const std::string ConvertString2NumberString(const std::string& str);

    /*Convert a Utf8 Chinese character to pinyin*/
    static const std::string ConvertKanji2Pinyin(const std::string& aChineseChar);

private:
    /*Convert a Utf8 Chinese character to Unicode number*/
    static int ConvertUtf8ToUnicode(const std::string& aChineseChar);
};

#endif