#pragma once
#include<fstream>
#include<string>
#include"x86Code.h"


using std::string;
using std::ofstream;

//ջ�����������ɴ���ʱ��Ҫ����ss��ָ���ʱ��Ӧ��ʹ��������ĺ���
//��Ҫ�޸�bp��sp�������Ĵ�����������һЩѰַ����
class StackManager {
public:
	//���push����������һ��push operָ��
	void push(ofstream &outfile, string oper)
	{
		PUSH(outfile, oper);
	}

	//���pop����������һ��pop operָ��
	void pop(ofstream &outfile, string oper)
	{
		POP(outfile, oper);
	}


	//������sp����������һ��sub sp��operָ��
	void subSp(ofstream &outfile, string oper)
	{
		SUB(outfile, "sp", oper);
	}

	//�������sp����������һ��add sp��operָ��
	void addbSp(ofstream &outfile, string oper)
	{
		ADD(outfile, "sp", oper);
	}

	//������bp����������һ��sub bp��operָ��
	void subBp(ofstream &outfile, string oper)
	{
		SUB(outfile, "bp", oper);
	}

	//�������bp����������һ��add bp��operָ��
	void addBp(ofstream &outfile, string oper)
	{
		ADD(outfile, "bp", oper);
	}

	//����bp����sp���sp����bp����������һ��mov bp(sp),sp(bp)ָ��
	void movp(ofstream &outfile, string dest, string oper)
	{
		MOV(outfile, dest, oper);
	}

};

//nmd,��Щ����ָ��...