
#include "tokenType\tokenType.h"
#include "STable\STable.h"
#include ".\XTable\FunSheet.h"
#include <bits/stdc++.h>

using namespace std;
int main() {
    auto it =xTable.creatMain("main");
    it.addArrType("arr","integer",1000);
    auto type=it.getTypeIterator("arr");
    cout<<type.len()<<endl;


    vector<string>name;
    vector<string>sonType;
    name.push_back("b");
    sonType.push_back("arr");
    name.push_back("a");
    sonType.push_back("integer");

    it.addStructType("node",name,sonType);

    it.addParameter("tmp1",CAT::catVn,"bool");//0
    it.addParameter("tmp2",CAT::catVn,"arr");//2
    it.addParameter("tmp3",CAT::catVn,"node");//4
    it.addVariable("a","arr");//6
    it.addVariable("b","node");//2006
    it.addTmpVariable("xx","integer");//4008
    it.setOffSet(0);
    cout<<it.getElemIterator("tmp1").offSet()<<endl;
    cout<<it.getElemIterator("tmp2").offSet()<<endl;
    cout<<it.getElemIterator("tmp3").offSet()<<endl;
    cout<<it.getElemIterator("a").offSet()<<endl;
    cout<<it.getElemIterator("b").offSet()<<endl;
    cout<<it.getElemIterator("xx").offSet()<<endl;
    cout<<"########################"<<endl;
    cout<<it.getElemIterator("b").type().structType("b").len()<<endl;
    cout<<it.getElemIterator("b").type().sonOffSet("a")<<endl;
    return 0;
}