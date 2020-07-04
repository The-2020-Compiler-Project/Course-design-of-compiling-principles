//
// 类型信息表 Created by YCJ on 2020/7/1.
//

#ifndef COMPLIER_TYPESHEET_H
#define COMPLIER_TYPESHEET_H


#include "..\type\type.h"
#include <bits/stdc++.h>

using namespace std;

class TypeSheet {
private:

    ///类型表结点定义
    class TypeNode {
        friend TypeSheet;
    private:
        ///数组表的定义
        struct ArrSheet {
            int low,up;
            TypeNode* arrType;
        };
        ///结构体表的定义
        struct StructSheet{
            unordered_map<string,TypeNode*>record;
        };

    private:
        TYPEVAL tVal;//类型值
        string name;//类型的名字，对自定义名字有一定的作用
        int len=0;//类型的长度
        ArrSheet* arrSheet;//数组信息表指针
        StructSheet* structSheet;//结构体信息表指针
    };

    unordered_map<string,TypeNode>record;

public:
    typedef TypeNode* typePoint;
public:
    ///类型表迭代器的定义
    struct iterator {
    private:
        typePoint root;
    public:
        TYPEVAL tVal();
        string name();
        int len();
        iterator arrType();///获取数组元素的类型
        iterator structType(const string&elemName);///获取结构体元素的类型
        explicit iterator(typePoint root=nullptr):root(root){};
    };
public:
    typePoint find(const string&name);//查找类型，返回指针
    typePoint addType(TYPEVAL typeVal);

    TypeSheet();
};



#endif //COMPLIER_TYPESHEET_H
