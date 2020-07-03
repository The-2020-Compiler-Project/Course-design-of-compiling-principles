/*#include"StackManager.h"
#include"x86Code.h"


//汇编push操作，生成一条push oper指令
void StackManager::push(ofstream &outfile, string oper)
{
	PUSH(outfile, oper);
}

//汇编pop操作，生成一条pop oper指令
void StackManager::pop(ofstream &outfile, string oper)
{
	POP(outfile, oper);
}

//汇编减少sp操作，生成一条sub sp，oper指令
void StackManager::subSp(ofstream &outfile, string oper)
{
	SUB(outfile, "sp", oper);
}

//汇编增加sp操作，生成一条add sp，oper指令
void StackManager::addbSp(ofstream &outfile, string oper)
{
	ADD(outfile, "sp", oper);
}

//汇编减少bp操作，生成一条sub bp，oper指令
void StackManager::subBp(ofstream &outfile, string oper)
{
	SUB(outfile, "bp", oper);
}

//汇编增加bp操作，生成一条add bp，oper指令
void StackManager::addBp(ofstream &outfile, string oper)
{
	ADD(outfile, "bp", oper);
}

//汇编把bp赋给sp或把sp赋给bp操作，生成一条mov bp(sp),sp(bp)指令
void StackManager::movp(ofstream &outfile, string dest, string oper)
{
	MOV(outfile, dest, oper);
}*/