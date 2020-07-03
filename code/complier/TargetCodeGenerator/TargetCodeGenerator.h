#pragma once
#include<vector>
#include<map>
#include<set>
#include"StackManager.h"

using std::vector;
using std::pair;
using std::make_pair;
using std::string;
using std::to_string;
using std::set;
using std::map;
using std::ifstream;
using std::ofstream;


//四元式
struct quar {
	string oper, o1, o2, target;
};

//四元式的活跃变量
struct active {
	string oper, o1, o2, target;
};

//定义基本块，完成分割基本块，初始化变量活跃，计算活跃变量
class BaseBlock {
public:
	
	//储存基本块开始和结束位置
	int BaseBlockStart, BaseBlockEnd;

	//储存基本块内四元式，空的地方为_
	vector<quar> BaseBlockQuar;

	//储存变量活跃信息，与四元式位置对应
	vector<active> BaseBlockActivity;

	//临时变量表,用于计算活跃信息
	map<string, string> tempVariable;

	//非临时变量表，用于计算活跃信息
	map<string, string> notTempVariable;

	//判断操作符是否是函数调用，函数声明，主程序声明，过程声明，函数结束，主程序结束，过程结束
	//以上这些没有活跃信息
	int isHaveActivity(string nowOp);

	//判断操作数是否是空格
	int	isBlankOper(string nowOper);

	//初始化活跃信息，临时变量为no，用户变量为yes
	void initBlockActive();

	//计算活跃信息，用的上课ppt的方法
	void checkBlockActive();

	//判断操作符是否是函数调用，函数声明，主程序声明，过程声明，函数结束，主程序结束，过程结束
	int notAcitvityCount = 7;
	string notAcitvity[7] = { "call","beginfunction","beginprogram","beginprocedure","endfuction","endProgram","endprocedure" };

	//离当前四元式最近的操作数的活跃信息表
	map<string, string> nearActivity;
};


//目标代码生成器，生成8086汇编
//暂时先使用单寄存器，完成了之后考虑多寄存器
//可以实现+，-，*，/，:=，if，while
class TargetCodeGenerator {
public:
	//从文件中读入四元式
	void read(string filename);

	//判断是否是紧接着基本块开始的语句
	bool isSpilt(quar nowQuar);

	//分割基本块
	void spiltMidCode();

	//计算变量存储开始单元相对于变量所在层bp的偏移量，这个函数是我应该实现的
	int findBpOffset(string tempName);

	//计算所求变量相对于变量所在层bp的偏移量，这个函数是我应该实现的
	int findTrueOffset(string tempName);

	//生成数据段和堆栈段,初始化,在准备生成代码时调用
	void initAsm();

	//处理程序开始四元式
	void programBegin(quar nowQuar);

	//处理赋值
	void assignCalculation(quar nowQuar);

	//处理加减乘除
	void numCalculation(quar nowQuar);

	//处理关系运算
	void relCalculation(quar nowQuar);

	//生成目标代码
	void generateCode();

	//基本块集合
	vector<BaseBlock> BaseBlockColl;

	//四元式集合
	vector<quar> AllQuarColl;
private:
	//紧接着基本块开始的语句
	const int SpiltQuarSize = 5;
	string SpiltQuar[5] = { "do","endWhile","beginIf","else","endif"};

	//用于按照ppt上的方法使用栈来判断跳转的结果
	vector<int> SEMStack;

	//现在寄存器（ax）中变量的名字
	string nowRdlTemp;

	//文件指针，用于向文件中写入汇编代码
	ofstream asmout;

	//堆栈管理对象，通过调用函数实现堆栈管理
	StackManager stackManager;
};