//
// Created by Lenovo on 2020/7/2.
//

#include "TmpSheet.h"


bool TmpSheet::find(const string& name) {
    return !(record.find(name)==record.end());
}
