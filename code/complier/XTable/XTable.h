//
// Created by YCJ on 2020/6/30.
//

#ifndef COMPLIER_XTABLE_H
#define COMPLIER_XTABLE_H

#include "FunSheet.h"
#include "TypeSheet.h"

class XTable {
private:

public:
    /*迭代器end系列函数*/
    TypeSheet::const_iterator typeEnd();

    /*find系列函数,仅在局部查找，用于重定义检查、信息修改*/
    TypeSheet::const_iterator findType(const string& name);//查询const 一个类型&的信息
    void findFun(string name);//查询一个函数的信息
    void findElm(string fName,string eName);//查询变量/常量/参数的信息
    /*search系列函数，按照最近嵌套规则进行查找，用于查找信息*/
    TypeSheet::const_iterator searchType(string name);//查询一个类型的信息
    void searchFun(string name);//查询一个函数的信息
    void searchElt(string fName,string eName);//查询变量/常量/参数的信息
    /*insert系列函数，用于插入新的标识符*/
    void insertFun(string name);//插入函数
    void insertElm(string fName,string eName);//插入变量/常量/参数
    /*delet系列函数，用于删除标识符*/
    void deletElm(string fName,string eName);//删除变量/常量/参数
};


#endif //COMPLIER_XTABLE_H
