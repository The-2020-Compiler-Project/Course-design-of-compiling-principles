//
// Created by Lenovo on 2020/7/2.
//

#include "ElemSheet.h"

int ElemSheet::initOffSet(int beginOffSet) {
    for(auto it=record.begin();it!=record.end();it++){
        it->offSet=beginOffSet;
        beginOffSet+=TypeSheet::iterator(it->typePoint).len();
    }
    return beginOffSet;
}

int ElemSheet::len() {
    return this->sheetLen;
}

pair<CAT, ElemSheet::ElemPoint> ElemSheet::find(const string &findName) {
    pair<CAT, ElemPoint> ans;
    ans.first = CAT::catVoid;
    ans.second = NULL;
    if (shine.find(findName) != shine.end()) {
        ans.second=&(*shine[findName]);//通过迭代器获取指针
        ans.first=ans.second->cat;
    }
    return ans;
}

ElemSheet::ElemPoint ElemSheet::add(const string &addName, CAT addCat, TypeSheet::typePoint addType) {
    //不进行安全性检查
    ElemNode addNode;
    addNode.name=addName;
    addNode.cat=addCat;
    addNode.typePoint=addType;
    ///维持双向链表
    addNode.next= nullptr;
    ElemPoint preNode= nullptr;
    if(this->record.empty()){
        //链表为空
        addNode.pre= nullptr;
    }else{
        //链表不为空
        preNode=&(this->record.back());
        addNode.pre=preNode;
    }
    this->record.push_back(addNode);
    this->shine[addName]=(--this->record.end());
    this->sheetLen+=TypeSheet::iterator(addType).len();
    ElemPoint ansPoint=&(this->record.back());//通过指针
    ///维持双向链表
    if(preNode){
        preNode->next=&(this->record.back());
    }
    return ansPoint;
}

ElemSheet::iterator ElemSheet::begin() {
    if(this->record.empty()){
        return iterator(nullptr);
    }
    return iterator(&(this->record.front()));
}

bool ElemSheet::erase(ElemSheet::ElemPoint erasePoint) {
    if(erasePoint== nullptr){
        cerr<<"ElemSheet::erase::试图删除无效项"<<endl;
        return false;
    }else{
        auto it=shine.find(erasePoint->name);
        if(it==shine.end()){
            cerr<<"ElemSheet::erase::试图删除不属于本成员信息表的项"<<endl;
            return false;
        }
        ///删除，并维护链表、双向指针、哈希表
        if(erasePoint->pre!= nullptr)
            erasePoint->pre->next=erasePoint->next;
        if(erasePoint->next!= nullptr)
            erasePoint->next->pre=erasePoint->pre;
        this->sheetLen-=TypeSheet::iterator(erasePoint->typePoint).len();
        record.erase(it->second);
        shine.erase(it);
        return true;
    }
}

bool ElemSheet::erase(ElemSheet::iterator deletIterator) {
    return this->erase(deletIterator.root);
}


ElemSheet::iterator ElemSheet::iterator::operator++() {
    this->root=this->root->next;
    return *this;
}

string ElemSheet::iterator::name() {
    return this->root->name;
}

int ElemSheet::iterator::offSet() {
    if(this->root->offSet==-1){
        cerr<<"ElemSheet::iterator::offSet::偏移量未初始化"<<endl;
    }
    return this->root->offSet;
}

TypeSheet::iterator ElemSheet::iterator::type() {
    return TypeSheet::iterator(this->root->typePoint);
}

CAT ElemSheet::iterator::cat() {
    return this->root->cat;
}

bool ElemSheet::iterator::useful() {
    if(this->root!= nullptr)return true;
}
