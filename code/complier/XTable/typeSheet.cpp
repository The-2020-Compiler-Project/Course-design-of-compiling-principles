//
// Created by YCJ on 2020/7/1.
//

#include "typeSheet.h"

typeSheet::typeSheet() {
    typeNode tmp{};
    for(int i=0;i<3;i++) {
        tmp.tVal = typeVal(i);
        tmp.len = typeLen[i];
        tmp.tPoint = nullptr;
        tmp.name = typeName[i];
        record.push_back(tmp);
    }
}



typeSheet::ull typeSheet::hash(const string &s) {
    //计算哈希值,溢出取模
    ull ans = 0;
    ull prime = 1;
    for (char i : s) {
        ans += i * prime;
        prime *= hPrime;
    }
    return ans;
}

typeSheet::iterator  typeSheet::find(const string &name) {
    for (auto it=record.begin();it!=record.end();it++) {
        if (it->name == name) {
            return it;
        }
    }
    return record.end();
}

typeSheet::iterator typeSheet::end() {
    return record.end();
}
