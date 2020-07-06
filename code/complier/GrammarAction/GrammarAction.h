//
// Created by YCJ on 2020/7/6.
//

#ifndef COMPLIER_GRAMMARACTION_H
#define COMPLIER_GRAMMARACTION_H

#include "..\type\quatName.h"
#include "..\XTable\FunSheet.h"
#include <bits/stdc++.h>
using namespace std;
//语义动作,纯函数接口
//四元式的格式(operator,object,object,object)
//空：_
class GrammarAction {
    const string begin="(";
    const string end=")";
    const string separator=",";
    const string empty="_";
    const string fileName=R"(..\Files\quat.txt)";//中间代码所在文件地址


    stack<FunSheet::iterator> funStack;//函数栈
    stack<string>object;//运算对象栈
    stack<string>ope;//运算符栈
    stack<string>funCall;//函数调用栈
    void genetQuat(string a,string b,string c,string d);//生成四元式
public:

    void pushFunStack(FunSheet::iterator pushIterator);//将函数迭代器压入函数栈
    void beginProgram();//生成函数定义四元式
    void beginFunction();//生成代码段开始四元式
    void endFunction();//函数定义结束四元式
    void Assign();//赋值四元式
    void pushObjectStack(const string&objectName);//运算对象压栈
    void getAddress();//生成获取数组地址的四元式
    void getSonAddress();//生成获取结构体元素的四元式
    void assignResult();//生成返回值赋值四元式
    void beginIf();//生成beginIf语句
    void Else();//生成Else对应的四元式
    void endIf();//生成endIf对应的四元式
    void pushOpeStack(const string&opeName);//压入算符
    void Relation();//生成关系运算四元式，关系运算加入的临时变量为bool型
    void beginWhile();//生成beginWhile四元式
    void Do();//生成Do对应的四元式
    void endWhile();//生成endWhile对应的四元式
    void popObjectStack();//运算对象弹栈
    void pushFunCallStack(const string&funName);//将函数标识符压栈
    void moveParameter();//生成参数传递四元式
    void Call();//生成函数调用四元式
    void getResult();//生成获取函数返回值的四元式
    void popFunCallStack();//函数表示符弹栈
    void Arithmetic();//生成算数运算四元式
};

#endif //COMPLIER_GRAMMARACTION_H
