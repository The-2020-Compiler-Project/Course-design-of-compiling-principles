#include"TargetCodeGenerator.h"
#include<iostream>
#include<algorithm>


using std::ios;
using std::cout;
using std::endl; 


//从文件中读取四元式
void TargetCodeGenerator::read(string filename)
{
	ifstream infile;
	infile.open("../quar.txt", ios::in);
	if (!infile.is_open())
	{
		cout << "Open Failed!!!" << endl;
		exit(0);
	}

	string line, th[4];
	while (!infile.eof())
	{
		std::getline(infile, line);

		//book为开始复制的位置，turn为到了哪个操作数（0，1，2，3）
		int book = 1, turn = 0;	

		for (int i = 1; i < line.size() - 1; i++)
		{
			if (line[i] == ',')
			{
				th[turn] = "";
				for (int j = book; j < i; j++)
				{
					th[turn] += line[j];
				}
				turn++;
				book = i + 1;
			}
		}

		//因为到了最后一个逗号就停下了，所以要把最后的目标操作数再处理一下
		th[turn] = "";
		for (int i = book; i < line.size() - 1; i++)
		{
			th[turn] += line[i];
		}

		//存入所有四元式集中
		quar tq;
		tq.oper = th[0], tq.o1 = th[1], tq.o2 = th[2], tq.target = th[3];
		AllQuarColl.push_back(tq);
	}
	int a = 1;
}

//是否是跳转符，目前有if，else，while，we
//是则返回true，否则返回false
bool TargetCodeGenerator::isSpilt(quar nowQuar)
{
	for (int i = 0; i < SpiltQuarSize; i++)
	{
		if (nowQuar.oper == SpiltQuar[i])
		{
			return true;
		}
	}
	return false;
}

//分割四元式为基本块，存入基本块集中
void TargetCodeGenerator::spiltMidCode()
{
	quar nowQuar;
	set<int> BeginQuarColl;		//基本块入口指令编号集，从0开始
	BeginQuarColl.insert(0);	//第一个四元式加入
	
	//遍历所有四元式
	for (int i = 1; i < AllQuarColl.size(); i++)
	{

		//如果上一条是跳转语句，则将此条加入
		if (isSpilt(AllQuarColl[i - 1]))
		{
			BeginQuarColl.insert(i);
		}
		//特判，while开始四元式也是开始四元式
		if (AllQuarColl[i].oper == "beginWhile")
		{
			BeginQuarColl.insert(i);
		}
	}

	//当前基本块
	BaseBlock nowBlock;

	//遍历每一个编号，以当前入口语句开始，下一个入口语句结束（不包含），开始复制
	for (auto g = BeginQuarColl.begin(); g != prev(BeginQuarColl.end()); g++)
	{
		nowBlock.BaseBlockQuar.clear();

		auto next = g;
		int nowBegin = *g;				//开始
		int nowEnd = *(++next);			//结束

		//基本块开始和结束的位置编号
		nowBlock.BaseBlockStart = nowBegin;
		nowBlock.BaseBlockEnd = nowEnd - 1;
		
		//开始复制
		for (int i = nowBegin; i < nowEnd; i++)
		{
			nowBlock.BaseBlockQuar.push_back(AllQuarColl[i]);
		}

		//加入基本块集
		BaseBlockColl.push_back(nowBlock);
	}

	//剩下最后一个开始语句直到所有四元式结束，所以补上
	nowBlock.BaseBlockQuar.clear();
	int nowBegin = *prev(BeginQuarColl.end());
	int nowEnd = AllQuarColl.size();

	nowBlock.BaseBlockStart = nowBegin;
	nowBlock.BaseBlockEnd = nowEnd - 1;

	for (int i = nowBegin; i < nowEnd; i++)
	{
		nowBlock.BaseBlockQuar.push_back(AllQuarColl[i]);
	}
	BaseBlockColl.push_back(nowBlock);

	int a = 1;
}


//测试用，实际应该使用符号表的函数
bool isTemporary(string nowString)
{
	if (nowString[0] == 't')
		return true;
	return false;
}

//判断操作符是否是函数调用，函数声明，主程序声明，过程声明，函数结束，主程序结束，过程结束
int BaseBlock::isHaveActivity(string nowOp)
{
	for (int i = 0; i < notAcitvityCount; i++)
	{
		if (nowOp == notAcitvity[i])
			return 1;
	}
	return 0;
}

//判断操作数是否是空格
int BaseBlock::isBlankOper(string nowOper)
{
	if (nowOper == "_")
		return 1;
	return 0;
}

