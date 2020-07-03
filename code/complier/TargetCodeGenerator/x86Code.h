#pragma once
#include<fstream>
#include<string>

using std::string;
using std::ofstream;
using std::endl;

//����ָ��Ĳ��������ﶨ�壬ע������ֻ�����ɻ��ָ��Ĳ��������ɶ��û��

//�ƶ�ָ�o1��dest����ͬʱΪ�洢��Ԫ
inline void MOV(ofstream &outfile, string dest, string o1)
{
	outfile << "mov " << dest << "," << o1 << endl;
}

//��ָ�o1��dest����ͬʱΪ�洢��Ԫ,add dest,o1
inline void ADD(ofstream &outfile, string dest, string o1)
{
	outfile << "add " << dest << "," << o1 << endl;
}

//��ָ�o1��dest����ͬʱΪ�洢��Ԫ,sub dest,o1
inline void SUB(ofstream &outfile, string dest, string o1)
{
	outfile << "sub " << dest << "," << o1 << endl;
}

//��ָ�����������������8λʱ��al��ˣ�����ax�С�16λʱ��ax��ˣ������λ����dx�У���λ����ax��
inline void IMUL(ofstream &outfile, string o1)
{
	outfile << "imul " << o1 << endl;
}

//��ָ�����������������8λʱax/��������al�С�16λʱdx��ax/��������ax��
inline void IDIV(ofstream &outfile, string o1)
{
	outfile << "idiv " << o1 << endl;
}

//pushָ���������
inline void PUSH(ofstream &outfile, string o1)
{
	outfile << "push " << o1 << endl;
}

//popָ���������
inline void POP(ofstream &outfile, string o1)
{
	outfile << "pop " << o1 << endl;
}

//nmd,��Щ����ָ��...
//a.h���������������c.h���ã�������a.h�������м���inline
//̫����������