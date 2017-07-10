#ifndef T9SEARCHIMPL_HPP_
#define T9SEARCHIMPL_HPP_
#include<string>
#include<vector>
#include"../PBCodeUnit.hpp"

class T9Search
{
public:
    static bool Search(const std::string& name, const std::string& key, std::string& hilight);

private:
    static bool SearchImpl(int index, const std::vector<PBCodeUnit>& cus, const std::string& key, std::string &hilight);

    static bool FirstCharSearchImpl(int index, const std::vector<PBCodeUnit>& cus, const std::string& key, std::string &hilight);
};

#endif
