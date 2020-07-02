//
// Created by Lenovo on 2020/7/2.
//

#ifndef COMPLIER_TMPSHEET_H
#define COMPLIER_TMPSHEET_H

#include "TypeSheet.h"
#include <bits/stdc++.h>
using namespace std;

class TmpSheet {
    struct TmpNode{
        string name;
        TypeSheet iterator;
    };
    map<string,TmpNode>record;
public:
    bool find(const string& name);//查询是否存同名临时变量
};


#endif //COMPLIER_TMPSHEET_H
