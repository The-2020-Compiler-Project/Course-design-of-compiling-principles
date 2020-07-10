#include "Lexical/lexical.h"
#include "GrammarAnalyzer/Grammar.h"
#include "GrammarAction/GrammarAction.h"
#include "Optimizer/Optimizer.h"
#include "TargetCodeGenerator/TargetCodeGenerator.h"
#include <bits/stdc++.h>

#define BATPATH "./todo.bat"    //���ڿ�������dos���������ļ��ĵ�ַ
static const string source=R"(..\Files\GrammarError.txt)";//Ĭ��Դ�ļ���ַ
static const string tokenOutput=R"(..\Files\tokenOutput.txt)";//Ĭ�������ַ
static const string middle=R"(..\Files\middle.txt)";//Ĭ���м��������ļ�
static const string tokenFile=R"(..\Files\tokenOutput.txt)";//Ĭ��token����ļ�
static const string exMiddle=R"(..\Files\exMiddle.txt)";//Ĭ���Ż��м���������ַ
static const string Symboltable=R"(..\Files\Symboltable.txt)";//Ĭ�Ϸ��ű������ַ

using namespace std;
int main(int argc, char* argv[]) {
    string infile;
    if(argc == 1)   //cmd�����¶�ȡԴ�ļ���ַ������ʹ��Ĭ��Դ�ļ���ַ
        infile = source;
    else
        infile = argv[1];

    lexical lex(infile,tokenFile);//�ʷ���������ʼ��
    GrammarAction action(middle);//���嶯����ʼ��
    Grammar grammar(lex, action);//�﷨�������Ӵʷ������������嶯��

    grammar.analyzer();//�﷨����

    ifstream in;//�Ż����������·������
    in.open(middle);
    Optimizer optimizer(exMiddle);
    optimizer.optimize(in);
    in.close();


    TargetCodeGenerator tarCode;//Ŀ���������������
    tarCode.read(exMiddle);
    tarCode.spiltMidCode();
    tarCode.generateCode();

    xTable.output(Symboltable);//���ű����

    if(argc != 1) {//cmd����ģʽ��dos�������������ļ�
        ofstream batfile(BATPATH);
        batfile << "masm target.asm;\nlink target;\ntarget.exe\n";
        batfile.close();
        system("DOSbox");//����dos
    }
    return 0;
}