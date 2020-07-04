
#include "tokenType\tokenType.h"
#include "STable\STable.h"
#include ".\XTable\FunSheet.h"
#include <bits/stdc++.h>

using namespace std;
int main() {
    auto it =xTable.creatMain("main");
    it.addConst("a","integer","123");
    it.addConst("b","integer","12");
    it.addConst("c","integer","1");
    auto ans=it.searchConstInfo("b");
    cout<<ans.second<<endl;
    cout<<ans.first.name()<<endl;
    auto son=it.addFunction("test",CAT::catP);
    cout<<son.name()<<endl;
    cout<<son.level()<<endl;
    cout<<son.cat()<<endl;
    son.addTmpVariable("a","integer");
    son.addTmpVariable("b","integer");
    son.addTmpVariable("c","integer");
    son.addVariable("d","bool");
    cout<<son.len()<<endl;
    son.eraseTmpVariable(son.getElemIterator("b"));
    cout<<son.len()<<endl;
    auto sontest=son.getElemIterator("b");
    if(sontest.useful()){
        cout<<"yes"<<endl;
    }else{
        cout<<"no"<<endl;
    }
    son.addParameter("p1",CAT::catVn,"integer");
    son.addParameter("p2",CAT::catVf,"integer");
    cout<<son.parameterNum()<<endl;
    --son;
    cout<<son.name()<<endl;
    cout<<son.level()<<endl;
    cout<<son.cat()<<endl;
    return 0;
}