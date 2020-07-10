//
// Created by YCJ on 2020/6/30.
//

#ifndef COMPLIER_FUNSHEET_H
#define COMPLIER_FUNSHEET_H

#include "TypeSheet.h"
#include "..\type\cat.h"
#include "ElemSheet.h"
#include <bits/stdc++.h>

using namespace std;
typedef unsigned long long ull;

class FunSheet {
public:
    struct iterator;
private:
    struct funNode;

    class funNode {
        friend FunSheet;
        string name;
        int level;
        CAT cat;
        //int len;//数据区长度实时计算的方式获取
        int parameterNum = 0;//参数个数
        string addr;//入口地址
        TypeSheet::typePoint type;
        ElemSheet elemSheet;//变量信息表
        ElemSheet tmpSheet;//临时变量表
        ElemSheet parmSheet;//参数信息表
        unordered_map<string, pair<TypeSheet::typePoint, string>> constSheet;//常量表，只支持字面值常量
        TypeSheet typeSheet;//类型信息表
        unordered_map<string, funNode *> sFunTable;//子函数表,表面上看上去是使用了未定义的类本身，实际上是个指针
        funNode *pFunPoint;//父函数地址
    public://嵌套类只对父类公开
        bool isTmpVariable(const string &tmpName);//本地判断是否为临时变量
        int len();
        ///查询部分
        pair<CAT, void *> find(const string &findName);//全局搜索，查找一个名字为name的标识符,cat用来判断如何使用返回的指针
        pair<CAT, void *> search(const string &searchName);//全局搜索，查找一个名字为name的标识符,cat用来判断如何使用返回的指针
        int getLevel(const string&searchName);///全局查找一个标识符的层次号
        ///插入部分
        funNode *addFun(const string &name, CAT cat, const string &type);//增加子函数
        pair<bool, ElemSheet::ElemPoint>
        addParameter(const string &addName, CAT addCat, const string &addType);//增加参数返回成功与否以及指针
        pair<bool, ElemSheet::ElemPoint> addVariable(const string &addName, const string &addType);//增加变量，返回成功以及指针
        pair<bool, ElemSheet::ElemPoint> addTmpVariable(const string &addName, const string &addType,CAT addCat=CAT::catV);//增加临时变量，返回成功以及指针
        bool addConst(const string &addName, const string &addType, const string &addValue);//增加常量，给定名字，类型，常量值，只支持基本类型常量
        pair<bool,TypeSheet::typePoint > addArrType(const string&arrName,const string&typeName,const int&size);//增加数组类型
        pair<bool,TypeSheet::typePoint > addStructType(const string&structName,vector<string>&sonName,vector<string>&sonType);//增加结构体类型
        ///删除部分
        bool eraseTmpVariable(ElemSheet::iterator eraseIterator);//删除临时变量
        void setOffSet(int beginOffSet);//设定偏移量
    };
    typedef funNode* funPoint;
    funNode *root;//根节点
public:
    ///提供给用户的函数接口
    struct iterator{
        friend FunSheet;
    private:
        funNode* root;//迭代器的底层为指针
    public:
        ///函数自身信息获取接口
        string name();//函数名字
        int level();//函数层次号
        CAT cat();//函数语义角色
        int len();//函数数据区长度
        int parameterNum();//函数参数个数
        ///判断临时变量
        bool isTmpVariable(const string&tmpName);///本地判断，是否为其他类型标识符无法判断出来
        ///定义性部分
        iterator addFunction(const string&name,CAT cat,const string& type="");///增加一个函数/过程
        void addParameter(const string &addName, CAT addCat, const string &addType) ;///增加一个参数
        void addConst(const string &addName, const string &addType,const string &addValue);///增加一个常量
        void addVariable(const string &addName, const string &addType);///增加一个变量
        void addTmpVariable(const string &addName, const string &addType,CAT addCat=CAT::catV);///增加一个临时变量
        void addArrType(const string&arrName,const string&typeName,const int&size);///增加数组类型
        void addStructType(const string&structName,vector<string>&sonName,vector<string>&sonType);///增加结构体类型
        ///查询部分
        pair<bool,CAT> find(const string&findName);///本地查询一个标识符，获取是否定义以及语义角色
        pair<bool,CAT> search(const string&searchName);///全局查找一个标识符，获取是否定义以及语义角色
        int getLevel(const string&searchName);///全局查找一个标识符的层次号
        ///迭代器获取部分
        iterator getFunIterator(const string&funName);///获取函数迭代器,全局性质
        ElemSheet::iterator getElemIterator(const string&elemNaem);///获取变量、参数、临时变量的迭代器，全局性质
        TypeSheet::iterator getTypeIterator(const string&typeName);///全局获取类型的迭代器，全局性质
        ElemSheet::iterator beginParameter();///获取所有参数的迭代器
        ///常量信息获取
        pair<TypeSheet::iterator, string> searchConstInfo(const string &constName);///获取常量的类型，常量值
        ///删除部分
        bool eraseTmpVariable(ElemSheet::iterator eraseIterator);///删除临时变量
        ///迭代器相关操作
        iterator operator--();///移动到直接外层
        bool useful();///检测迭代器是否为空
        ///结束性操作
        void setOffSet(int beginOffSet);//设定偏移量
        explicit iterator(funNode*point=nullptr);
        ///高级接口区域
        bool isCallFun(const string& name);///判断一个函数是否为自身可调用
    };
public:
    iterator creatMain(const string& name);//创建主函数
    iterator getMain();//获取主函数的迭代器
    void output(const string&fileName);
};

extern FunSheet xTable;

#endif //COMPLIER_FUNSHEET_H
