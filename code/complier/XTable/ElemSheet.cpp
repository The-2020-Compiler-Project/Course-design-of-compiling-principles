//
// Created by Lenovo on 2020/7/2.
//

#include "ElemSheet.h"

bool ElemSheet::find(const string& name) {
    return !(record.end() == record.find(name));
}
