//
// Created by YCJ on 2020/7/1.
//

#include "TypeSheet.h"

TypeSheet::TypeSheet() {
    typeNode tmp{};
    for(int i=0;i<baseNum;i++) {
        tmp.tVal = TYPEVAL(i);
        tmp.len = typeLen[i];

        tmp.name = typeName[i];
        record.push_back(tmp);
    }
}



TypeSheet::ull TypeSheet::hash(const string &s) {
    //计算哈希值,溢出取模
    ull ans = 0;
    ull prime = 1;
    for (char i : s) {
        ans += i * prime;
        prime *= hPrime;
    }
    return ans;
}

TypeSheet::const_iterator  TypeSheet::find(const string &name) {
    for (auto it=record.begin();it!=record.end();it++) {
        if (it->name == name) {
            return it;
        }
    }
    return record.end();
}

TypeSheet::const_iterator TypeSheet::end() {
    return record.end();
}
