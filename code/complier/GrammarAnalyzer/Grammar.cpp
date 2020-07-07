#include "Grammar.h"
#include <iostream>
#include <stack>
#include <vector>
//extern STable sTable;

// 读下一个token
#define NEXT word=_lexical.next()

//错误处理，DEBUG用
#define ERROR(cat)  {                                 \                                        
            cout<<"Line "<<word.row<<": "<<cat<<endl; \
            exit(1);                                  \
        }                         

Grammar::Grammar(lexical& LA,GrammarAction& action):_lexical(LA) ,_action(action){}

token Pre_word;

string tmp_record;

FunSheet::iterator Fun_It;

//可能需要一个类型栈，一个参数栈

stack<string> TypeIdentifier;

vector<string> DomainName;

vector<string> DomainType;

vector<string> Parameters;

vector<pair<pair<CAT,string>,vector<string>>> ParameterLists;


void Grammar::analyzer(){
    NEXT;
    Program();
}

//<程序>
void Grammar::Program(){                  //<程序>
    if(word.symbol != PROGRAM)
        ERROR('!');
    NEXT;
    if(word.symbol != IDENTIFIER)         //填符号表
        ERROR('!');
    //创建主函数
    string name = sTable.searchIt(word.loc);
    Fun_It = xTable.creatMain(name);
    NEXT;

    //函数迭代器,pushFunStack
    _action.pushFunStack(Fun_It);
    //−，beginProgram
    _action.beginProgram();

    if(word.symbol != COLON)
        ERROR('!');
    NEXT;
    SubProgram();
    if(word.symbol != PERIOD)
        ERROR('!');
    NEXT;
}

//<分程序>
void Grammar::SubProgram(){             //<分程序>
    if(word.symbol == TYPE)
        TypeDecl();
    if(word.symbol == CONST)
        ConstDecl();
    if(word.symbol == VAR)
        VarDecl();
    while(word.symbol == PROCEDURE || word.symbol == FUNCTION){
        if(word.symbol == PROCEDURE)
            ProcedureDecl();
        else if(word.symbol == FUNCTION)
            FunctionDecl();
    }
    //−，beginFunction
    _action.beginFunction();
    CompoundStatement();
    //−,endFunction
    _action.endFunction();
}

