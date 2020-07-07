#pragma once
#include<fstream>
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

	//在display表中查找位置，输入变量层次，返回指向低位的偏移量
	int findInDisplay(int Level);

	//生成数据段和堆栈段,初始化,在准备生成代码时调用
	void initAsm();

	//处理程序开始四元式
	void programBegin(quar nowQuar);

	//处理赋值
	void assignCalculation(quar nowQuar, string name);

	//处理加减
	void addCalculation(quar nowQuar, string name);

	//处理乘
	void mulCalculation(quar nowQuar, string name);

	//处理除
	void divCalculation(quar nowQuar, string name);

	//处理关系运算
	void relCalculation(quar nowQuar, string name);

	//处理if跳转(假跳)
	void ifCalculation(quar nowQuar, string name);

	//处理else跳转(无条件)
	void elCalculation(quar nowQuar, string name);

	//处理endIf(无动作，NOP,为了嵌套才搞了这个)
	void endIfCalculation(quar nowQuar, string name);

	//处理wh语句
	void whCalculation(quar nowQuar, string name);

	//处理do跳转(假跳)
	void doCalculation(quar nowQuar, string name);

	//处理we跳转(无条件)
	void weCalculation(quar nowQuar, string name);

	//处理结束程序
	void programEnd(quar nowQuar);

	//生成目标代码
	void generateCode();

	//判断一个字符串是否是数字
	int isNum(string nowStr);

	//进行寻址操作
	string findBpxxx(string nowOper, string name);

	//基本块集合
	vector<BaseBlock> BaseBlockColl;

	//四元式集合
	vector<quar> AllQuarColl;

	//将关系转换成汇编操作符，如LT对应JL
	string getRelAsm(string relOper);

	//用于判断之前是否是需要生成标号，if用这个
	string getName(int prevQuarNum);

	//如果需要的话生成标号，之后反填,if用这个
	void returnLabel(string name);

	//如果需要的话生成标号，之后反填,while用这个
	void returnWeLabel(string name, quar nowQuar);
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

	//目标代码区,生成的代码先存在这里，最后输入文件
	vector<code> targetCodeArea;

	//语义栈，用于登记if待反填的位置(目标代码区的序号)
	vector<int> ifSEM;

	//语义栈，用于登记else待反填的位置
	vector<int> elSEM;

	//语义栈，用于登记do待反填的位置
	vector<int> doSEM;

	//语义栈，用于登记 给we反填 的位置
	vector<int> whSEM;

	//标号顺序，防止标号重复
	int labelId = 0;

	//函数名字栈，用于生成对应的开始标号
	vector<string> funStack;

	//前一条四元式进行到哪了,用于查看前一条四元式是否符合一些条件
	int prevQuarNum = -1;
}; 
