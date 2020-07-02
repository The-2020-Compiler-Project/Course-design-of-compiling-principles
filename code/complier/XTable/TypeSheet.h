//
// 类型信息表 Created by YCJ on 2020/7/1.
//

#ifndef COMPLIER_TYPESHEET_H
#define COMPLIER_TYPESHEET_H

#include "ArrSheet.h"
#include "..\type\type.h"
#include <bits/stdc++.h>

using namespace std;

class ArrSheet {
    int low,up;
    TypeSheet* sIterator;
    int len;
    void setSonType(string name){

    }
};

class TypeSheet {
private:
    typedef unsigned long long ull;
    const ull hPrime=769;//哈希常数
    struct typeNode {
        TYPEVAL tVal;
        string name;
        int len;
        ArrSheet* arrSheet;

    };
    vector<typeNode> record;
public:
    struct iterator {
        const TYPEVAL &tVal;
        const string &name;
        const int &len;

    };
    typedef vector<typeNode>::const_iterator const_iterator;

private:
    ull hash(const string& s);
public:
    const_iterator find(const string& name);//给定类型名，返回const迭代器，仅用于查看信息
    const_iterator end();
    TypeSheet();
};



#endif //COMPLIER_TYPESHEET_H