//初始化基本块活跃信息，按照基本块中四元式数量生成对应数量的活跃信息结构体
//临时变量初始化为no，其他初始化为yes
void BaseBlock::initBlockActive()
{
	for (int i = 0; i < BaseBlockQuar.size(); i++)
	{
		//当前四元式的活跃信息，算符没啥影响，所以直接复制
		active nowActivity;
		nowActivity.oper = BaseBlockQuar[i].oper;
		
		//查看是否是临时变量
		string checkTemp = "";

		//查看是否是空的
		if (!isBlankOper(BaseBlockQuar[i].o1))		//非空，继续
		{
			checkTemp = BaseBlockQuar[i].o1;
			if (isTemporary(checkTemp))				//查看是否是临时变量
			{
				nowActivity.o1 = "no";				//临时变量为no
				tempVariable[checkTemp] = "no";
			}
			else
			{
				nowActivity.o1 = "yes";				//否则为yes
				notTempVariable[checkTemp] = "yes";
			}
		}

		//第二个操作数，同上
		if (!isBlankOper(BaseBlockQuar[i].o2))
		{
			checkTemp = BaseBlockQuar[i].o2;
			if (isTemporary(checkTemp))
			{
				nowActivity.o2 = "no";
				tempVariable[checkTemp] = "no";
			}
			else
			{
				nowActivity.o2 = "yes";
				notTempVariable[checkTemp] = "yes";
			}
		}

		//目标操作数，同上
		if (!isBlankOper(BaseBlockQuar[i].target))
		{
			checkTemp = BaseBlockQuar[i].target;
			if (isTemporary(checkTemp))
			{
				nowActivity.target = "no";
				tempVariable[checkTemp] = "no";
			}
			else
			{
				nowActivity.target = "yes";
				notTempVariable[checkTemp] = "yes";
			}
		}

		BaseBlockActivity.push_back(nowActivity);	//存储
		int a = 1;
	}
	
}

//计算活跃信息，用的上课ppt的方法
void BaseBlock::checkBlockActive()
{
	//首先初始化当前基本块的活跃信息
	initBlockActive();
	
	int a = 1;
	//开始填活跃信息表，倒着来
	for (int i = BaseBlockActivity.size() - 1; i >= 0; i--)
	{
		if (isHaveActivity(BaseBlockQuar[i].oper))
			continue;

		string nowVariable = "";
		string changeMessage = "";
		
		if (!isBlankOper(BaseBlockQuar[i].target))
		{
			nowVariable = BaseBlockQuar[i].target;		//当前操作数
			if (isTemporary(nowVariable))						//如果是临时变量的操作
			{
				changeMessage = tempVariable[nowVariable];		
				BaseBlockActivity[i].target = changeMessage;	//从表中找到值，赋给活跃信息表
				tempVariable[nowVariable] = "no";				//目标操作数变为no
			}
			else												//如果不是临时变量的操作
			{
				changeMessage = notTempVariable[nowVariable];
				BaseBlockActivity[i].target = changeMessage;
				notTempVariable[nowVariable] = "no";
			}
		}

		if (!isBlankOper(BaseBlockQuar[i].o2))
		{
			nowVariable = BaseBlockQuar[i].o2;
			if (isTemporary(nowVariable))
			{
				changeMessage = tempVariable[nowVariable];		
				BaseBlockActivity[i].o2 = changeMessage;		//从表中找到值，赋给活跃信息表
				tempVariable[nowVariable] = "yes";				//操作数2变为yes
			}
			else
			{
				changeMessage = notTempVariable[nowVariable];
				BaseBlockActivity[i].o2 = changeMessage;
				notTempVariable[nowVariable] = "yes";
			}
		}

		if (!isBlankOper(BaseBlockQuar[i].o1))
		{
			nowVariable = BaseBlockQuar[i].o1;
			if (isTemporary(nowVariable))
			{
				changeMessage = tempVariable[nowVariable];
				BaseBlockActivity[i].o1 = changeMessage;		//从表中找到值，赋给活跃信息表
				tempVariable[nowVariable] = "yes";				//操作数1变为yes
			}
			else
			{
				changeMessage = notTempVariable[nowVariable];
				BaseBlockActivity[i].o1 = changeMessage;
				notTempVariable[nowVariable] = "yes";
			}
		}
		int a = 1;
	}
}

//查找变量相对于变量存储开始位置的偏移量，测试用，实际使用需要符号表的函数
//查找后要继续处理（加上old sp什么的）
int findTempOffset(string tempName)
{
	//变量相对于存储数据区的偏移量，指向高位
	if (tempName == "a")
		return 0;
	if (tempName == "b")
		return 2;
	if (tempName == "c")
		return 4;
	if (tempName == "t1")
		return 6;
	if (tempName == "t2")
		return 8;
	if (tempName == "t3")
		return 10;
	if (tempName == "t4")
		return 12;
}

//查看变量的层次，从0开始，测试用
int findTempLevel(string tempName)
{
	return 0;
}

//计算变量存储开始单元相对于old sp存储位置的偏移量，这个函数是我应该实现的
int TargetCodeGenerator::findBpOffset(string tempName)
{
	//查看层次，暂时使用测试函数
	int tempLevel = findTempLevel(tempName);

	//old sp(2)+返回值(2)+display((层次+1)*2)
	//由于变量偏移位置给出时也是从零开始，所以一开始可以直接加2
	int spOffset = 2 + 2 + 2 * (tempLevel + 1);
	return spOffset;
}

