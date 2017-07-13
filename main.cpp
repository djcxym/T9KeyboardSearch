#include<iostream>
#include<string>
#include"T9Search/T9Search.hpp"
#include"Converter/Converter.hpp"

using namespace std;

int main()
{
    const vector<string> names = { u8"成都",u8"重庆",u8"北京", u8"上海",u8"杭州",u8"南京", u8"哈尔滨",
                                   u8"齐齐哈尔", u8"乌鲁木齐"};

    string name = u8"成都重庆";
    cout << "All possible pinyin for " << name << ":" << endl;
    vector<vector<PBCodeUnit>> vpb = Converter::ConvertString2CodeUnits(name);
    for (size_t i = 0; i < vpb.size(); i++)
    {
        for (size_t j = 0; j < vpb[i].size(); j++)
        {
            cout << vpb[i][j].pyStr << " ";
        }
        cout << endl;
    }
    cout << endl;

    string key;
    cout << "Input T9 number to search: ";
    while (cin >> key)
    {
        for (auto name : names)
        {
            string hilight;
            if (T9Search::Search(name, key, hilight))
            {
                cout<< name << "--" << hilight << endl;
            }
        }
        cout << "--------------------------------" << endl;
        cout << "Input to search: ";
    }
}