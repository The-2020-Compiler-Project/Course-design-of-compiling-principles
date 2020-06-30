//
// Created by YCJ on 2020/6/30.
//

#ifndef COMPLIER_STABLE_H
#define COMPLIER_STABLE_H

#include "..\tokenType\tokenType.h"
#include "Sheet.h"
#include <bits/stdc++.h>

using namespace std;
class STable{
    const ull Lprime = 769, Sprime = 128, Iprime = 17;
    Sheet ktSheet,ptSheet,itSheet,ctSheet,stSheet;
public:
    STable():ktSheet(Lprime),ptSheet(Sprime),itSheet(Lprime),ctSheet(Sprime),stSheet(Lprime)
    {}
};
extern STable sTable;




#endif //COMPLIER_STABLE_H
