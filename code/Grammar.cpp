#include "Grammar.h"
#include <iostream>
#define NEXT word=_lexical.next()

//错误处理，DEBUG用
#define ERROR(cat)  {   \                                        
            cout<<"Line "<<word.row<<": "<<cat<<endl; \
            exit(1);        \
        }                         

Grammar::Grammar(lexical& LA):_lexical(LA) {}

void Grammar::analyzer(){
    NEXT;
    Program();
}

void Grammar::Program(){
    if(word.symbol != PROGRAM)
        ERROR('!');
    NEXT;
    if(word.symbol != IDENTIFIER)         //填符号表
        ERROR('!');
    NEXT;
    if(word.symbol != SEMICOLON)
        ERROR('!');
    NEXT;
    SubProgram();
    if(word.symbol != PERIOD)
        ERROR('!');
    NEXT;
}

void Grammar::SubProgram(){
    if(word.symbol == CONST)
        ConstDecl();
    if(word.symbol == VAR)
        VarDecl();
    //.....
    CompoundStatement();
}

void Grammar::ConstDecl(){
    if(word.symbol != CONST)
        ERROR('!');
    NEXT;
    ConstDef();
    while(word.symbol == COMMA){
        NEXT;
        ConstDef();
    }
    if(word.symbol != SEMICOLON)
        ERROR('!');
    NEXT;
}

void Grammar::ConstDef(){
    if(word.symbol != IDENTIFIER)     
        ERROR('!');                   //重定义判断          
    NEXT;                             //记录标识符
    if(word.symbol != EQUAL)
        ERROR('!');
    NEXT;
    IsConst();
    //填符号表.........想办法解决返回值.......返回常数表的指针？
}

void Grammar::IsConst(){
    if(word.symbol == PLUS || word.symbol == MINUS){
        // 记录符号
        NEXT;
        if(word.symbol != NUMBER)
            ERROR('!');
        // 记录值
        // 填符号表
        NEXT;
    }
    else if(word.symbol == NUMBER){
        //记录值
        //填符号表
        NEXT;
    }
    else if(word.symbol == CHARATER){
        //记录值
        //填符号表
        NEXT;
    }
    else
    {
        ERROR('!');
    }
    
}

void Grammar::VarDecl(){
    if(word.symbol != VAR)
        ERROR('!');
    NEXT;
    VarDef();
    if(word.symbol != SEMICOLON)
        ERROR('!');
    NEXT;
    while(word.symbol == IDENTIFIER){
        VarDef();
        if(word.symbol != SEMICOLON)
            ERROR('!');
        NEXT;
    }
}

void Grammar::VarDef(){
    if(word.symbol != IDENTIFIER)
        ERROR('!');
    //记录标识符
    //判断重定义
    NEXT;
    while(word.symbol == COMMA){
        NEXT;
        if(word.symbol != IDENTIFIER)
            ERROR('!');
        //记录标识符
        //判断重定义
        NEXT;
    }
    if(word.symbol != COLON)
        ERROR('!');
    NEXT;
    TypeDecl();
    // 填写符号表........想办法解决返回值.......返回类型表的指针？
}

void Grammar::TypeDecl(){
    BasicType();
    // 返回类型表指针？
    // 其他用户自定义数据类型
}

void Grammar::BasicType(){
    if(word.symbol == INTEGER)
    {
        NEXT;
        return;
    }
    else if (word.symbol == CHAR)
    {
        NEXT;
        return;
    }
    else if (word.symbol == BOOL)
    {
        NEXT;
        return;
    }
    else if (word.symbol == REAL)
    {
        NEXT;
        return;
    }
    else
    {
        ERROR('!');
    }   
}

void Grammar::CompoundStatement(){
    if(word.symbol != BEGIN)
        ERROR('!');
    NEXT;
    Statement();
    while(word.symbol == SEMICOLON){
        NEXT;
        Statement();
    }
    if(word.symbol != END)
        ERROR('!');
    NEXT;
}

void Grammar::Statement(){
    if(word.symbol == IDENTIFIER)
        AssignStatement();
}

void Grammar::AssignStatement(){
    if(word.symbol != IDENTIFIER)
        ERROR('!');
    // 未定义？
    NEXT;
    if(word.symbol != ASSIGN)
        ERROR('!');
    NEXT;
    Expression();
}

void Grammar::Expression(){
    if (word.symbol == PLUS || word.symbol == MINUS){
        //记录符号
        //负号四元式？
        NEXT;
    }
    Item();
    while(word.symbol == PLUS || word.symbol == MINUS){
        AddOperator();
        //咋返回。。。怎么生成四元式。。。想不明白
        Item();
    }
}

void Grammar::Item(){
    Factor();
    while(word.symbol == ASTERISK || word.symbol == SLASH){
        MulOperator();
        Factor();
    }
}

void Grammar::Factor(){
    if(word.symbol == IDENTIFIER){
        //未定义？
        //记录值
        NEXT;
    }
    else if(word.symbol == NUMBER){
        //记录值
        NEXT;
    }
    else if(word.symbol == LEFT_BRACKET){
        NEXT;
        Expression();
        if(word.symbol != RIGHT_BRACKET)
            ERROR('!');
        NEXT;
    }
    else
    {
        ERROR('!');
    }   
}

void Grammar::AddOperator(){
    if(word.symbol == PLUS){
        NEXT;
        //语义，但我不知道咋弄
        return;
    }
    else if(word.symbol == MINUS){
        NEXT;
        return;
    }
    else{
        ERROR('!');
    }
}

void Grammar::MulOperator(){
    if(word.symbol == ASTERISK){
        NEXT;
        return;
    }
    else if(word.symbol == SLASH){
        NEXT;
        return;
    }
    else{
        ERROR('!');
    }
}