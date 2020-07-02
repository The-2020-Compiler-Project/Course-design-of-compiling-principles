#pragma once
#include<vector>
#include<map>
#include<set>
#include<string>
using std::vector;
using std::pair;
using std::make_pair;
using std::string;
using std::set;
using std::map;


//四元式
struct quar {
	string oper;
	pair<string, string> o1, o2, target;
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

	//判断操作数pair是否为全空,不空返回0，左空返回1，右空返回2，全空返回3
	int isEmpty(pair<string, string> nowPair);

	//初始化活跃信息，临时变量为no，用户变量为yes
	void initBlockActive();

	//计算活跃信息，用的上课ppt的方法
	void checkBlockActive();

};


//目标代码生成器，生成8086汇编
//暂时先使用单寄存器，完成了之后考虑多寄存器
//可以实现+，-，*，/，:=，if，while
class TargetCodeGenerator {
public:
	//从文件中读入四元式
	void read(string filename);

	//判断是否是跳转指令
	bool isJump(quar nowQuar);

	//分割基本块
	void spiltMidCode();

	//生成目标代码
	void generateCode();

	//基本块集合
	vector<BaseBlock> BaseBlockColl;

	//四元式集合
	vector<quar> AllQuarColl;
private:
	const int JumpQuarSize = 5;
	//endif也算作跳转语句，跳转到下一个四元式(●ˇ∀ˇ●)
	string Jump[5] = { "do","we","if","el","ie" };
};