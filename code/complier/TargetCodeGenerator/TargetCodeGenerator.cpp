#include"TargetCodeGenerator.h"
#include<iostream>
#include<algorithm>


using std::ios;
using std::cout;
using std::endl;


//测试用，实际应该使用符号表的函数
bool isTemporary(string nowString)
{
	if (nowString[0] == 't')
		return true;
	return false;
}

//查找变量相对于变量存储开始位置的偏移量，测试用，实际使用需要符号表的函数
//查找后要继续处理（加上old sp什么的）
int findTempOffset(string tempName)
{
	//变量相对于存储数据区的偏移量，指向低位
	/*
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
	*/
	if (tempName == "a")
		return 1;
	if (tempName == "c")
		return 3;
}

//获得数据存储区长度，测试用
int funDataLen()
{
	return 4;
}

//获得当前函数层次，测试用
int funLevel()
{
	return 0;
}

//查看变量的层次，从0开始，测试用
int findTempLevel(string tempName)
{
	return 0;
}

//查看变量的类型，测试用
string findTempType(string tempName)
{
	if (tempName == "a")
		return typeName[0];
	if (tempName == "c")
		return typeName[0];
}


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





//计算变量存储开始单元相对于old sp存储位置(使用oldsp低位的位置)的偏移量，这个函数是我应该实现的
int TargetCodeGenerator::findBpOffset(string tempName)
{
	//查看层次，暂时使用测试函数
	int tempLevel = findTempLevel(tempName);

	//返回值(2)+display((层次+1)*2)
	//由于变量偏移位置给出时也是从零开始，所以一开始可以直接加2
	//应该加1，因为计算后指向display表的最后的低位，再加1才是存储开始位置
	int spOffset = 2 + 2 * (tempLevel + 1) + 1;
	return spOffset;
}

//计算所求变量相对于所在层sp的偏移量，这个函数是我应该实现的
int TargetCodeGenerator::findTrueOffset(string tempName)
{
	//在这时我将会把偏移量转化为指向低位的
	int trueOffset = findBpOffset(tempName) + findTempOffset(tempName);
	return trueOffset;
}

//生成数据段和堆栈段,初始化,在准备生成代码时调用
void TargetCodeGenerator::initAsm()
{
	code nowCode;

	//生成数据段因为所有数据都存在堆栈段，所以数据段为空
	nowCode.name = "dseg", nowCode.oper = "segment";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
	

	nowCode.name = "dseg", nowCode.oper = "ends";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
	

	//生成数据段
	nowCode.name = "sseg", nowCode.oper = "segment", nowCode.dest = "stack";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
	

	nowCode.name = blank, nowCode.oper = "dw", nowCode.dest = "512", nowCode.source = "dup(?)";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
	

	nowCode.name = "sseg", nowCode.oper = "ends";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
	

	//开始程序段，初始化数据段，堆栈段，程序段
	nowCode.name = "cseg", nowCode.oper = "segment";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
	

	nowCode.name = blank, nowCode.oper = "assume",nowCode.dest="cs:cseg,",nowCode.source="ds:dseg";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
	

	nowCode.name = "	assume", nowCode.oper = "ss:sseg";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
	

	nowCode.name = "start:", nowCode.oper = "mov",nowCode.dest="ax,",nowCode.source="sseg";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
	

	nowCode.name = blank, nowCode.oper = "mov", nowCode.dest = "ds,", nowCode.source = "ax";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
	

	nowCode.name = blank, nowCode.oper = "mov", nowCode.dest = "ax,", nowCode.source = "sseg";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
	

	nowCode.name = blank, nowCode.oper = "mov", nowCode.dest = "ss,", nowCode.source = "ax";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

}



//处理程序开始四元式
void TargetCodeGenerator::programBegin(quar nowQuar)
{
	//这个函数中应该通过符号表的接口，给符号表发送programbegin命令
	//这个可以先放放，等到讨论的时候问问

	//开始分配程序的堆栈空间
	//首先将bp存入，为old sp
	stackManager.push(targetCodeArea, blank, "bp");
	//mov bp，sp
	stackManager.movp(targetCodeArea, blank, "bp,", "sp");

	//确定需要的存储单元，查表确定数据存储区的长度后计算，old sp已经加入了，就不用算了
	//现在bp指向old sp的低位，通过[bp]可以取出old sp
	//(返回值(2)+display((当前函数层次数+1)*2)+数据存储区长度（不用乘，可以直接得到存储单元数))
	int dataLenth = funDataLen();
	int nowLevel = funLevel();
	int storeLenth = 2 + (nowLevel + 1) * 2 + dataLenth;
	
	//通过减小sp来开辟存储空间
	stackManager.subSp(targetCodeArea, blank, to_string(storeLenth));

	//计算display表，之前已经获取过当前层次了，直接用就行了
	//因为是程序开始，所以直接2个存储单元赋值就行了
	MOV(targetCodeArea, blank, "word ptr [bp-4],", "bp");

}

