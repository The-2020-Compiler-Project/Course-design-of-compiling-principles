//
// 数据类型定义 Created by YCJ on 2020/7/1.
//

#ifndef COMPLIER_TYPE_H
#define COMPLIER_TYPE_H

#include <bits/stdc++.h>
using namespace std;
//基本类型定义头文件
//i(整型)，r(实型)，c(字符型)，b(布尔型)|| a(数组型)，d(结构型)

const int baseTypeNum=4;//基本类型的数量
enum TYPEVAL{typeValI,typeValR,typeValC,typeValB,typeValA,typeValD};
const int baseTypeLen[baseTypeNum]={2,6,1,1};//一一匹配
const string baseTypeName[baseTypeNum]={"integer","real","char","bool"};//与关键字名一一匹配
const int iLen=2;
const int rLen=6;
const int cLen=1;
const int bLen=1;
#endif //COMPLIER_TYPE_H
