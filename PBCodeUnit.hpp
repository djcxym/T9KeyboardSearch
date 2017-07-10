#ifndef PBCODEUNIT_HPP_
#define PBCODEUNIT_HPP_
#include<string>
/*
 struct PBCodeUnit represents a Chinese character or a alpha string
 eg:"北"=>{true, "北", "bei"}
 **/
struct PBCodeUnit
{
    bool isChinese;
    std::string oriStr;
    std::string pyStr;
};

#endif