//计算所求变量相对于所在层sp的偏移量，这个函数是我应该实现的
int TargetCodeGenerator::findTrueOffset(string tempName)
{
	int trueOffset = findBpOffset(tempName) + findTempOffset(tempName);
	return trueOffset;
}

//生成数据段和堆栈段,初始化,在准备生成代码时调用
void TargetCodeGenerator::initAsm()
{
	//生成数据段因为所有数据都存在堆栈段，所以数据段为空
	asmout << "dseg segment" << endl;
	asmout << "dseg ends" << endl << endl;

	//生成数据段
	asmout << "sseg segment	stack" << endl;
	asmout << "	        dw 512 dup(?)" << endl;
	asmout << "sseg ends" << endl << endl;

	//开始程序段，初始化数据段，堆栈段，程序段
	asmout << "cseg segment" << endl;
	asmout << "	        assume cs:cseg,ds:dseg" << endl;
	asmout << "	assume ss:sseg" << endl;
	asmout << "start:	mov ax,sseg" << endl;	//此处试验段更换
	asmout << "	        mov ds,ax" << endl;
	asmout << "	        mov ax,sseg" << endl;
	asmout << "	        mov ss,ax" << endl << endl;

}

//获得数据存储区长度，测试用
int funDataLen()
{
	return 14;
}

int funLevel()
{
	return 0;
}

//处理程序开始四元式
void TargetCodeGenerator::programBegin(quar nowQuar)
{
	//这个函数中应该通过符号表的接口，给符号表发送programbegin命令
	//这个可以先放放，等到讨论的时候问问

	//开始分配程序的堆栈空间
	//首先将bp存入，为old sp
	asmout << "	        ";			//为了汇编语句对齐
	stackManager.push(asmout, "bp");

	//将bp移到sp的位置
	asmout << "	        ";			//为了汇编语句对齐
	stackManager.movp(asmout, "bp", "sp");

	//确定需要的存储单元，查表确定数据存储区的长度后计算，old sp已经加入了，就不用算了
	//(返回值(2)+display((当前函数层次数+1)*2)+数据存储区长度（不用乘，可以直接得到存储单元数))
	int dataLenth = funDataLen();
	int nowLevel = funLevel();
	int storeLenth = 2 + (nowLevel + 1) * 2 + dataLenth;
	
	//通过减小sp来开辟存储空间
	asmout << "	        ";			//为了汇编语句对齐
	stackManager.subSp(asmout, to_string(storeLenth));

	//计算display表，之前已经获取过当前层次了，直接用就行了
	//因为是程序开始，所以直接2个存储单元赋值就行了
	asmout << "	        ";			//为了汇编语句对齐
	MOV(asmout, "[bp+5]", "bp");

}

//处理赋值
void TargetCodeGenerator::assignCalculation(quar nowQuar)
{
	string o1 = nowQuar.o1;

	//判断操作数1是否是数字
	int isNum = 1;
	for (int i = 0; i < o1.size(); i++)
	{
		if (o1[i]<'0' || o1[i]>'9')
		{
			isNum = 0;
			break;
		}
	}

	if (isNum)		//如果是数字
	{
		string o2 = nowQuar.o2;

	}
}

//处理加减乘除
void TargetCodeGenerator::numCalculation(quar nowQuar)
{
	string nowOper = nowQuar.oper;

	if (nowOper == "ADD")
	{

	}
}

//生成目标代码
void TargetCodeGenerator::generateCode()
{
	for (int i = 0; i < BaseBlockColl.size(); i++)
	{
		BaseBlockColl[i].checkBlockActive();
	}
	int a = 1;

	//打开目标文件
	asmout.open("../target.asm", ios::out);
	if (!asmout.is_open())
	{
		cout << "Open Failed!!!" << endl;
		exit(0);
	}

	//初始化数据段，堆栈段
	initAsm();

	//遍历所有基本块
	for (int i = 0; i < BaseBlockColl.size(); i++)
	{
		//遍历基本块中四元式
		for (int j = 0; j < BaseBlockColl[i].BaseBlockQuar.size(); j++)
		{
			//取得当前四元式操作数
			quar nowQuar = BaseBlockColl[i].BaseBlockQuar[i];

			//操作符为程序开始时调用对应函数处理
			if (nowQuar.oper == "beginprogram")
			{
				programBegin(nowQuar);
				//测试用break，别忘了删掉
				break;
			}

			//操作符为加减乘除时调用对应函数处理
			if (nowQuar.oper == "ADD" ||nowQuar.oper == "SUB" ||
				nowQuar.oper == "MUL" ||nowQuar.oper == "DIV")
			{
				numCalculation(nowQuar);
			}
		}

		//测试用break，别忘了删掉
		break;
	}
	asmout.close();
}
