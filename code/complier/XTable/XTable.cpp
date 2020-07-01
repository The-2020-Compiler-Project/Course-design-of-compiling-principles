//
// Created by YCJ on 2020/6/30.
//

#include "XTable.h"

typeSheet::iterator XTable::findType(const string& name) {
    return typeTable.find(name);
}

typeSheet::iterator XTable::typeEnd() {
    return typeTable.end();
}
