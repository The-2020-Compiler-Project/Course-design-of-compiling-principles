#pragma once
//
// �������Ͷ��� Created by YCJ on 2020/7/1.
//
#include <bits/stdc++.h>
using namespace std;
//i(����)��r(ʵ��)��catC(�ַ���)��b(������)|| a(������)��catD(�ṹ��)
//���Ͷ���
const int baseNum = 4;//�������͵�����
enum TYPEVAL { typeValI, typeValR, typeValC, typeValB, typeValA, typeValD };
const int typeLen[baseNum] = { 2,6,1,1 };//һһƥ��
const string typeName[baseNum] = { "inteter","real","char","bool" };//��ؼ�����һһƥ��
const int iLen = 2;
const int rLen = 6;
const int cLen = 1;
const int bLen = 1;