//<常量说明部分>
void Grammar::ConstDecl(){            //<常量说明部分>
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

//<常量定义>
void Grammar::ConstDef(){             
    if(word.symbol != IDENTIFIER)     
        ERROR('!'); 
    
    //记录标识符                     
    string name = sTable.searchIt(word.loc);
    //重定义判断  
    auto pr = Fun_It.find(name);
    if(pr.first) ERROR('!');  

    NEXT;                             //记录标识符
    if(word.symbol != EQUAL)
        ERROR('!');
    NEXT;
    auto _pr=IsConst();
    Fun_It.addConst(name,_pr.second,_pr.first);
}

//<常量>       first---value, second---type
pair<string,string> Grammar::IsConst(){ 
    bool isNeg = false;
    if(word.symbol == PLUS || word.symbol == MINUS){
        if(word.symbol == MINUS) isNeg = true;
        NEXT;
        if(word.symbol != NUMBER)
            ERROR('!');
        //记录值
        string num = sTable.searchNt(word.loc);
        NEXT;
        //添加符号
        if(isNeg){
            string tmp = "-";
            num=tmp+num;
        }
        return make_pair(num,"integer");
    }
    else if(word.symbol == NUMBER){
        //记录值
        string num = sTable.searchNt(word.loc);
        NEXT;
        return make_pair(num,"integer");
    }
    else if(word.symbol == CHARATER){
        //记录值
        string num = sTable.searchCt(word.loc);
        NEXT;
        return make_pair(num,"char");
    }
    else
    {
        ERROR('!');
    }
    
}

//<变量说明部分>
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

//<变量说明>
void Grammar::VarDef(){             
    if(word.symbol != IDENTIFIER)
        ERROR('!');
    //记录标识符
    string name = sTable.searchIt(word.loc);
    //判断重定义
    auto pr = Fun_It.find(name);
    if(pr.first) ERROR('!');

    vector<string> nameV;
    //标识符存入向量
    nameV.push_back(name);
    
    NEXT;
    while(word.symbol == COMMA){
        NEXT;
        if(word.symbol != IDENTIFIER)
            ERROR('!');
        
        //记录标识符
        name = sTable.searchIt(word.loc);
        //判断重定义
        pr = Fun_It.find(name);
        if(pr.first) ERROR('!');
        //标识符存入向量
        nameV.push_back(name);

        NEXT;
    }
    if(word.symbol != COLON)
        ERROR('!');
    NEXT;
    string _type=Type();
    //增加变量定义
    for(auto s:nameV)
        Fun_It.addVariable(s,_type);
}

//<类型>
string Grammar::Type(){
    if(word.symbol == IDENTIFIER){
        string name = sTable.searchIt(word.loc);
        auto pr=Fun_It.find(name);
        if(!pr.first || pr.second != catD) ERROR('!');      //未找到标识符或语义角色不是类型报错
        NEXT;
        return name;
    }
    else          
        return BasicType();
    // 返回类型表指针？
    // 其他用户自定义数据类型
}

//<基本类型>
string Grammar::BasicType(){
    if(word.symbol == INTEGER)
    {
        NEXT;
        return "int";
    }
    else if (word.symbol == CHAR)
    {
        NEXT;
        return "char";
    }
    // else if (word.symbol == BOOL)
    // {
    //     NEXT;
    //     return;
    // }
    // else if (word.symbol == REAL)
    // {
    //     NEXT;
    //     return;
    // }
    else
    {
        ERROR('!');
    }   
}

//<类型定义部分>
void Grammar::TypeDecl(){
    if(word.symbol != TYPE)
        ERROR('!');
    NEXT;
    TypeDef();
    if(word.symbol != SEMICOLON)
        ERROR('!');
    NEXT;
    while(word.symbol == IDENTIFIER){
        TypeDef();
        if(word.symbol != SEMICOLON)
            ERROR('!');
        NEXT;
    }
}

//<类型定义>
void Grammar::TypeDef(){
    if(word.symbol != IDENTIFIER)
        ERROR('!');
    //压入类型定义标识符栈
    string name = sTable.searchIt(word.loc);
    TypeIdentifier.push(name);
    
    NEXT;
    if(word.symbol != EQUAL)
        ERROR('!');
    NEXT;
    if(word.symbol == ARRAY){
        ArrayDef();
    }
    else if(word.symbol == RECORD){
        RecordDef();
    }
    else
    {
        ERROR('!');
    }
    
}

//<数组类型定义> 
void Grammar::ArrayDef(){
    if(word.symbol != ARRAY)
        ERROR('!');
    NEXT;
    if(word.symbol != LEFT_SQUARE)
        ERROR('!');
    NEXT;
    if(word.symbol != NUMBER)
        ERROR('!');

    //记录常数值
    string number = sTable.searchNt(word.loc); 
    int value = stoi(number);

    NEXT;
    if(word.symbol != RIGHT_SQUARE)
        ERROR('!');
    NEXT;
    if(word.symbol != OF)
        ERROR('!');
    NEXT;
    // if(word.symbol != IDENTIFIER)
    //     ERROR('!');
    // NEXT;
    string _type=Type();

    //增加数组类型定义，类型定义标识符栈弹栈
    string name = TypeIdentifier.top();
    TypeIdentifier.pop();
    Fun_It.addArrType(name,_type,value);
}

//<结构体类型定义>
void Grammar::RecordDef(){
    if(word.symbol != RECORD)
        ERROR('!');
    NEXT;
    DomainDef();
    if(word.symbol != SEMICOLON)
        ERROR('!');
    NEXT;
    while(word.symbol == IDENTIFIER){
        DomainDef();
        if(word.symbol != SEMICOLON)
            ERROR('!');
        NEXT;
    }
    if(word.symbol != END)
        ERROR('!');
    NEXT;
    //增加结构体类型定义，域名向量、域名类型向量清空
    string name = TypeIdentifier.top();
    TypeIdentifier.pop();
    Fun_It.addStructType(name,DomainName,DomainType);
    DomainName.clear(); DomainType.clear();
}

//<域名定义>
void Grammar::DomainDef(){
    if(word.symbol != IDENTIFIER)
        ERROR('!');
    //域名标识加入，域名向量
    string name = sTable.searchIt(word.loc);
    DomainName.push_back(name);

    NEXT;
    if(word.symbol != COLON)
        ERROR('!');
    NEXT;
    string _type=Type();
    //类型标识加入，域名类型向量
    DomainType.push_back(_type);
}

//<过程说明部分>
void Grammar::ProcedureDecl(){
    ProcedureHead();
    //此时函数迭代器已经是该过程的迭代器
    SubProgram();
    if(word.symbol != SEMICOLON)
        ERROR('!');
    NEXT;
    //此时函数迭代器已回到上层
    --Fun_It;
}

//<过程首部>
void Grammar::ProcedureHead(){
    if(word.symbol != PROCEDURE)
        ERROR('!');
    NEXT;
    if(word.symbol != IDENTIFIER)
        ERROR('!');

    //记录标识符
    string name = sTable.searchIt(word.loc);
    //判断重定义
    auto pr = Fun_It.find(name);
    if(pr.first) ERROR('!');
    //增加过程
    Fun_It=Fun_It.addFunction(name,catP);
    //此时函数迭代器已进入新过程/函数

    //函数迭代器；pushFunStack
    _action.pushFunStack(Fun_It);
    //−，beginProgram
    _action.beginProgram();

    NEXT;
    if(word.symbol == LEFT_BRACKET)
        FormalParameterList();
    if(word.symbol != SEMICOLON)
        ERROR('!');
    NEXT;
    for(auto pr1:ParameterLists){
        for(auto s:pr1.second){
            Fun_It.addParameter(s,pr1.first.first,pr1.first.second);
        }
    }
    ParameterLists.clear();
}

//<函数说明部分>
void Grammar::FunctionDecl(){
    FunctionHead();
    SubProgram();
    //此时函数迭代器已经是该函数的迭代器
    if(word.symbol != SEMICOLON)
        ERROR('!');
    NEXT;
    //此时函数迭代器已回到上层
    --Fun_It;
}

//<函数首部>
void Grammar::FunctionHead(){
    if(word.symbol != FUNCTION)
        ERROR('!');
    NEXT;
    if(word.symbol != IDENTIFIER)
        ERROR('!');

    //记录标识符
    string name = sTable.searchIt(word.loc);

    NEXT;
    if(word.symbol == LEFT_BRACKET)
        FormalParameterList();
    if(word.symbol != COLON)
        ERROR('!');
    NEXT;
    string _type=BasicType();
    if(word.symbol != SEMICOLON)
        ERROR('!');
    NEXT;

    //增加函数
    Fun_It=Fun_It.addFunction(name,catF,_type);

    //函数迭代器；pushFunStack
    _action.pushFunStack(Fun_It);
    //−，beginProgram
    _action.beginProgram();

    //新函数增加参数
    for(auto pr1:ParameterLists){
        for(auto s:pr1.second){
            Fun_It.addParameter(s,pr1.first.first,pr1.first.second);
        }
    }
    ParameterLists.clear();
}

//<形式参数表>
void Grammar::FormalParameterList(){
    if(word.symbol != LEFT_BRACKET)
        ERROR('!');
    NEXT;
    auto pr=FormalParameter();

    ParameterLists.push_back(make_pair(pr,Parameters));
    Parameters.clear();

    while(word.symbol == SEMICOLON){
        NEXT;
        pr=FormalParameter();

        ParameterLists.push_back(make_pair(pr,Parameters));
        Parameters.clear();
    }
    if(word.symbol != RIGHT_BRACKET)
        ERROR('!');
    NEXT;
}

//<形式参数段>  first---Vn or Vf, second---type
pair<CAT,string> Grammar::FormalParameter(){
    CAT _cat = catVf;
    if(word.symbol == VAR){
        _cat = catVn;
        NEXT;
    }
    if(word.symbol != IDENTIFIER)
        ERROR('!');

    //记录标识符
    string name = sTable.searchIt(word.loc);
    //加入标识符向量
    Parameters.push_back(name);

    NEXT;
    while(word.symbol == COMMA){
        NEXT;
        if(word.symbol != IDENTIFIER)
            ERROR('!');
        
        //记录标识符
        name = sTable.searchIt(word.loc);
        //加入标识符向量
        Parameters.push_back(name);

        NEXT;
    }
    if(word.symbol != COLON)
        ERROR('!');
    NEXT;
    string _type=BasicType();
    return make_pair(_cat,_type);
}



//<复合语句>
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

//<语句>
void Grammar::Statement(){
    if(word.symbol == IDENTIFIER){
        string name = sTable.searchIt(word.loc);
        auto pr=Fun_It.find(name);
//////////////////////////////////////////////////////////////////////////
        if(pr.first){
            if(pr.second == catV){
                AssignStatement();
            }
            else if(pr.second == catF || pr.second == catP){
                auto tmp = Fun_It;
                Fun_It = Fun_It.getFunIterator(name);
                //此时进入调用语句，函数迭代器时改函数的迭代器
                CallStatement();
                //退出调用语句，函数迭代器恢复到当前函数
                Fun_It = tmp;
            }
            else{
                ERROR('!');
            }
        }
        else{
            auto tmp = Fun_It;
            Fun_It = Fun_It.getFunIterator(name);
            if(!Fun_It.useful()) ERROR('!');
            //此时进入调用语句，函数迭代器时改函数的迭代器
            CallStatement();
            //退出调用语句，函数迭代器恢复到当前函数
            Fun_It = tmp;
        }
////////////////////////////////////////////////////////////////////////////        
        
    }
    else if(word.symbol == RESULT){
        ReturnStatement();
    }
    else if (word.symbol == IF)
    {
        IfStatement();
    }
    else if(word.symbol == WHILE)
    {
        WhileStatement();
    }
    else if(word.symbol == BEGIN)
    {
        CompoundStatement();
    }
}

//<赋值语句>
void Grammar::AssignStatement(){
    Variable();
    if(word.symbol != ASSIGN)
        ERROR('!');
    NEXT;
    Expression();
    //_,Assign
    _action.Assign();
}

//<变量>
void Grammar::Variable(){
    if(word.symbol != IDENTIFIER)
        ERROR('!');

    //判断标识符类型
    string name = sTable.searchIt(word.loc);
    auto pr=Fun_It.find(name);
    if(! pr.first || pr.second != catV) ERROR('!');    //未找到标识符或语义角色不是变量报错
    auto e_it = Fun_It.getElemIterator(name);
    auto t_it = e_it.type();
    TYPEVAL t = t_it.tVal();
    
    if(t == typeValI || t == typeValC){
        NEXT;
        //string,pushObjectStack
        _action.pushObjectStack(name);
        return;
    }
    else if(t == typeValA){
        NEXT;

        //string,pushObjectStack
        _action.pushObjectStack(name);

        if(word.symbol != LEFT_SQUARE)
            ERROR('!');
        NEXT;
        Expression();
        if(word.symbol != RIGHT_SQUARE)
            ERROR('!');
        NEXT;

        //−，getAddress
        _action.getAddress();

        return;
    }
    else if(t == typeValD){

        //string,pushObjectStack
        _action.pushObjectStack(name);

        NEXT;
        if(word.symbol != PERIOD)
            ERROR('!');
        NEXT;
        if(word.symbol != IDENTIFIER)
            ERROR('!');

        string dname = sTable.searchIt(word.loc);

        { //语义检查
            auto _it=Fun_It.getElemIterator(name);
            auto _itType=_it.type();
            if(_itType.sonOffSet(dname) == -1)
                ERROR('!');
        }
        
        //string,pushObjectStack
        _action.pushObjectStack(dname);

        NEXT;
        //−，getSonAddress
        _action.getSonAddress();
        return;
    }
}

//<返回值语句>
void Grammar::ReturnStatement(){
    if(word.symbol != RESULT)
        ERROR('!');
    NEXT;
    if(word.symbol != ASSIGN)
        ERROR('!');
    NEXT;
    Expression();

    //−，assignResult
    _action.assignResult();
}

//<条件语句>
void Grammar::IfStatement(){
    if(word.symbol != IF)
        ERROR('!');
    NEXT;
    Condition();
    if(word.symbol != THEN)
        ERROR('!');
    NEXT;
    //−，beginIf
    _action.beginIf();
    Statement();
    if(word.symbol == ELSE){
        NEXT;
        //−,Else
        _action.Else();
        Statement();
    }
   // −,endIf
    _action.endIf();
}

//<条件>
void Grammar::Condition(){
    Expression();
    BoolOperator(); 
    Expression();
    //−，Relation
    _action.Relation();
}

//<while循环语句>
void Grammar::WhileStatement(){
    if(word.symbol != WHILE)
        ERROR('!');
    NEXT;

    //−，beginWhile
    _action.beginWhile();

    Condition();
    if(word.symbol != DO)
        ERROR('!');
    NEXT;

    //_,Do
    _action.Do();

    Statement();
    if(word.symbol != SEMICOLON)
        ERROR('!');
    NEXT;

    //_,endWhile
    _action.endWhile();
}

//<调用语句>
void Grammar::CallStatement(){
    //此时函数迭代器已经是当前调用的函数的迭代器
    if(word.symbol != IDENTIFIER)
        ERROR('!');

    if(Fun_It.cat() == catF){
        CallFunction();
        //−，popObjectStack
        _action.popObjectStack();
    }
    else if(Fun_It.cat() == catP)
        CallProcedure();
    else
    {
        ERROR('!');
    }
    
}

//<函数调用语句>
void Grammar::CallFunction(){
    //此时函数迭代器已经是当前调用的函数的迭代器
    if(word.symbol != IDENTIFIER)
        ERROR('!');
        
    if(Fun_It.cat() != catF) ERROR('!'); //标识符不是函数报错
    
    //string，pushFunCallStack
    _action.pushFunCallStack(Fun_It.name());


    NEXT;

    if(word.symbol == LEFT_BRACKET){
        //参数个数判断
        int parameter_num=Fun_It.parameterNum();
        int count=RealParameterList();
        if(count != parameter_num) ERROR('!');
    }

    //−,moveParameter
    _action.moveParameter();
    //−，Call
    _action.Call();
    //−，getResult
    _action.getResult();
    //−，popFunCallStack
    _action.popFunCallStack();
}

//<过程调用语句>
void Grammar::CallProcedure(){
    //此时函数迭代器已经是当前调用的函数的迭代器
    if(word.symbol != IDENTIFIER)
        ERROR('!');
    
    //判断标识符类型
    if(Fun_It.cat() != catP) ERROR('!'); //标识符不是过程报错

    //string，pushFunCallStack
    _action.pushFunCallStack(Fun_It.name());

    NEXT;

    if(word.symbol == LEFT_BRACKET){
        //参数个数判断
        int parameter_num=Fun_It.parameterNum();
        int count=RealParameterList();
        if(count != parameter_num) ERROR('!');
    }

    //−,moveParameter
    _action.moveParameter();
    //−，Call
    _action.Call();
    //−，popFunCallStack
    _action.popFunCallStack();
}

//<实在参数表>  返回参数个数
int Grammar::RealParameterList(){
    if(word.symbol != LEFT_BRACKET)
        ERROR('!');
    NEXT;
    int count=0;
    RealParameter();
    count++;
    while(word.symbol == COMMA){
        NEXT;
        RealParameter();
        count++;
    }
    if(word.symbol != RIGHT_BRACKET)
        ERROR('!');
    NEXT;
    return count;
}

//<实在参数>
void Grammar::RealParameter(){
    Expression();
}

//<表达式>
void Grammar::Expression(){
    if (word.symbol == MINUS){
        NEXT;
    }
    Item();
    while(word.symbol == PLUS || word.symbol == MINUS){
        AddOperator();
        Item();
        //−，Arithmetic
        _action.Arithmetic();
    }
}

//<项>
void Grammar::Item(){
    Factor();
    while(word.symbol == ASTERISK || word.symbol == SLASH){
        MulOperator();
        Factor();
        //−，Arithmetic
        _action.Arithmetic();
    }
}

//<因子>
void Grammar::Factor(){
    if(word.symbol == IDENTIFIER){
        string name = sTable.searchIt(word.loc);
        auto pr=Fun_It.find(name);
//////////////////////////////////////////////////////////////////////
        if(pr.first){
            if(pr.second == catV){
                Variable();
            }
            else if(pr.second == catF){
                auto tmp = Fun_It;
                Fun_It = Fun_It.getFunIterator(name);
                //此时进入调用语句，函数迭代器是该函数的迭代器
                CallFunction();
                //退出调用语句，函数迭代器恢复到当前函数
                Fun_It = tmp;
            }
            else if(pr.second == catC){
                //类型检查，是否为整数
                auto e_it = Fun_It.getElemIterator(name);
                auto t_it = e_it.type();
                TYPEVAL t = t_it.tVal();
                if(t != typeValI)
                    ERROR('!');
                NEXT;

                //0+string,pushObjectStack
                string tmp="0";
                _action.pushObjectStack(tmp+name);
            }
            else{
                ERROR('!');
            }
        }
        else{
            auto tmp = Fun_It;
            Fun_It = Fun_It.getFunIterator(name);
            if(!Fun_It.useful()) ERROR('!');
            //此时进入调用语句，函数迭代器是该函数的迭代器
            CallFunction();
            //退出调用语句，函数迭代器恢复到当前函数
            Fun_It = tmp;
        }
////////////////////////////////////////////////////////////////////
    }
    else if(word.symbol == NUMBER){
        string num = sTable.searchNt(word.loc);
        string tmp = "0";

        //0+string,pushObjectStack
        _action.pushObjectStack(tmp+num);

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

//<加法运算符>
void Grammar::AddOperator(){
    if(word.symbol == PLUS){
        NEXT;
        //string，pushOpeStack
        _action.pushOpeStack("+");
        return;
    }
    else if(word.symbol == MINUS){
        NEXT;
        //string，pushOpeStack
        _action.pushOpeStack("-");
        return;
    }
    else{
        ERROR('!');
    }
}

//<乘法运算符>
void Grammar::MulOperator(){
    if(word.symbol == ASTERISK){
        NEXT;
        //string，pushOpeStack
        _action.pushOpeStack("*");
        return;
    }
    else if(word.symbol == SLASH){
        NEXT;
        //string，pushOpeStack
        _action.pushOpeStack("/");
        return;
    }
    else{
        ERROR('!');
    }
}

//<关系运算符>
void Grammar::BoolOperator(){
    if(word.symbol == LESS){
        NEXT;
        _action.pushOpeStack("<");
        return;
    }
    else if(word.symbol == LESS_EQUAL){
        NEXT;
        _action.pushOpeStack("<=");
        return;
    }
    else if(word.symbol ==GREATER){
        NEXT;
        _action.pushOpeStack(">");
        return;
    }
    else if(word.symbol == GREATER_EQUAL){
        NEXT;
        _action.pushOpeStack(">=");
        return;
    }
    else if(word.symbol == EQUAL){
        NEXT;
        _action.pushOpeStack("=");
        return;
    }
    else if(word.symbol == INEQUAL){
        NEXT;
        _action.pushOpeStack("<>");
        return;
    }
    else{
        ERROR('!');
    }
}