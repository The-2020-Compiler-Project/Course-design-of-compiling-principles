#pragma once
//
// 数据类型定义 Created by YCJ on 2020/7/1.
//
#include <bits/stdc++.h>
using namespace std;
//i(整型)，r(实型)，catC(字符型)，b(布尔型)|| a(数组型)，catD(结构型)
//类型定义
const int baseNum = 4;//基本类型的数量
enum TYPEVAL { typeValI, typeValR, typeValC, typeValB, typeValA, typeValD };
const int typeLen[baseNum] = { 2,6,1,1 };//一一匹配
const string typeName[baseNum] = { "inteter","real","char","bool" };//与关键字名一一匹配
const int iLen = 2;
const int rLen = 6;
const int cLen = 1;
const int bLen = 1; 
