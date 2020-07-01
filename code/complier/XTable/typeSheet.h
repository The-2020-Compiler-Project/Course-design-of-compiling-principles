//
// 类型信息表 Created by YCJ on 2020/7/1.
//

#ifndef COMPLIER_TYPESHEET_H
#define COMPLIER_TYPESHEET_H

#include "..\type\type.h"
#include <bits/stdc++.h>
using namespace std;
class typeSheet {
public:
    typedef unsigned long long ull;
    const ull hPrime=769;//哈希常数
    struct typeNode {
        typeVal tVal;
        string name;
        int len;
        void *tPoint;//tVal=a,tPoint指向数组；tVal=d,tPoint指向结构体
    };
    typedef vector<typeNode>::iterator iterator;
private:
    vector<typeNode> record;
    ull hash(const string& s);
public:
    iterator find(const string& name);//给定类型名查找类型信息所在位置
    iterator end();
    typeSheet();

};


#endif //COMPLIER_TYPESHEET_H
