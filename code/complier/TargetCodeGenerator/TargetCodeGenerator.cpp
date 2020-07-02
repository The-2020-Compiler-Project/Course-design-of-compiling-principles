#include"TargetCodeGenerator.h"
#include<iostream>
#include<fstream>
#include<algorithm>


using std::ifstream;
using std::ofstream;
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
		int inparent = 0;	//是否在括号中，用于区分括号内外的逗号

		for (int i = 1; i < line.size() - 1; i++)
		{
			if (line[i] == '(')
			{
				inparent = 1;
				continue;
			}
			if (line[i] == ')')
			{
				inparent = 0;
				continue;
			}
			if (line[i] == ','&&!inparent)
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

		pair<string, string> nowPair[4];
		//处理o1，o2，target的内容，将字符串变为paie<string,string>
		for (int i = 1; i <= 3; i++)
		{
			string first="", second="";		//第一个和第二个字符串
			int isSecond = 0, pis = 1;		//是否是第二个字符串和第二个字符串开始位置
			for (int j = 1; j < th[i].size() - 1; j++)
			{
				if (th[i][j] == ',')					//第一个字符串到头了
				{
					for (int k = 1; k < j; k++)
					{
						first += th[i][k];
					}
					pis = j + 1;
					isSecond = 1;
				}
				if (isSecond && j == th[i].size() - 2)	//第二个字符串&&到头了
				{
					for (int k = pis; k <= j; k++)
					{
						second += th[i][k];
					}
				}
			}
			nowPair[i] = make_pair(first, second);
		}

		//存入所有四元式集中
		quar tq;
		tq.oper = th[0], tq.o1 = nowPair[1], tq.o2 = nowPair[2], tq.target = nowPair[3];
		AllQuarColl.push_back(tq);
	}
}

//是否是跳转符，目前有if，else，while，we
//是则返回true，否则返回false
bool TargetCodeGenerator::isJump(quar nowQuar)
{
	for (int i = 0; i < JumpQuarSize; i++)
	{
		if (nowQuar.oper == Jump[i])
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
		/*暂时作废，四元式中不会给出目标四元式

		//如果是跳转语句，则将目标四元式加入
		if (isJump(AllQuarColl[i]))
		{
			int jumpId = atof(AllQuarColl[i].target.second.c_str());	//目标编号
			BeginQuarColl.insert(jumpId);
		}
		*/

		//如果上一条是跳转语句，则将此条加入
		if (isJump(AllQuarColl[i - 1]))
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
}


//测试用，实际应该使用符号表的函数
bool isTemporary(string nowString)
{
	if (nowString[0] == 't')
		return true;
	return false;
}

//判断操作数pair是否为全空,不空返回0，左空返回1，右空返回2，全空返回3
int BaseBlock::isEmpty(pair<string, string> nowPair)
{
	int emptyCount = 0;
	if (nowPair.first == "")
		emptyCount = 1;
	if (nowPair.second == "")
	{
		if (emptyCount)
			emptyCount = 3;
		else
			emptyCount = 2;
	}
	return emptyCount;
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
		int ise = isEmpty(BaseBlockQuar[i].o1);	
		if (ise == 0 || ise == 1 || ise == 2)		//非空，继续
		{
			checkTemp = BaseBlockQuar[i].o1.second;
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

		ise = isEmpty(BaseBlockQuar[i].o2);			//第二个操作数，同上
		if (ise == 0 || ise == 1 || ise == 2)
		{
			checkTemp = BaseBlockQuar[i].o2.second;
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

		ise = isEmpty(BaseBlockQuar[i].target);		//目标操作数，同上
		if (ise == 0 || ise == 1 || ise == 2)
		{
			checkTemp = BaseBlockQuar[i].target.second;
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
	}
}

//计算活跃信息，用的上课ppt的方法
void BaseBlock::checkBlockActive()
{
	//首先初始化当前基本块的活跃信息
	initBlockActive();
	
	//开始填活跃信息表，倒着来
	for (int i = BaseBlockActivity.size() - 1; i >= 0; i--)
	{
		string nowVariable = "";
		string changeMessage = "";
		
		int ise = isEmpty(BaseBlockQuar[i].target);
		if (ise == 0 || ise == 1 || ise == 2)
		{
			nowVariable = BaseBlockQuar[i].target.second;		//当前操作数
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

		ise = isEmpty(BaseBlockQuar[i].o2);
		if (ise == 0 || ise == 1 || ise == 2)
		{
			nowVariable = BaseBlockQuar[i].o2.second;
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

		ise = isEmpty(BaseBlockQuar[i].o1);
		if (ise == 0 || ise == 1 || ise == 2)
		{
			nowVariable = BaseBlockQuar[i].o1.second;
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
}