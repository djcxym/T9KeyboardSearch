#include<iostream>
#include<string>
#include"T9Search/T9Search.hpp"

using namespace std;

int main()
{
    const vector<string> names = { u8"鲁鲁庭勇",u8"连长",u8"鲁庭勇", u8"邓佳川",u8"陈新杰",u8"邓邓佳川", u8"邓佳川鲁庭勇",
                                   u8"华为客服", u8"胡H杨Y", u8"胡HY杨", u8"GU邓佳SHU川",u8"邓佳SHU川",u8"胡杨"};

    string key;
    cout << "Input to search: ";
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