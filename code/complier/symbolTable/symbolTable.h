//
// Created by YCJ on 2020/6/30.
//

#ifndef COMPLIER_SYMBOLTABLE_H
#define COMPLIER_SYMBOLTABLE_H

#include "..\tokenType\tokenType.h"
#include <bits/stdc++.h>

using namespace std;
class symbolTable {
public:
    //迭代器定义
    struct iterator{

    };
public:
    //插入函数
    int insertIt(string name);
    int insertCt(char c);
    int insertSt(string s);
    int insertNt(int num);
    int insertRt(double num);
    //按名字查找函数
    iterator findIt(string name);
    int findKt(string name);
    int findPt(string name);
    int findCt(char c);
    int findSt(string s);
    int findNt(int num);
    int findRt(double num);
    //按照tokenType和index查找名字
    string search(tokenType type,int index);
    //按index查找符号表iterator,提供给语义分析的接口
    iterator search(int index);

}table;//全局符号表



#endif //COMPLIER_SYMBOLTABLE_H
