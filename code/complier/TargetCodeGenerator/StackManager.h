//
// Created by Nowidh on 2020/7/8.
//

#ifndef COMPLIER_STACKMANAGER_H
#define COMPLIER_STACKMANAGER_H

#include"x86Code.h"

using std::string;

//栈管理器，生成代码时需要操作ss段指针的时候应该使用这里面的函数
//主要修改bp，sp这两个寄存器，还包含一些寻址操作
class StackManager {
public:
    //汇编push操作，生成一条push oper指令
    void push(std::vector<code> &targetCodeArea, string name, string oper)
    {
        PUSH(targetCodeArea, name, oper);
    }

    //汇编pop操作，生成一条pop oper指令
    void pop(std::vector<code> &targetCodeArea, string name, string oper)
    {
        POP(targetCodeArea, name, oper);
    }


    //汇编减少sp操作，生成一条sub sp，oper指令
    void subSp(std::vector<code> &targetCodeArea, string name, string oper)
    {
        SUB(targetCodeArea, name, "sp,", oper);
    }

    //汇编增加sp操作，生成一条add sp，oper指令
    void addbSp(std::vector<code> &targetCodeArea, string name, string oper)
    {
        ADD(targetCodeArea, name, "sp,", oper);
    }

    //汇编减少bp操作，生成一条sub bp，oper指令
    void subBp(std::vector<code> &targetCodeArea, string name, string oper)
    {
        SUB(targetCodeArea, name, "bp,", oper);
    }

    //汇编增加bp操作，生成一条add bp，oper指令
    void addBp(std::vector<code> &targetCodeArea, string name, string oper)
    {
        ADD(targetCodeArea, name, "bp,", oper);
    }

    //汇编把bp赋给sp或把sp赋给bp操作，生成一条mov bp(sp),sp(bp)指令
    void movp(std::vector<code> &targetCodeArea, string name, string dest, string oper)
    {
        MOV(targetCodeArea, name, dest, oper);
    }

};

//nmd,这些阴间指令...

#endif //COMPLIER_STACKMANAGER_H
