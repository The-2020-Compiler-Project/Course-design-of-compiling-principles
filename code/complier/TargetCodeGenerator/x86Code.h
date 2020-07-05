#pragma once
#include<string>
#include"type.h"

using std::string;
using std::endl;

const string blank = "	       ";

//生成指令的操作在这里定义，注意这里只有生成汇编指令的操作，别的啥都没有

//目标代码
struct code {
	//标号，操作符，目标操作数，源操作数
	string name, oper, dest, source;
};
 
//移动指令，o1和dest不能同时为存储单元
inline void MOV(std::vector<code> &targetCodeArea, string name, string dest, string o1)
{
	code nowCode;
	nowCode.name = name, nowCode.oper = "mov";
	nowCode.dest = dest, nowCode.source = o1;
	targetCodeArea.push_back(nowCode);
}

//加指令，o1和dest不能同时为存储单元,add dest,o1
inline void ADD(std::vector<code> &targetCodeArea, string name, string dest, string o1)
{
	code nowCode;
	nowCode.name = name, nowCode.oper = "add";
	nowCode.dest = dest, nowCode.source = o1;
	targetCodeArea.push_back(nowCode);
}

//减指令，o1和dest不能同时为存储单元,sub dest,o1
inline void SUB(std::vector<code> &targetCodeArea, string name, string dest, string o1)
{
	code nowCode;
	nowCode.name = name, nowCode.oper = "sub";
	nowCode.dest = dest, nowCode.source = o1;
	targetCodeArea.push_back(nowCode);
}

//乘指令，单操作数，操作数8位时与al相乘，放在ax中。16位时与ax相乘，结果高位放在dx中，低位放在ax中
inline void IMUL(std::vector<code> &targetCodeArea, string name, string o1)
{
	code nowCode;
	nowCode.name = name, nowCode.oper = "imul";
	nowCode.dest = "", nowCode.source = o1;
	targetCodeArea.push_back(nowCode);
}

//除指令，单操作数，操作数8位时ax/他，放在al中。16位时dx，ax/他，放在ax中
inline void IDIV(std::vector<code> &targetCodeArea, string name, string o1)
{
	code nowCode;
	nowCode.name = name, nowCode.oper = "idiv";
	nowCode.dest = "", nowCode.source = o1;
	targetCodeArea.push_back(nowCode);
}

//push指令，单操作数
inline void PUSH(std::vector<code> &targetCodeArea, string name, string o1)
{
	code nowCode;
	nowCode.name = name, nowCode.oper = "push";
	nowCode.dest = "", nowCode.source = o1;
	targetCodeArea.push_back(nowCode);
}

//pop指令，单操作数
inline void POP(std::vector<code> &targetCodeArea, string name, string o1)
{
	code nowCode;
	nowCode.name = name, nowCode.oper = "pop";
	nowCode.dest = "", nowCode.source = o1;
	targetCodeArea.push_back(nowCode);
}

//XOR指令
inline void XOR(std::vector<code> &targetCodeArea, string name, string dest, string o1)
{
	code nowCode;
	nowCode.name = name, nowCode.oper = "xor";
	nowCode.dest = dest, nowCode.source = o1;
	targetCodeArea.push_back(nowCode);
}

//CMP指令
inline void CMP(std::vector<code> &targetCodeArea, string name, string dest, string o1)
{
	code nowCode;
	nowCode.name = name, nowCode.oper = "cmp";
	nowCode.dest = dest, nowCode.source = o1;
	targetCodeArea.push_back(nowCode);
}

//条件跳转指令
inline void relJmp(std::vector<code> &targetCodeArea, string name, string relJmp, string o1)
{
	code nowCode;
	nowCode.name = name, nowCode.oper = relJmp;
	nowCode.dest = "", nowCode.source = o1;
	targetCodeArea.push_back(nowCode);
}

//nmd,这些阴间指令...
//a.h中声明函数，如果c.h想用，必须再a.h的声明中加上inline
//太他妈阴间了
