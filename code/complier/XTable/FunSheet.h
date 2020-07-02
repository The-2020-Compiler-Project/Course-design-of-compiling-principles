//
// Created by YCJ on 2020/6/30.
//

#ifndef COMPLIER_FUNSHEET_H
#define COMPLIER_FUNSHEET_H

#include "TypeSheet.h"
#include "..\type\cat.h"
#include "ElemSheet.h"
#include "TmpSheet.h"
#include <bits/stdc++.h>

using namespace std;
typedef unsigned long long ull;


class FunSheet {
    const CAT cat = CAT::f;
    TypeSheet typeSheet;
    struct funNode {
        string name;
        int level;
        TypeSheet::const_iterator typeIterator;
        int offset;
        int len;
        int fn;
        ElemSheet elemSheet;
        TmpSheet tmpSheet;
        string addr;
        map<string, funNode> sFunTable;
        FunSheet *pFunPoint;
    };

    funNode head;
    funNode *root;
public:
    struct funIterator{
        const string&name;
        const int&level;
        funIterator(const string& name,int level);
    };
public:
    int test(){
        ArrSheet a;
    }
    typedef funNode* iterator;
    iterator end();
    bool find(const string& name);
    funIterator insertFun(const string& name);
    FunSheet();

};


#endif //COMPLIER_FUNSHEET_H
