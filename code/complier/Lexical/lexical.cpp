/*
    lexical.cpp
    词法分析器的实现。
*/
#include <ctype.h>
#include <map>
#include<math.h>
#include<string>
#include<iostream>
#include "lexical.h"
using namespace std;
static char ch;         // 下一个即将读出来的（还没读到它）的字符
//  设这个变量的目的是提高效率

static map<string, symbolType> reservedWords;   // 保留字表，建立每个保留字与其类型之前的关系

// 真正的构造函数
lexical::lexical(const string& file) : sourceFile(file.c_str()) {
    // 将文件输入流置为参数
// 读进来文件中的第一个字符备用
    ch = sourceFile.get();
    //初始化保留字表
#define INSERT_RES(str, type)    \
        reservedWords.insert(pair<string, symbolType>(str, type))   // 定义宏，简化填表代码
    INSERT_RES("procedure", PROCEDURE);
    INSERT_RES("function", FUNCTION);
    INSERT_RES("const", CONST);
    INSERT_RES("var", VAR);
    INSERT_RES("array", ARRAY);
    INSERT_RES("of", OF);
    INSERT_RES("integer", INTEGER);
    INSERT_RES("char", CHAR);
    INSERT_RES("begin", BEGIN);
    INSERT_RES("end", END);
    INSERT_RES("for", FOR);
    INSERT_RES("do", DO);
    INSERT_RES("while", WHILE);
    INSERT_RES("if", IF);
    INSERT_RES("then", THEN);
    INSERT_RES("else", ELSE);
    INSERT_RES("read", READ);
    INSERT_RES("write", WRITE);
    INSERT_RES("result", RESULT);
    INSERT_RES("true", TRUE);
    INSERT_RES("false", FALSE);
    INSERT_RES("call", CALL);
    INSERT_RES("program", PROGRAM);
    INSERT_RES("type", TYPE);
    INSERT_RES("record", RECORD);

#undef  INSERT_RES                                                 // 取消定义 INSERT_RES 宏

    if (sourceFile.is_open())
        cout << "open!" << endl;

}

