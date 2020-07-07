//
// Created by YCJ on 2020/6/30.
//

#ifndef COMPLIER_STABLE_H
#define COMPLIER_STABLE_H

#include "..\tokenType\tokenType.h"
#include "Sheet.h"
#include <bits/stdc++.h>

//环境配置
#define CLION_ENVIROMENT
#ifdef CLION_ENVIROMENT
//clion 环境
static const string ktFile=R"(..\STable\KTtable.txt)";
static const string ptFile=R"(..\STable\PTtable.txt)";
#else
//VS 环境配置
static const string ktFile=R"(..\STable\KTtable.txt)";
static const string ptFile=R"(..\STable\PTtable.txt)";
#endif

using namespace std;
/*简单符号表定义*/
class STable {
    const ull Lprime = 769, Sprime = 128, Iprime = 17;
    //存储关键字、界符、标识符、字符常量、字符串常量
    Sheet ktSheet, ptSheet, itSheet, ctSheet, stSheet;
    //存储整型常量、浮点型常量
    map<int, int> ntSheet;//value-index
    vector<int> ntRSheet;//index-value
    map<double, int> rtSheet;//value-index
    vector<double> rtRSheet;//index-value
private:
    //初始化，加载关键字表和界符表
    void init(const string& ktName=ktFile,const string& ptName=ptFile);
public:
    /*插入系列函数*/
    //标识符插入
    int insertIt(const string& name);

    //字符常const 量插入
    int insertCt(const string& name);

    //字符串常const 量的插入&
    int insertSt(const  string& name);

    //整型常量的插入
    int insertNt(int num);

    //浮点型常量的插入
    int inserRt(double num);

    /*按名查询系列函数*/
    int findIt(const string& name);

    int findKt(const string& name);

    int findPt(const string& name);

    int findCt(const string& c);

    int findSt(const string& s);

    int findNt(int num);

    int findRt(double num);

    /*按位置查询系列函数*/
    string searchKt(int index);

    string searchPt(int index);

    string searchIt(int index);

    string searchNt(int index);

    string searchCt(int index);

    STable() : ktSheet(Lprime), ptSheet(Sprime), itSheet(Lprime), ctSheet(Sprime), stSheet(Lprime) {
        init();
    }
};
extern STable sTable;//全局变量

#endif //COMPLIER_STABLE_H
