//
// Created by YCJ on 2020/7/1.
//

#include "TypeSheet.h"
///类型表的初始化
TypeSheet::TypeSheet() {
//从type.h文件中读取定义的基本类型以及名字，tval值
    for(int i=0;i<baseTypeNum;i++) {
        TypeNode *point = new(TypeNode);
        TypeSheet::TypeNode &tmp = *point;
        tmp.name = baseTypeName[i];
        tmp.len = baseTypeLen[i];
        tmp.structSheet = nullptr;
        tmp.arrSheet = nullptr;
        tmp.tVal = TYPEVAL(i);
        this->record[tmp.name] = point;
    }
}

TypeSheet::typePoint TypeSheet::find(const string &findName) {
    if(record.find(findName)!=record.end()){
        return record[findName];
    }
    else return nullptr;
}

TypeSheet::typePoint TypeSheet::addArrType(const string &arrName, TypeNode* typePpint, const int &size) {
    TypeNode *addNode=new(TypeNode);
    addNode->tVal=TYPEVAL::typeValA;
    addNode->name=arrName;
    addNode->len=size*(typePpint->len);
    addNode->arrSheet=new(TypeNode::ArrNode);
    addNode->arrSheet->low=0;
    addNode->arrSheet->up=size-1;///size所指示的是数组的大小，上届比它小1
    addNode->arrSheet->arrType=typePpint;
    addNode->structSheet= nullptr;
    ///添加到哈希表中
    record.insert(make_pair(arrName,addNode));
    return addNode;
}

TypeSheet::typePoint
TypeSheet::addStructType(const string &structName, vector<string> &sonName, vector<TypeNode*> &sonType) {
    TypeNode *addNode=new(TypeNode);
    addNode->tVal=TYPEVAL::typeValD;
    addNode->name=structName;
    addNode->len=0;
    addNode->arrSheet=nullptr;
    addNode->structSheet=new(TypeNode::StructSheet);
    ///增加域名定义,不进行安全性检查
    auto itType=sonType.begin();
    ///域名的偏移量确定
    int offset=0;
    for(auto it=sonName.begin();it!=sonName.end();it++,itType++) {
        addNode->len+=(*itType)->len;
        addNode->structSheet->record.insert(make_pair(*it, *itType));
        addNode->structSheet->offset[*it]=offset;
        offset+=(*itType)->len;
    }
    ///添加到哈希表中
    record.insert(make_pair(structName,addNode));
    return addNode;
}

void TypeSheet::output(ofstream &fout) {
    for(auto it=this->record.begin();it!=this->record.end();it++){
        fout<<it->second->name<<' '<<it->second->len<<endl;
        if(it->second->arrSheet){
            fout<<"     arr  len:"<<it->second->arrSheet->up<<"  type:"<<it->second->arrSheet->arrType->name<<endl;
        }else if(it->second->structSheet){
            fout<<"     struct:";
            for(auto son=it->second->structSheet->record.begin();son!=it->second->structSheet->record.end();son++){
                fout<<son->first<<" ";
            }
            fout<<endl;
        }
    }
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

TypeSheet::iterator TypeSheet::iterator::arrType() {
    if(this->root->arrSheet== nullptr){
        cerr<<"TypeSheet::iterator::Non array type attempted to get array type information"<<endl;
        return TypeSheet::iterator(nullptr);
    }
    return TypeSheet::iterator(this->root->arrSheet->arrType);
}

TypeSheet::iterator TypeSheet::iterator::structType(const string &elemName) {
    if(this->root->structSheet== nullptr){
        cerr<<"TypeSheet::iterator::An unstructured type attempted to obtain structure domain name type information"<<endl;
        return TypeSheet::iterator(nullptr);
    }
    if(this->root->structSheet->record.find(elemName)==this->root->structSheet->record.end() ){
        cerr<<"TypeSheet::iterator::Structure attempts to obtain type information of illegal domain name"<<endl;
        return TypeSheet::iterator(nullptr);
    }
    return TypeSheet::iterator(this->root->structSheet->record[elemName]);
}

int TypeSheet::iterator::arrSize() {
    if(this->root->tVal==TYPEVAL::typeValA){
        return this->root->len;
    }
    else return -1;
}

int TypeSheet::iterator::sonOffSet(const string &elemName) {
    if(this->root->structSheet== nullptr){
        cerr<<"TypeSheet::iterator::sonOffSet::Nonstructural type attempted to get structure domain name type offset"<<endl;
        return -1;
    }
    if(this->root->structSheet->record.find(elemName)==this->root->structSheet->record.end() ){
        cerr<<"TypeSheet::iterator::Structure attempted to get the offset of illegal domain name"<<endl;
        return -1;
    }
    return (this->root->structSheet->offset[elemName]);
}

bool TypeSheet::iterator::useful() {
    return this->root != nullptr;
}