#define GET     ch = sourceFile.get()   // 定义宏，简化从文件中读取一个字符的操作
#define PEEK    ch = sourceFile.peek()  // 定义宏，简化查看文件流第一个字符的操作
int m = 1;//行号初始化为1
int flag = -1;//按名字查询返回位置，flag>=0为位置，flag=-1为未查询到
int a = -1;//a为返回的插入的位置
string Pt_str = "\0";//暂时存储界符，为了方便找到位置
//extern STable sTable;
token token1;
// 读取下一个符号，将符号类型存入 symbol,位置存入loc,行数存入row
token lexical::next() {
    while (ch != EOF) {
        while (isspace(ch) && (ch != '\n'))//读到空格后直接跳过，读下一个字符
            GET;
        while (ch == '\n') {//读到换行符号的时候，要将行数加一并且读下一个字符/
            m++;
            GET;
        }
        if (isdigit(ch)) {          // 遇数字，这里读到的一定是整数或者实数
            char c[100];//用来存储扫描的数字
            int n = 0;
            while (ch >= '0' && ch <= '9')//当向后扫描遇到不是数字的时候停止
            {
                c[n++] = ch;//从第一个扫描到的数字就放在数组c中
                GET;//然后读下一位继续判断是否为数数字，如果是数字，继续读入数组，如果不是，直接跳出循环
            }// 这里，循环跳出时，ch 是一个新的未处理的字符（不是数字）接下来要判断这个ch是不是小数点的点
            if (ch == '.') {//如果进入这个循环，就是实数，否则为整数
                c[n++] = ch;//识别小数，将点读进数组c
                GET;//然后读下一个字符，开始进行小数的后半部分的进行
                while (ch >= '0' && ch <= '9')//如果读到的字符是数字的话，继续存入数组c
                {
                    c[n++] = ch;
                    GET;//读下一个字符继续判断，当读到的字符不是数字时，跳出循环，小数的内容已经完全读入数组c中
                }//这里跳出循环时，ch是一个未处理的字符（不是数字）
                c[n] = '\0';//结尾
                double num = atof(c); //将字符串转换成double型
                flag = sTable.findRt(num);//去查找该实数的位置
                if (flag == -1) {//为-1则没有查到，要插入进去，并得到返回的位置值
                    a = sTable.inserRt(num);//a为浮点型常量插入放入位置
                    token1.loc = a;//a是得到的插入位置
                    token1.symbol = REALNUMBER;//类型为实数类型
                    token1.row = m;//行数只有在遇到换行符号时，m才会加一
                    return token1;//返回token值，下面同此
                }
                else {
                    token1.symbol = REALNUMBER;//flag值不是-1的话，返回的就是这个单词的位置，直接填写就可以
                    token1.loc = flag;//位置就是返回的flag值
                    token1.row = m;
                    return token1;
                }
            }
            //如果没有进入上面的if循环，那么就是整数类型，这个时候的ch时一个未处理的字符（不是数字也不是小数点），符合逻辑
            c[n] = '\0';
            int num = atof(c); //将字符串转换成int型
            flag = sTable.findNt(num);//同上
            if (flag == -1) {//未查询到，则要插入
                a = sTable.insertNt(num);
                token1.symbol = NUMBER;
                token1.loc = a;
                token1.row = m;
                return token1;
            }
            else {//查询到
                token1.symbol = NUMBER;
                token1.loc = flag;
                token1.row = m;
                return token1;
            }
        }
            // 这里，循环跳出时，ch 是一个新的未处理的字符，逻辑正确
        else if (isalpha(ch)) {   // 遇字母，这里读到的一定是标识符或保留字
            char word[10];//用来保存读出来的标识符串或者保留字
            int x = 0;
            while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch == '_') || isdigit(ch)) {
                word[x++] = ch;//将读到的所有可以作为标识符关键字的字符读入数组word
                GET;
            }//跳出循环时，ch是一个未处理的新字符
            word[x] = '\0';
            // 以下，判断读到的符号是标识符还是保留字，将其具体类型存入 symbol
            map<string, symbolType>::iterator it = reservedWords.find(word);
            if (it == reservedWords.end()) {      // 不在保留字表中，说明是标识符
                token1.symbol = IDENTIFIER;
                flag = sTable.findIt(word);//去标识符表中寻找
                if (flag == -1) {//没有找到的情况处理
                    a = sTable.insertIt(word);
                    token1.loc = a;
                    token1.row = m;
                    return token1;
                }
                else {
                    token1.loc = flag;
                    token1.row = m;
                    return token1;
                }
            }
            else {                               // 在保留字表中
                string Kt = it->first;//取这个关键字的名字
                flag = sTable.findPt(Kt);
                token1.symbol = it->second;// 要把保留字的具体类型存入 symbol
                token1.loc = flag;//关键字的loc代表它在符号表的位置
                token1.row = m;
                return token1;
            }
        }
        else if (ch == '\"') {//处理字符串
            token1.symbol = STRING;
            char y[100];
            int l = 0;
            y[l++] = ch;
            GET;
            while (ch != '\"') {
                y[l++] = ch;
                GET;
            }//循环跳出时，ch是双引号
            y[l++] = ch;
            y[l] = '\0';
            GET;//读一个下一个被处理的字符
            //判断该字符串是否存在于字符串表中
            flag = sTable.findSt(y);
            if (flag == -1) {//不在，要插入
                a = sTable.insertSt(y);
                token1.loc = a;
                token1.row = m;
                return token1;
            }
            else {
                token1.loc = flag;
                token1.row = m;
                return token1;
            }
        }
        else if (ch == '\'') {//处理字符
            token1.symbol = CHAR;
            char b[100];
            int k = 0;
            b[k++] = ch;
            GET;
            while (ch != '\'') {
                b[k++] = ch;
                GET;
            }//循环跳出时，ch是单引号
            b[k++] = ch;
            b[k] = '\0';
            GET;//读一个下一个被处理的字符
            //判断该字符是否存在于字符表中
            flag = sTable.findCt(b);
            if (flag == -1) {
                a = sTable.insertCt(b);
                token1.loc = a;
                token1.row = m;
                return token1;
            }
            else {
                token1.loc = flag;
                token1.row = m;
                return token1;
            }
        }
        else if (ch == '{') {//处理单行注释一{花括号里面的都被注释掉}
            GET;
            while (ch != '}') {
                GET;
            }//跳出循环时，ch是‘}’
            GET;//读一个要被处理的下一个字符
            return next();//调用函数自身，进行字符的处理
        }
        else if (ch == '/') {//处理单行注释二//
            PEEK;//查看下一个字符是不是/
            if (ch == '/') {//两个反斜杠同时出现就是单行注释，否则为界符除以号
                GET;//读第二个反斜杠
                while (ch != '\n') {//只有出现换行符号才说明单行注释结束
                    GET;//如果不出现换行符号就一直读下一个符号；
                }//跳出循环时，ch为换行符号
                m++;//行数m要加一
                GET;//读下一个字符以备分析
                return next();//调用自身进行下一个字符的分析
            }
            else {//下一个字符不是反斜杠，那么就是界符除以号
                Pt_str = "/";
                flag = sTable.findPt(Pt_str);
                token1.symbol = SLASH;//类型为反斜杠，除以号，是界符
                token1.loc = flag;//该界符在符号表的位置
                token1.row = m;
                GET;//将当前字符变为没有处理的字符
                return token1;
            }
        }
        else if (ch == '(') {//如果出现了‘（’那么有可能是多行注释，或者界符左括号，下面要去区分这个
            PEEK;//查看下一个ch的值来判断
            if (ch == '*') {//如果是‘（*’的组合，那么就是多行注释
                GET;//当前字符为*
                for (int j = 0;; j++) {
                    GET;//当前字符为*的下一位
                    while (ch != '*') {//只有出现了*,才有可能是注释结束的地方
                        GET;//如果不出现就一直读下一个符号
                    }//跳出循环时，ch是*，还要继续判断下一个符号是不是‘）’才能确定是否是多行注释结束的地方
                    PEEK;//判断这个是否为‘）’
                    if (ch == ')') //如果出现了*）的组合，说明多行注释结束
                        break;//如果出现了*）的组合才可以跳出循环，如果没有出现继续进行下一轮判断
                }
                GET;//当前符号为）
                GET;//读下一个字符以备分析
                return next();//调用自身进行分析
            }
            else {//只有左括号，那么就是界符左括号
                Pt_str = "(";
                flag = sTable.findPt(Pt_str);
                token1.symbol = LEFT_BRACKET;//类型为左括号，是界符
                token1.loc = flag;//该界符在符号表的位置
                token1.row = m;
                GET;//读下一个字符进行下一轮分析
                return token1;
            }
        }
        else if (!isspace(ch) && ch != EOF) {          //如果不是空白字符
            switch (ch) {
                case '<':
                    PEEK;
                    if (ch == '=') {
                        GET;//当前符号为=
                        Pt_str = "<=";
                        flag = sTable.findPt(Pt_str);
                        token1.symbol = LESS_EQUAL;
                        token1.loc = flag;
                        token1.row = m;
                        GET;//读一个为被处理的字符，下面同理
                        return token1;
                    }
                    else if (ch == '>') {
                        GET;//当前符号为>
                        Pt_str = "<>";
                        flag = sTable.findPt(Pt_str);
                        token1.symbol = INEQUAL;
                        token1.loc = flag;
                        token1.row = m;
                        GET;//读一个为被处理的字符，下面同理
                        return token1;
                    }
                    else  {
                        Pt_str = "<";
                        flag = sTable.findPt(Pt_str);
                        token1.symbol = LESS;
                        token1.loc = flag;
                        token1.row = m;
                        GET;
                        return token1;
                    }
                case '>':
                    PEEK;
                    if (ch == '=') {
                        GET;
                        Pt_str = ">=";
                        flag = sTable.findPt(Pt_str);
                        token1.symbol = GREATER_EQUAL;
                        token1.loc = flag;
                        token1.row = m;
                        GET;
                        return token1;
                    }
                    else  {
                        Pt_str = ">";
                        flag = sTable.findPt(Pt_str);
                        token1.symbol = GREATER;
                        token1.loc = flag;
                        token1.row = m;
                        GET;
                        return token1;
                    }
                case ':':
                    PEEK;
                    if (ch == '=') {
                        GET;
                        Pt_str = ":=";
                        flag = sTable.findPt(Pt_str);
                        token1.symbol = ASSIGN;
                        token1.loc = flag;
                        token1.row = m;
                        GET;
                        return token1;
                    }
                    else {
                        Pt_str = ":";
                        flag = sTable.findPt(Pt_str);
                        token1.symbol = COLON;
                        token1.loc = flag;
                        token1.row = m;
                        GET;
                        return token1;
                    }
                case ')':
                    //GET;
                    Pt_str = ")";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = RIGHT_BRACKET;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    return token1;
                case '[':
                    Pt_str = "[";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = LEFT_SQUARE;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    return token1;
                case ']':
                    Pt_str = "]";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = RIGHT_SQUARE;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    return token1;
                case '\'':
                    Pt_str = "\'";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = SINGLE_QUOTE;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    return token1;
                case '\"':
                    Pt_str = "\"";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = DOUBLE_QUOTE;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    return token1;
                case ',':
                    Pt_str = ",";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = COMMA;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    return token1;
                case ';':
                    Pt_str = ";";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = SEMICOLON;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    return token1;
                case '.':
                    Pt_str = ".";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = PERIOD;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    return token1;
                case '+':
                    Pt_str = "+";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = PLUS;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    return token1;
                case '-':
                    Pt_str = "-";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = MINUS;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    return token1;
                case '*':
                    Pt_str = "*";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = ASTERISK;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    return token1;
                case '=':
                    Pt_str = "=";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = EQUAL;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    return token1;
                default:  //这里进行非法字符的报错
                    if(ch==-1){
                        cout<<"over"<<endl;
                    }
                    cout << "此单词为错误单词" << endl;
                    GET;
                    exit(1);
            }
        }
    }
}