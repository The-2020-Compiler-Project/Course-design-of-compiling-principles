#define WITHOUT_ERROR
#include "../Lexical/lexical.h"
#include "../XTable/FunSheet.h"
#include "../GrammarAction/GrammarAction.h"
class Grammar{

public:

#ifdef WITHOUT_ERROR
  Grammar(lexical &,GrammarAction&);  //构造函数，实例化一个词法分析器对象
#elif  defined(WITH_ERROR)
//加入错误处理
#endif

  void analyzer();     //分析器

private: 
  token word;
  lexical& _lexical;
  GrammarAction& _action;
#ifndef WITHOUT_ERROR
//错误处理器
#endif
  void Program();                       //<程序>
  void SubProgram();                    //<分程序>
  void ConstDecl();                     //<常量说明部分>
  void ConstDef();                      //<常量定义>
  pair<string,string> IsConst();                       //<常量>
  void VarDecl();                       //<变量说明部分>
  void VarDef();                        //<变量说明>
  void TypeDecl();                      //<类型定义部分>
  void TypeDef();                       //<类型定义>
  void ArrayDef();                      //<数组类型定义>
  void RecordDef();                     //<结构体类型定义>
  void DomainDef();                     //<域名定义>
  void ProcedureDecl();                 //<过程说明部分>
  void ProcedureHead();                 //<过程首部>
  void FunctionDecl();                  //<函数说明部分>
  void FunctionHead();                  //<函数首部>
  void FormalParameterList();           //<形式参数表>
  pair<CAT,string> FormalParameter();               //<形式参数段>
  string Type();                          //<类型>··················判断标识符
  string BasicType();                     //<基本类型>
  void BoolType();                      //<布尔类型>
  void CompoundStatement();             //<复合语句>
  void Statement();                     //<语句>··················判断标识符
  void AssignStatement();               //<赋值语句>
  void Variable();                      //<变量>··················判断标识符
  void ReturnStatement();               //<返回值语句>
  void IfStatement();                   //<条件语句>
  void Condition();                     //<条件>
  void WhileStatement();                //<while循环语句>
  void CallStatement();                 //<调用语句>··············判断标识符
  void CallFunction();                  //<函数调用语句>···········判断标识符
  void CallProcedure();                 //<过程调用语句>···········判断标识符
  int RealParameterList();             //<实在参数表>
  void RealParameter();                 //<实在参数>
  void Expression();                    //<表达式>
  void Item();                          //<项>
  void Factor();                        //<因子>··················判断标识符
  void AddOperator();                   //<加法运算符>························可以有返回值
  void MulOperator();                   //<乘法运算符>························可以有返回值
  void BoolOperator();                  //<关系运算符>························可以有返回值
};