#pragma once
#include<fstream>
#include<string>

using std::string;
using std::ofstream;
using std::endl;

//生成指令的操作在这里定义，注意这里只有生成汇编指令的操作，别的啥都没有

//移动指令，o1和dest不能同时为存储单元
inline void MOV(ofstream &outfile, string dest, string o1)
{
	outfile << "mov " << dest << "," << o1 << endl;
}

//加指令，o1和dest不能同时为存储单元,add dest,o1
inline void ADD(ofstream &outfile, string dest, string o1)
{
	outfile << "add " << dest << "," << o1 << endl;
}

//减指令，o1和dest不能同时为存储单元,sub dest,o1
inline void SUB(ofstream &outfile, string dest, string o1)
{
	outfile << "sub " << dest << "," << o1 << endl;
}

//乘指令，单操作数，操作数8位时与al相乘，放在ax中。16位时与ax相乘，结果高位放在dx中，低位放在ax中
inline void IMUL(ofstream &outfile, string o1)
{
	outfile << "imul " << o1 << endl;
}

//除指令，单操作数，操作数8位时ax/他，放在al中。16位时dx，ax/他，放在ax中
inline void IDIV(ofstream &outfile, string o1)
{
	outfile << "idiv " << o1 << endl;
}

//push指令，单操作数
inline void PUSH(ofstream &outfile, string o1)
{
	outfile << "push " << o1 << endl;
}

//pop指令，单操作数
inline void POP(ofstream &outfile, string o1)
{
	outfile << "pop " << o1 << endl;
}

//nmd,这些阴间指令...
//a.h中声明函数，如果c.h想用，必须再a.h的声明中加上inline
//太他妈阴间了