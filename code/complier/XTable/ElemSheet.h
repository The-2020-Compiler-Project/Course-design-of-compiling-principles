//
// Created by Lenovo on 2020/7/2.
//
//成员信息表定义
#ifndef COMPLIER_ELEMSHEET_H
#define COMPLIER_ELEMSHEET_H

#include <bits/stdc++.h>
#include "TypeSheet.h"
#include "..\type\cat.h"
using namespace std;
class ElemSheet {
    struct ElemNode{
        string name;
        TypeSheet::iterator type;
        CAT cat;
        void *addr;
    };
    map<string,ElemNode>record;
public:
    bool find(const string& name);//查找一个标识符是否存在
};


#endif //COMPLIER_ELEMSHEET_H
