#pragma once
#include<fstream>
#include<string>
#include"x86Code.h"


using std::string;
using std::ofstream;

//栈管理器，生成代码时需要操作ss段指针的时候应该使用这里面的函数
//主要修改bp，sp这两个寄存器，还包含一些寻址操作
class StackManager {
public:
	//汇编push操作，生成一条push oper指令
	void push(ofstream &outfile, string oper)
	{
		PUSH(outfile, oper);
	}

	//汇编pop操作，生成一条pop oper指令
	void pop(ofstream &outfile, string oper)
	{
		POP(outfile, oper);
	}


	//汇编减少sp操作，生成一条sub sp，oper指令
	void subSp(ofstream &outfile, string oper)
	{
		SUB(outfile, "sp", oper);
	}

	//汇编增加sp操作，生成一条add sp，oper指令
	void addbSp(ofstream &outfile, string oper)
	{
		ADD(outfile, "sp", oper);
	}

	//汇编减少bp操作，生成一条sub bp，oper指令
	void subBp(ofstream &outfile, string oper)
	{
		SUB(outfile, "bp", oper);
	}

	//汇编增加bp操作，生成一条add bp，oper指令
	void addBp(ofstream &outfile, string oper)
	{
		ADD(outfile, "bp", oper);
	}

	//汇编把bp赋给sp或把sp赋给bp操作，生成一条mov bp(sp),sp(bp)指令
	void movp(ofstream &outfile, string dest, string oper)
	{
		MOV(outfile, dest, oper);
	}

};

//nmd,这些阴间指令...