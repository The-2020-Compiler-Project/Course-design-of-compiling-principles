//
// Created by YCJ on 2020/7/2.
//
//成员信息表定义
#ifndef COMPLIER_ELEMSHEET_H
#define COMPLIER_ELEMSHEET_H

#include <bits/stdc++.h>
#include "TypeSheet.h"
#include "..\type\cat.h"
using namespace std;

class ElemSheet {
    int sheetLen=0;//成员表所有成员的总长度
    class ElemNode{
        friend ElemSheet;
        string name;//名字
        TypeSheet::typePoint typePoint;//类型指针
        CAT cat;//语义角色
        int offSet=-1;//偏移量
        ElemNode* pre= nullptr;
        ElemNode *next= nullptr;//双向链表指针，在插入和删除时记得维护
    };
    list<ElemNode>record;
    unordered_map<string,list<ElemNode>::iterator>shine;
public:
    typedef ElemNode* ElemPoint;
    struct iterator{
        friend ElemSheet;
    private:
        ElemPoint root;
    public:
        ///迭代器相关接口
        bool useful();///判断迭代器是否为有效迭代器
        iterator operator++();
        ///成员自身信息
        int offSet();
        string name();
        TypeSheet::iterator type();
        CAT cat();
        explicit iterator(ElemPoint root= nullptr):root(root){};
    };

private:
    bool erase(ElemPoint erasePoint);//删除一项
public:
    iterator begin();
    int initOffSet(int beginOffSet);//offSet计算函数，给定初始的offSet顺序计算每个单元的offSet，返回下一个可用的offset
    bool erase(iterator eraseIterator);//删除一项
    int len();//信息表的总长度
    ElemPoint add(const string&addName,CAT addCat,TypeSheet::typePoint addType);//增加新的一项，传入名字、种类、类型
    pair<CAT,ElemPoint> find(const string &findName);//查找一个名字为name的标识符，返回安全指针
    void output(ofstream&fout);
};

#endif //COMPLIER_ELEMSHEET_H
