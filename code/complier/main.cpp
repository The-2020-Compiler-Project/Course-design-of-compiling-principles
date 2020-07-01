#include <iostream>
#include "tokenType\tokenType.h"
#include "STable\STable.h"
#include ".\XTable\XTable.h"
int main() {
    //主函数可用于测试，随意使用
    //revert_test2
    XTable test;
    auto it =test.findType("bool");
    if(it!=test.typeEnd()){
        cout<<it->name<<endl;
    }
    string s="my name is YCJ";
    sTable.insertIt("my name is YCJ");
    sTable.insertIt("my name");
    sTable.inserRt(3.14);
    sTable.inserRt(6.28);
    cout<<sTable.findRt(6.28)<<endl;
    cout<<sTable.findKt("integer");
    return 0;
}