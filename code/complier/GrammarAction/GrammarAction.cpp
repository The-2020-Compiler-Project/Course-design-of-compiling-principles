//
// Created by YCJ on 2020/7/6.
//

#include "GrammarAction.h"
GrammarAction::GrammarAction(const string&fileName) {
    this->fin.open(fileName);
}

void GrammarAction::genetQuat(const string& a, const string& b, const string& c, const string& d) {
    fin<<begin<<a<<this->separator<<b<<this->separator<<c<<this->separator<<d<<end<<endl;
}

void GrammarAction::pushFunStack(FunSheet::iterator pushIterator) {
    this->funStack.push(pushIterator);
}

void GrammarAction::beginProgram() {
    genetQuat(quatName::beginProgram,this->funStack.top().name(),empty,empty);
}

void GrammarAction::beginFunction() {
    this->genetQuat(quatName::beginFunction,empty,empty,empty);
}

void GrammarAction::endFunction() {
    this->genetQuat(quatName::endFunction,empty,empty,empty);
    this->funStack.pop();
}

void GrammarAction::Assign() {
    string b=this->object.top();
    this->object.pop();
    string a=this->object.top();
    this->object.pop();
    genetQuat(quatName::Assign,b,empty,a);
}

void GrammarAction::pushObjectStack(const string &objectName) {
    this->object.push(objectName);
}

void GrammarAction::getAddress() {
    string b=this->object.top();
    this->object.pop();
    string a=this->object.top();
    this->object.pop();
    string tmpName=getTmpName("Point");
    this->funStack.top().addTmpVariable(tmpName,"integer",CAT::catPoint);
    this->object.push(tmpName);
    genetQuat(quatName::getAddress,a,b,tmpName);
}

string GrammarAction::getTmpName(string Cat) {
    string ans;
    ans+='_';
    ans+=to_string(tmpIndex);
    ans+=Cat;
    tmpIndex++;
    return ans;
}

void GrammarAction::getSonAddress() {
    ///生成任何变量都必须加入运算对象栈、符号表
    string b=this->object.top();
    this->object.pop();
    string a=this->object.top();
    this->object.pop();
    string tmpName=getTmpName("StructPoint");
    this->funStack.top().addTmpVariable(tmpName,"integer",CAT::catPoint);
    this->object.push(tmpName);
    genetQuat(quatName::getSonAddres,a,b,tmpName);
}

void GrammarAction::assignResult() {
    string a=this->object.top();
    this->object.pop();
    this->genetQuat(quatName::assignResult,a,empty,empty);
}

void GrammarAction::beginIf() {
    string a=this->object.top();
    this->object.pop();
    this->genetQuat(quatName::beginIf,a,empty,empty);
}

void GrammarAction::Else() {
    this->genetQuat(quatName::Else,empty,empty,empty);
}

void GrammarAction::endIf() {
    this->genetQuat(quatName::endIf,empty,empty,empty);
}

void GrammarAction::pushOpeStack(const string &opeName) {
    this->ope.push(opeName);
}

void GrammarAction::Relation() {
    string rela=this->ope.top();
    this->ope.pop();
    string b=this->object.top();
    this->object.pop();
    string a=this->object.top();
    this->object.pop();
    string tmpName=getTmpName("Relation");
    this->funStack.top().addTmpVariable(tmpName,"integer",CAT::catV);
    this->object.push(tmpName);

    if(rela=="<"){
        rela=quatName::LT;
    }
    else if(rela=="<="){
        rela=quatName::LE;
    }else if(rela==">"){
        rela=quatName::GT;
    }else if(rela==">="){
        rela=quatName::GE;
    }else if(rela=="=") {
        rela = quatName::EQ;
    }else if(rela=="<>"){
        rela = quatName::NEQ;
    }
    else{
        cerr<<"Undefined relational operator"<<endl;
    }
    this->genetQuat(rela,a,b,tmpName);
}

void GrammarAction::beginWhile() {
    this->genetQuat(quatName::beginWhile,empty,empty,empty);
}

void GrammarAction::Do() {
    string a=this->object.top();
    this->object.pop();
    this->genetQuat(quatName::Do,a,empty,empty);
}

void GrammarAction::endWhile() {
    this->genetQuat(quatName::endWhile,empty,empty,empty);
}

void GrammarAction::popObjectStack() {
    this->object.pop();
}

void GrammarAction::pushFunCallStack(const string &funName) {
    this->funCall.push(funName);
}

    void GrammarAction::moveParameter() {
    ///增加参数类型匹配
    auto it =this->funStack.top().getFunIterator(this->funCall.top());
    stack<string>paraName;//实际参数列表
    int num=it.parameterNum();
    for(int i=0;i<num;i++){
        paraName.push(this->object.top());
        this->object.pop();
    }
    auto paraIteraotor=it.beginParameter();
    while(num--){
        if(this->funStack.top().getElemIterator(paraName.top()).type().name()!=paraIteraotor.type().name()){
            cerr<<"Parameter type mismatch::"<<it.name()<<endl;
            exit(-1);
        }
        if(paraIteraotor.cat()==CAT::catVn){//换名形参
            this->genetQuat(quatName::moveFalsePar,paraName.top(),empty,empty);
        }else{//赋值形参
            this->genetQuat(quatName::moveTurePar,paraName.top(),empty,empty);
        }
        ++paraIteraotor;
        paraName.pop();
    }
}

void GrammarAction::Call() {
    this->genetQuat(quatName::Call,this->funCall.top(),empty,empty);
}

void GrammarAction::getResult() {
    ///增加临时变量，表示函数返回值
    string tmpName;
    tmpName=this->getTmpName("Return");
    this->funStack.top().addTmpVariable(tmpName,"integer",CAT::catV);
    this->object.push(tmpName);
    this->genetQuat(quatName::getResult,empty,empty,tmpName);
}

void GrammarAction::popFunCallStack() {
    this->funCall.pop();
}

void GrammarAction::Arithmetic() {
    string rela=this->ope.top();
    this->ope.pop();

    string b=this->object.top();
    this->object.pop();

    string a=this->object.top();
    this->object.pop();

    string tmpName=getTmpName("Arithm");

    this->funStack.top().addTmpVariable(tmpName,"integer",CAT::catV);
    this->object.push(tmpName);

    if(rela=="+"){
        rela=quatName::ADD;
    }
    else if(rela=="-"){
        rela=quatName::SUB;
    }else if(rela=="*"){
        rela=quatName::MUL;
    }else if(rela=="/"){
        rela=quatName::DIV;
    }else{
        cerr<<"Undefined arithmetic operator"<<endl;
    }
    this->genetQuat(rela,a,b,tmpName);
}


