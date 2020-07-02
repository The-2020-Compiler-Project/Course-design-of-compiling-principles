//
// Created by YCJ on 2020/6/30.
//

#include "FunSheet.h"

FunSheet::FunSheet() {
    root = &head;
}

FunSheet::iterator FunSheet::end() {
    return nullptr;
}

bool FunSheet::find(const string &name) {
    //查询当前函数是否同名
    if(root->name==name)return true;
    //查询子函数表中是否定义
    if(root->sFunTable.find(name)!=root->sFunTable.end())return true;
    //查询是否在成员信息表中定义
    if(root->elemSheet.find(name))return true;
    //查询是否在临时变量中定义，在定义阶段查找没意义，但为了保险起见
    return root->tmpSheet.find(name);
}

FunSheet::funIterator FunSheet::insertFun(const string& name) {
    funNode tmp = funNode();
    root->sFunTable[tmp.name]=tmp;
    funIterator ans(tmp.name,tmp.level);
    return ans;
}

FunSheet::funIterator::funIterator(const string &name, int level) :name(name),level(level){}