//在display表中查找位置，输入变量层次，返回指向低位的偏移量
int TargetCodeGenerator::findInDisplay(int Level)
{
	//返回值(2)+(Level+1)*2
	int place = 2 + (Level + 1) * 2;
	return place;
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

	//开始进行LD过程
	string o1Code = "";	//先计算，之后统一生成指令
	if (isNum)		//如果是数字,则只需将其放到寄存器中
	{
		//计算源操作数形式
		o1Code = o1;
		//MOV(targetCodeArea, blank, "ax,", o1);
		//更改nowRdlTemp的值
		nowRdlTemp = "const";	//const代表常值
	}
	else
	{
		//计算真实偏移量,原因查看测试ifwhile文档
		int trueOffset = findTrueOffset(o1);

		//查display表确定这个变量所在的活动记录开始位置
		//表的前缀可以计算得出，所有的东西计算后变为汇编代码即可
		//查看目标操作数的层次
		int o1Level = findTempLevel(o1);

		//在display表中查找目标位置
		int o1Dis = findInDisplay(o1Level);

		//将地址装到bx中，通过[bp-xxx]找到
		string strDis = to_string(o1Dis);

		//因为地址为2个存储单元，所以为word ptr
		strDis = "word ptr [bp-" + strDis + "]";

		//装入bx中
		MOV(targetCodeArea, blank, "bx,", strDis);

		//现在bx里面有对应的开始位置的地址
		//通过偏移量可以确定位置
		//暂时没考虑优化，没省去ppt中st指令，后续可能会加
		string strTrueOffset = to_string(trueOffset);
		//查表确定变量的类型（长度）
		string nowType = findTempType(o1);
		if (nowType == typeName[0])
		{
			strTrueOffset = "word ptr [bx-" + strTrueOffset + "]";
		}
		else if(nowType==typeName[2]||nowType==typeName[3])
		{
			strTrueOffset = "byte ptr [bx-" + strTrueOffset + "]";
		}

		//计算源操作数形式
		o1Code = strTrueOffset;
		//MOV(targetCodeArea, blank, "ax,", strTrueOffset);

		//更改nowRdlTemp的值
		nowRdlTemp = o1;	//变量名
	}

	//生成mov指令
	MOV(targetCodeArea, blank, "ax,", o1Code);

	//开始进行ST过程,target一定不是数字
	//开始寻址
	string target = nowQuar.target;

	//计算真实偏移量,原因查看测试ifwhile文档
	int trueOffset = findTrueOffset(target);
	//查看目标操作数的层次
	int targetLevel = findTempLevel(target);
	//在display表中查找目标位置
	int targetDis = findInDisplay(targetLevel);
	//将目标位置装到bx中，以[bp-xxx]的形式
	string strDis = to_string(targetDis);
	//因为地址为2个存储单元，所以为word ptr
	strDis = "word ptr [bp-" + strDis + "]";
	//装入bx中
	MOV(targetCodeArea, blank, "bx,", strDis);

	//现在bx里面有对应的开始位置的地址
	//通过偏移量可以确定位置
	//暂时没考虑优化，没省去ppt中st指令，后续可能会加
	string strTrueOffset = to_string(trueOffset);
	string nowType = findTempType(target);
	if (nowType == typeName[0])
	{
		strTrueOffset = "word ptr [bx-" + strTrueOffset + "],";
	}
	else if (nowType == typeName[2] || nowType == typeName[3])
	{
		strTrueOffset = "byte ptr [bx-" + strTrueOffset + "],";
	}
	MOV(targetCodeArea, blank, strTrueOffset, "ax");
	//更改nowRdlTemp的值
	nowRdlTemp = "0";	//清空
}

//处理加减乘除
void TargetCodeGenerator::numCalculation(quar nowQuar)
{
	string nowOper = nowQuar.oper;

	if (nowOper == "ADD")
	{

	}
}

//处理结束程序
void TargetCodeGenerator::programEnd(quar nowQuar)
{
	//生成结束中断语句等等阴间东西
	//结束中断
	code nowCode;
	nowCode.name = blank, nowCode.oper = "mov", nowCode.dest = "ah,", nowCode.source = "4ch";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
	

	nowCode.name = blank, nowCode.oper = blank, nowCode.dest = "int", nowCode.source = "21h";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
	

	nowCode.name = "cseg", nowCode.oper = "ends", nowCode.dest = "", nowCode.source = "";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
	

	nowCode.name = blank, nowCode.oper = "end", nowCode.dest = "start", nowCode.source = "";
	targetCodeArea.push_back(nowCode);
	nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
}

//生成目标代码
void TargetCodeGenerator::generateCode()
{
	for (int i = 0; i < BaseBlockColl.size(); i++)
	{
		BaseBlockColl[i].checkBlockActive();
	}

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
			quar nowQuar = BaseBlockColl[i].BaseBlockQuar[j];

			//操作符为程序开始时调用对应函数处理
			if (nowQuar.oper == "beginprogram")
			{
				programBegin(nowQuar);
				//测试用break，别忘了删掉
				//break;
			}

			//操作符为赋值时调用对应函数处理
			if (nowQuar.oper == "assign")
			{
				assignCalculation(nowQuar);
			}

			//操作符为加减乘除时调用对应函数处理
			if (nowQuar.oper == "ADD" ||nowQuar.oper == "SUB" ||
				nowQuar.oper == "MUL" ||nowQuar.oper == "DIV")
			{
				numCalculation(nowQuar);
			}

			//操作符为程序结束时调用对应函数处理
			if (nowQuar.oper == "endProgram")
			{
				programEnd(nowQuar);
			}

		}

		//测试用break，别忘了删掉
		//break;
	}
	int a = 1;
	for (int i = 0; i < targetCodeArea.size(); i++)
	{
		if (targetCodeArea[i].name.size() > 0)
			asmout << targetCodeArea[i].name;
		if (targetCodeArea[i].oper.size() > 0)
			asmout << " " << targetCodeArea[i].oper;
		if (targetCodeArea[i].dest.size() > 0)
			asmout << " " << targetCodeArea[i].dest;
		if (targetCodeArea[i].source.size() > 0)
			asmout << " " << targetCodeArea[i].source;
		asmout << endl;
	}
	asmout.close();
}
