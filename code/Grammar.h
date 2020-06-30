#define WITHOUT_ERROR
#include "lexical.h"

class Grammar{

public:

#ifdef WITHOUT_ERROR
  Grammar(lexical &);  //构造函数，实例化一个词法分析器对象
#elif  defined(WITH_ERROR)
//加入错误处理
#endif

  void analyzer();     //分析器

private: 
  token word;
  lexical& _lexical;
#ifndef WITHOUT_ERROR
//错误处理器
#endif
  void Program();                       //<程序>
  void SubProgram();                    //<分程序>
  void ConstDecl();                     //<常量说明部分>
  void ConstDef();                      //<常量定义>
  void IsConst();                       //<常量>
  void VarDecl();                       //<变量说明部分>
  void VarDef();                        //<变量说明>
  void TypeDecl();                      //<类型>
  void BasicType();                     //<基本类型>
  void CompoundStatement();             //<复合语句>
  void Statement();                     //<语句>
  void AssignStatement();               //<赋值语句>
  void Expression();                    //<表达式>
  void Item();                          //<项>
  void Factor();                        //<因子>
  void AddOperator();                   //<加法运算符>
  void MulOperator();                   //<乘法运算符>

};