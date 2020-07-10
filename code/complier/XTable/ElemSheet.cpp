//
// Created by YCJ on 2020/7/2.
//

#include "ElemSheet.h"

int ElemSheet::initOffSet(int beginOffSet) {
    for(auto it=record.begin();it!=record.end();it++){
        it->offSet=beginOffSet;
        ///换名形参确定
        if(CAT::catVn != it->cat&&CAT::catPoint!=it->cat)
            beginOffSet+=TypeSheet::iterator(it->typePoint).len();
        else beginOffSet+=2;
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
    addNode.name = addName;
    addNode.cat = addCat;
    addNode.typePoint = addType;
    ///维持双向链表
    addNode.next = nullptr;
    ElemPoint preNode = nullptr;
    if (this->record.empty()) {
        //链表为空
        addNode.pre = nullptr;
    } else {
        //链表不为空
        preNode = &(this->record.back());
        addNode.pre = preNode;
    }
    this->record.push_back(addNode);
    this->shine[addName] = (--this->record.end());
    ///变量长度的确定不能仅通过类型来判断，还需要语义角色
    if (addCat == CAT::catPoint || addCat == CAT::catVn)
        this->sheetLen += 2;
    else
        this->sheetLen += TypeSheet::iterator(addType).len();
    ElemPoint ansPoint = &(this->record.back());//通过指针
    ///维持双向链表
    if (preNode) {
        preNode->next = &(this->record.back());
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
        cerr<<"ElemSheet::erase::An attempt was made to delete an invalid item"<<endl;
        return false;
    }else{
        auto it=shine.find(erasePoint->name);
        if(it==shine.end()){
            cerr<<"ElemSheet::erase::An attempt was made to delete an item that does not belong to this member information table"<<endl;
            return false;
        }
        ///删除，并维护链表、双向指针、哈希表
        if(erasePoint->pre!= nullptr)
            erasePoint->pre->next=erasePoint->next;
        if(erasePoint->next!= nullptr)
            erasePoint->next->pre=erasePoint->pre;
        if(erasePoint->cat==CAT::catPoint||erasePoint->cat==CAT::catVn)
            this->sheetLen-=2;
        else
            this->sheetLen-=TypeSheet::iterator(erasePoint->typePoint).len();
        record.erase(it->second);
        shine.erase(it);
        return true;
    }
}

bool ElemSheet::erase(ElemSheet::iterator deletIterator) {
    return this->erase(deletIterator.root);
}

void ElemSheet::output(ofstream &fout) {
    for(auto it=this->record.begin();it!=record.end();it++){
        fout<<it->name<<' '<<it->offSet<<' '<<it->cat<<' '<<TypeSheet::iterator(it->typePoint).name()<<endl;
    }
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
        cerr<<"ElemSheet::iterator::offSet::Offset not initialized"<<endl;
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
