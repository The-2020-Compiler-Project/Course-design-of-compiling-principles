//
// Created by YCJ on 2020/7/1.
//

#include "TypeSheet.h"
///类型表的初始化
TypeSheet::TypeSheet() {
//从type.h文件中读取定义的基本类型以及名字，tval值
    TypeSheet::TypeNode tmp;
    for(int i=0;i<baseTypeNum;i++){
      tmp.name=baseTypeName[i];
      tmp.len=baseTypeLen[i];
      tmp.structSheet=nullptr;
      tmp.arrSheet=nullptr;
      tmp.tVal=TYPEVAL(i);
      this->record[tmp.name]=tmp;
    }
}

TypeSheet::typePoint TypeSheet::find(const string &findName) {
    if(record.find(findName)!=record.end()){
        return &record[findName];
    }
    else return nullptr;
}

int TypeSheet::iterator::len() {
    return this->root->len;
}

TYPEVAL TypeSheet::iterator::tVal() {
    return this->root->tVal;
}

string TypeSheet::iterator::name() {
    return this->root->name;
}


