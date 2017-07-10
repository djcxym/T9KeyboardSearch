#include<iostream>
#include<string>
#include"T9Search/T9Search.hpp"

using namespace std;

int main()
{
    const vector<string> names = { u8"成都",u8"重庆",u8"成都重庆"};

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