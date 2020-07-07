//
// Created by YCJ on 2020/6/30.
//

#include "STable.h"
STable sTable;

int STable::insertIt(const string& name) {
    return itSheet.insert(name);
}

int STable::insertCt(const string& name) {
    return ctSheet.insert(name);
}

int STable::insertSt(const string& name) {
    return stSheet.insert(name);
}

int STable::insertNt(int num) {
    if(ntSheet.find(num)!=ntSheet.end()){
        return -1;
    }
    else{
        int ans=(ntSheet[num]=ntRSheet.size());
        ntRSheet.push_back(num);
        return ans;
    }
}

int STable::inserRt(double num) {
    if(rtSheet.find(num)!=rtSheet.end()){
        return -1;
    }
    else{
        int ans=(rtSheet[num]=rtRSheet.size());
        rtRSheet.push_back(num);
        return ans;
    }
}

int STable::findIt(const string& name) {
    return itSheet.find(name);
}

int STable::findKt(const string& name) {
    return ktSheet.find(name);
}

int STable::findPt(const string& name) {
    return ptSheet.find(name);
}

int STable::findCt(const string& c) {
    return ctSheet.find(c);
}

int STable::findSt(const string& s) {
    return stSheet.find(s);
}

int STable::findNt(int num) {
    if(ntSheet.find(num)!=ntSheet.end()){
        return ntSheet[num];
    }
    else return -1;
}

int STable::findRt(double num) {
    if(rtSheet.find(num)!=rtSheet.end()){
        return rtSheet[num];
    }
    else return -1;
}

string STable::searchKt(int index) {
    return ktSheet.search(index);
}

string STable::searchPt(int index) {
    return ptSheet.search(index);
}

string STable::searchIt(int index) {
    return itSheet.search(index);
}

void STable::init(const string& ktName, const string& ptName) {
    ktSheet.read(ktName);
    ptSheet.read(ptName);
}

string STable::searchNt(int index) {
    return to_string(ntRSheet[index]);
}

string STable::searchCt(int index) {
    return ctSheet.search(index);
}


