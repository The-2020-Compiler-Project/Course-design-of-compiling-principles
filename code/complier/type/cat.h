//
// Created by Lenovo on 2020/7/2.
//

#ifndef COMPLIER_CAT_H
#define COMPLIER_CAT_H
//标识符类语义角色
//catVn:换名形参
//catVf:赋值形参
///catT!!!!!!!!!!!!!!因符号表设计者的脑瘫，这个语义角色被废弃了
//catD,类型标识符
//catC,常量
//catV,变量
//catF，函数
//catP,过程
//catM,主函数
//catVoid内部特殊标记使用，外部接口不会出现此类型
enum CAT{catVn,catVf,catT,catD,catC,catV,catF,catP,catM,catVoid};
#endif //COMPLIER_CAT_H
