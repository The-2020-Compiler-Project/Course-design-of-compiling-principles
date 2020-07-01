//
// Created by YCJ on 2020/6/30.
//

#ifndef COMPLIER_FUNSHEET_H
#define COMPLIER_FUNSHEET_H

#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;

class cnode {//函数信息表单元定义
public:
    string name;    //名字


    int index{};
};
class cSheet {//字符串型表格
protected:
    int len;//表单长度
    ull hPrime{};//哈希常数
    vector<cnode> record;//数据表
    ull hash(const string& s) {//计算哈希值,溢出取模
        ull ans = 0;
        ull prime = 1;
        for (char i : s) {
            ans += i * prime;
            prime *= hPrime;
        }
        return ans;
    }

public:

    virtual int insert(const string& name) = 0;

    virtual int find(const string& name) = 0;

    bool read(const string& adress) {//从文件中读取内容进行初始化
        ifstream fin(adress.c_str(), ios::in);
        string name;
        while (fin.peek() != EOF) {
            fin >> name;
            //cout<<name<<endl;
            insert(name);
        }
        fin.close();
        return false;
    }

    bool write(const string& adress) {
        ofstream fout(adress.c_str(), ios::out);
        auto it = record.begin();
        while (it != record.end()) {
            fout << it->index << "   " << it->name << endl;
            it++;
        }
        fout.close();
        return false;
    }

    cSheet() : len(0) {};
};
class Sheet :public cSheet {//长字符串表
private:
    map<ull, list<int> > shine;//映射表,hash_key->地址链表

public:
    //插入字符串，若存在同名返回-1，否则返回插入位置
    int insert(const string &name) override {
        int ansIndex = -1;
        ull hashKey = hash(name);
        auto it = shine.find(hashKey);
        if (it == shine.end()) {
            cnode tmp;
            tmp.name = name;
            tmp.index = len++;
            record.emplace_back(tmp);
            ansIndex = tmp.index;
            shine[hashKey].emplace_back(tmp.index);
        } else {
            bool flag = false;
            for (auto &p : it->second) {
                if (record[p].name == name) {
                    flag = true;
                    ansIndex = -1;
                    break;
                }
            }
            if (!flag) {
                cnode tmp;
                tmp.name = name;
                tmp.index = len++;
                record.emplace_back(tmp);
                ansIndex = tmp.index;
                it->second.emplace_back(tmp.index);
            }
        }
        return ansIndex;
    }

    int find(const string &name) override {
        int ansIndex = -1;
        auto ansIterator = record.end();
        ull hashKey = hash(name);
        auto it = shine.find(hashKey);
        if (it != shine.end()) {
            for (auto &p : it->second) {
                if (record[p].name == name) {
                    ansIndex = p;
                    break;
                }
            }
        }
        return ansIndex;
    }

    string search(const int &index) {
        string ans;
        if (index >= record.size()) {
            return ans;
        } else return record[index].name;
    }

    explicit Sheet(ull prime) { hPrime = prime; };
};
class funSheet {

};


#endif //COMPLIER_FUNSHEET_H
