#include "Lexical/lexical.h"
#include "GrammarAnalyzer/Grammar.h"
#include "GrammarAction/GrammarAction.h"
#include "Optimizer/Optimizer.h"
#include "TargetCodeGenerator/TargetCodeGenerator.h"
#include <bits/stdc++.h>

#define BATPATH "./todo.bat"    //用于快速启动dos的批处理文件的地址
static const string source=R"(..\Files\GrammarError.txt)";//默认源文件地址
static const string tokenOutput=R"(..\Files\tokenOutput.txt)";//默认输出地址
static const string middle=R"(..\Files\middle.txt)";//默认中间代码输出文件
static const string tokenFile=R"(..\Files\tokenOutput.txt)";//默认token输出文件
static const string exMiddle=R"(..\Files\exMiddle.txt)";//默认优化中间代码输出地址
static const string Symboltable=R"(..\Files\Symboltable.txt)";//默认符号表输出地址

using namespace std;
int main(int argc, char* argv[]) {
    string infile;
    if(argc == 1)   //cmd启动下读取源文件地址，否则使用默认源文件地址
        infile = source;
    else
        infile = argv[1];

    lexical lex(infile,tokenFile);//词法分析器初始化
    GrammarAction action(middle);//语义动作初始化
    Grammar grammar(lex, action);//语法分析链接词法分析器、语义动作

    grammar.analyzer();//语法分析

    ifstream in;//优化器输入输出路径配置
    in.open(middle);
    Optimizer optimizer(exMiddle);
    optimizer.optimize(in);
    in.close();


    TargetCodeGenerator tarCode;//目标代码生成器配置
    tarCode.read(exMiddle);
    tarCode.spiltMidCode();
    tarCode.generateCode();

    xTable.output(Symboltable);//符号表输出

    if(argc != 1) {//cmd启动模式下dos的配置批处理文件
        ofstream batfile(BATPATH);
        batfile << "masm target.asm;\nlink target;\ntarget.exe\n";
        batfile.close();
        system("DOSbox");//调用dos
    }
    return 0;
}