#ifndef T9SEARCHHELPER_HPP_
#define T9SEARCHHELPER_HPP_
#include<string>
#include<vector>
#include"../PBCodeUnit.hpp"

class T9SearchHelper
{
public:
    T9SearchHelper(const std::string& name);
    ~T9SearchHelper();
    const std::vector<std::vector<PBCodeUnit>> SplitNameIntoCodeUnits();

private:
    int GetFirstCodeUnitOfName(const std::string& name, std::string &codeStr, bool &isChinese);
    void Combination(const std::vector<std::vector<PBCodeUnit>>& vec, size_t level, std::vector<std::vector<PBCodeUnit>>& res);

private:
    std::string mName;
    std::vector<int> indexes;
};

#endif
