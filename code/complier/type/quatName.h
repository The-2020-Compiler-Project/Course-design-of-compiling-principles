//
// Created by Lenovo on 2020/7/6.
//

#ifndef COMPLIER_QUATNAME_H
#define COMPLIER_QUATNAME_H

#include <string>
using namespace std;

//命名规范
//多个单词采取驼峰式命名法，即xxxYyyyZzzzTttt，如beginFunction
//单个单词为开头大写，如Else
//缩写单词为全大写，如ADD，MUL
namespace quatName{
    const string beginProgram="beginProgram";
    const string beginFunction="beginFunction";
    const string endFunction="endFunction";
    const string Assign="Assign";
    const string getAddress="getAddress";
    const string getSonAddres="getSonAddress";
    const string assignResult="assignResult";
    const string beginIf="beginIf";
    const string Else="Else";
    const string endIf="endIf";
    const string LT="LT";
    const string LE="LE";
    const string GT="GT";
    const string GE="GE";
    const string EQ="EQ";
    const string NEQ="NEQ";///新增加的不等于ha号
    const string beginWhile="beginWhile";
    const string Do="Do";
    const string endWhile="endWhile";
    const string moveTurePar="moveTurePar";
    const string moveFalsePar="moveFalsePar";
    const string Call="Call";
    const string getResult="getResult";
    const string ADD="ADD";
    const string SUB="SUB";
    const string MUL="MUL";
    const string DIV="DIV";
    const string Input="input";
    const string Output="output";
}
#endif //COMPLIER_QUATNAME_H
