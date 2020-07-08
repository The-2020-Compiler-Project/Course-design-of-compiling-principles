/*
    lexical.h
*/
#ifndef LEX
#define LEX
#include<vector>
#include<string>
#include<fstream>
#include "../STable/STable.h"
using namespace std;
typedef enum {
    PROCEDURE,          // procedure 关键字
    FUNCTION,           // function 关键字
    CONST,              // const 关键字
    VAR,                // var 关键字
    ARRAY,              // array 关键字
    OF,                 // of 关键字
    INTEGER,            // integer 关键字
    CHAR,               // char 关键字
    BEGIN,              // begin 关键字
    END,                // end 关键字
    FOR,                // for 关键字
    DO,                 // do 关键字
    WHILE,              // while 关键字
    IF,                 // if 关键字
    THEN,               // then 关键字
    ELSE,               // else 关键字
    READ,               // read 关键字
    WRITE,              // write 关键字
    RESULT,             // result关键字
    TRUE,               // true关键字
    FALSE,              // false关键字
    CALL,               // call关键字
    PROGRAM,            // program关键字
    TYPE,               // type关键字
    RECORD,             // record关键字

    IDENTIFIER,         // 标识符
    NUMBER,             // 整数
    STRING,             // 字符串
    CHARATER,           // char 类型的字符
    REALNUMBER,         // 实数

    LEFT_BRACKET,       // 左括号
    RIGHT_BRACKET,      // 右括号
    LEFT_SQUARE,        // 左方括号
    RIGHT_SQUARE,       // 右方括号
    SINGLE_QUOTE,       // 单引号
    DOUBLE_QUOTE,       // 双引号
    COMMA,              // 逗号
    SEMICOLON,          // 分号
    PERIOD,             // 句点
    PLUS,               // 加号
    MINUS,              // 减号
    ASTERISK,           // 星号，乘号
    SLASH,              // 斜杠，除号
    COLON,              // 冒号
    EQUAL,              // 等号
    ASSIGN,             // 赋值符
    LESS,               // 小于号
    LESS_EQUAL,         // 小于等于号
    GREATER,            // 大于号
    GREATER_EQUAL,      // 大于等于号
    INEQUAL             // 不等号
}symbolType;

typedef struct
{
    symbolType symbol; // 新符号的类型
    int loc;//每个单词在它的类型里面的位置
    int row;//单词出现的行数
}token;

class lexical {
public:
    token next();
    lexical(const string& file);   // 私有化构造函数，单例模式
private:
    ifstream sourceFile; // 源程序文件输入流

    /*vector<string> IT;    //标识符表
    vector<char> CT;      //字符表
    vector<string> ST;    //字符串表
    vector<int> iT;       //整数表
    vector<double> rT;    //实数表*/
    friend class Grammar;
};

#endif