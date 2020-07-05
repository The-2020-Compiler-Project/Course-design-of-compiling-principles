//
// Created by YCJ on 2020/6/30.
//

#include "FunSheet.h"

FunSheet xTable;

FunSheet::iterator FunSheet::creatMain(const string &addName) {
    //创建主函数program
    root=new(funNode);
    root->name=addName;
    root->level=0;
    root->cat=CAT::catM;
    root->parameterNum=0;
    ;//入口由目标程序填写
    root->type=nullptr;//不存在返回值
    root->pFunPoint=nullptr;//主函数没有父函数
    return iterator(root);
}

FunSheet::iterator::iterator(FunSheet::funNode *point) {
    root=point;
}

void FunSheet::iterator::addParameter(const string &addName, CAT addCat, const string &addType) {
    bool ans=this->root->addParameter(addName,addCat,addType).first;
    if(!ans){
        cerr<<"FunSheet::iterator::addParameter::添加参数失败"<<endl;
    }
}

void FunSheet::iterator::addVariable(const string &addName, const string &addType) {
    bool ans=this->root->addVariable(addName,addType).first;
    if(!ans){
        cerr<<"FunSheet::iterator::addVariable::添加变量失败"<<endl;
    }
}

void FunSheet::iterator::addConst(const string &addName, const string &addType, const string &addValue) {
    bool ans=this->root->addConst(addName,addType,addValue);
    if(ans==0){
        cerr<<"FunSheet::iterator::addConst::常量添加失败"<<endl;
    }
}

FunSheet::iterator FunSheet::iterator::addFunction(const string &name, CAT cat, const string &type) {
    auto ans=this->root->addFun(name,cat,type);
    if(ans== nullptr){
        cerr<<"FunSheet::iterator::addFunction::增加函数失败"<<endl;
    }
    return iterator(ans);
}

bool FunSheet::iterator::useful() {
    return this->root != nullptr;
}

string FunSheet::iterator::name() {
    return this->root->name;
}

int FunSheet::iterator::level() {
    return this->root->level;
}

CAT FunSheet::iterator::cat() {
    return this->root->cat;
}

int FunSheet::iterator::len() {
    return this->root->len();
}

int FunSheet::iterator::parameterNum() {
    return this->root->parameterNum;
}

pair<bool, CAT> FunSheet::iterator::find(const string &findName) {
    pair<bool, CAT> ans;
    auto findAns=this->root->find(findName);
    if(findAns.second== nullptr){
        ans.first= false;
        ans.second=CAT::catVoid;
    }else{
        ans.first=true;
        ans.second=findAns.first;
    }
    return ans;
}

pair<bool, CAT> FunSheet::iterator::search(const string &searchName) {
    pair<bool, CAT> ans;
    auto findAns=this->root->search(searchName);
    if(findAns.second== nullptr){
        ans.first= false;
        ans.second=CAT::catVoid;
    }else{
        ans.first=true;
        ans.second=findAns.first;
    }
    return ans;
}

FunSheet::iterator FunSheet::iterator::getFunIterator(const string &funName) {
    ///全局查询符号表寻找标识符
    auto findAns=this->root->search(funName);
    if(findAns.first!=CAT::catF&&findAns.first!=CAT::catP){
        ///标识符不是函数或者过程，返回空迭代器
        return iterator(nullptr);
    }else{
        return iterator((funPoint)findAns.second);
    }
}

ElemSheet::iterator FunSheet::iterator::getElemIterator(const string &elemNaem) {
    ///全局查询符号表寻找标识符
    auto findAns=this->root->search(elemNaem);
    if(findAns.first!=CAT::catV&&findAns.first!=CAT::catVn&&findAns.first!=CAT::catVf){
        ///标识符不是变量（临时变量）、形参、换名形参，返回空迭代器
        return ElemSheet::iterator(nullptr);
    }else{
        return ElemSheet::iterator((ElemSheet::ElemPoint)findAns.second);
    }
}

bool FunSheet::iterator::isTmpVariable(const string &tmpName) {
    return this->root->isTmpVariable(tmpName);
}

TypeSheet::iterator FunSheet::iterator::getTypeIterator(const string &typeName) {
    ///全局查询符号表寻找标识符
    auto searchAns=this->root->search(typeName);
    if(searchAns.first!=CAT::catD){
        ///标识符不是类型标识符
        return TypeSheet::iterator(nullptr);
    }else{
        return TypeSheet::iterator((TypeSheet::typePoint)searchAns.second);
    }
}

ElemSheet::iterator FunSheet::iterator::beginParameter() {
    return this->root->parmSheet.begin();
}

FunSheet::iterator FunSheet::iterator::operator--() {
    this->root=this->root->pFunPoint;
    return *this;
}

bool FunSheet::iterator::eraseTmpVariable(ElemSheet::iterator eraseIterator) {
    return this->root->eraseTmpVariable(eraseIterator);
}

pair<TypeSheet::iterator, string> FunSheet::iterator::searchConstInfo(const string &constName) {
    pair<TypeSheet::iterator, string> ans;
    ///安全性检查
    ///检查标识符是否已经定义（全局）
    auto safeAns=this->root->search(constName);
    if(safeAns.second== nullptr){
        cerr<<"FunSheet::iterator::searchConstInfo::不存在对应的标识符"<<endl;
        return ans;
    }
    else if(safeAns.first==CAT::catC){
        auto *p=(pair<TypeSheet::typePoint, string> *)(safeAns.second);
        ans.first=TypeSheet::iterator(p->first);
        ans.second=p->second;
        return ans;
    }
    else{
        cerr<<"FunSheet::iterator::searchConstInfo::标识符不是常量"<<endl;
        return ans;
    }
}

void FunSheet::iterator::addTmpVariable(const string &addName, const string &addType,CAT addCat) {
    bool ans=this->root->addTmpVariable(addName,addType,addCat).first;
    if(!ans){
        cerr<<"FunSheet::iterator::addTmpVariable::添加变量失败"<<endl;
    }
}

void FunSheet::iterator::addArrType(const string &arrName, const string &typeName, const int &size) {
    auto it=this->root->addArrType(arrName,typeName,size);
    if(!it.first){
        cerr<<"FunSheet::iterator::addArrType::数组类型定义错误"<<endl;
    }
}

void FunSheet::iterator::addStructType(const string &structName, vector<string> &sonName, vector<string> &sonType) {
    auto it =this->root->addStructType(structName,sonName,sonType);
    if(it.first==0){
        cerr<<"FunSheet::iterator::addStructType::结构体定义失败"<<endl;
    }
}

void FunSheet::iterator::setOffSet(int beginOffSet) {
    this->root->setOffSet(beginOffSet);
}

FunSheet::funNode* FunSheet::funNode::addFun(const string &addName, CAT addCat, const string& addType="") {
    ///安全性检查
    auto safeAns = this->find(addName);
    if (safeAns.second != nullptr) {
        cerr << "FunSheet::funNode::addFun::已经存在与函数/过程同名的标识符" << endl;
        return nullptr;
    } else {
        if (addCat != CAT::catF && addCat != CAT::catP) {
            cerr << "FunSheet::funNode::addFun::函数/过程语义角色错误" << endl;
            return nullptr;
        }
    }
    ///创建函数确定函数的名字，种类，类型,层次号,将参数个数初始化为0
    ///修正
    auto ptmp = new(funNode);
    funNode &tmp = *ptmp;

    tmp.name = addName;
    tmp.level = this->level + 1;
    tmp.parameterNum = 0;
    if (addCat == CAT::catF) {///函数
        tmp.cat = addCat;
        ///确定返回值的类型
        auto it = this->search(addType);
        if (it.first == CAT::catD) {///找到，且种类为类型标识符
            tmp.type = (TypeSheet::typePoint) it.second;
        } else {
            cerr << "FunSheet::funNode::addFun::企图使用未定义的类型标识符" << endl;
        }
    } else {///过程
        tmp.cat = addCat;
        if (!addType.empty()) {
            cerr << "FunSheet::funNode::addFun::企图使用给过程定义返回类型" << endl;
        }
        tmp.type = nullptr;///无类型
    }
    ///维护树的结构
    sFunTable[this->name] = ptmp;//向当前活动函数中增加新的子函数
    tmp.pFunPoint = this;
    //函数表具有的功能设定主函数
    //函数迭代器需要具有的功能：增加参数，设定类型，增加变量,设定入口地址，增加临时变量，
    //查看名字，层次号，种类，数据区长度，参数个数
    //获取类型信息、参数信息
    //查询标识符的种类
    return ptmp;
}

pair<CAT, void *> FunSheet::funNode::find(const string &findName) {
    //type用来标识如何使用返回的空指针，type=typeVoid时，表示未找到相应的标识符
    //返回值初始化
    pair<CAT, void *> ans;

    typedef void *voidPoint;
    voidPoint &ansPoint = ans.second;
    CAT &ansCat = ans.first;

    ansPoint = nullptr;
    ansCat = CAT::catVoid;

    //查找是否为本函数
    if (this->name == findName) {
        ansCat = this->cat;
        ansPoint = this;
        return ans;
    }
    //查找是否为为变量
    ans=this->elemSheet.find(findName);
    if(ans.second!= nullptr){
        return ans;
    }
    //查询是否为为临时变量
    ans=this->tmpSheet.find(findName);
    if(ans.second!= nullptr){
        return ans;
    }
    //查询是否为参数
    ans=this->parmSheet.find(findName);
    if(ans.second!= nullptr){
        return ans;
    }
    //查询是否为常量
    if(this->constSheet.find(findName)!=this->constSheet.end()){
        ans.first=CAT::catC;
        ans.second=&(this->constSheet[findName]);
        return ans;
    }
    //是否为类型标识符，包括基本类型标识符
    ansPoint=this->typeSheet.find(findName);
    if(ansPoint!=nullptr){
        ansCat=CAT::catD;
        return ans;
    }
    //查询是否为子函数
    if (sFunTable.find(findName) != sFunTable.end()) {
        ansCat = sFunTable[findName]->cat;
        ansPoint = sFunTable[findName];
        return ans;
    }
    return ans;
}

pair<CAT, void *> FunSheet::funNode::search(const string &searchName) {
    int times=this->level;
    funPoint pNode=this;
    pair<CAT, void *> ans;
    ans.first=CAT::catVoid;
    ans.second= nullptr;
    while(times>=0){
        ans=pNode->find(searchName);
        if(ans.second!= nullptr)
            return ans;
        pNode=pNode->pFunPoint;
        times--;
    }
    return ans;
}

pair<bool, ElemSheet::ElemPoint> FunSheet::funNode::addParameter(const string &addName, CAT addCat,const string&addType) {
    pair<bool, ElemSheet::ElemPoint> ans;
    ans.first=false;
    ans.second= nullptr;
    ///安全性检查
    auto safeAns=this->find(addName);
    ///检查标识符是否已经定义
    if(safeAns.second!= nullptr){
        cerr<<"FunSheet::funNode::addParameter::已经存在与参数同名的标识符"<<endl;
        return ans;
    }
    else{
        ///检查类型是否定义
        auto search1=this->search(addType);
        if(search1.first!=CAT::catD){
            cerr<<"FunSheet::funNode::addParameter::企图使用未定义的类型标识符"<<endl;
            return ans;
        }
        else{
            ///检查语义角色是否设置正确
            if(addCat!=CAT::catVn&&addCat!=CAT::catVf){
                cerr<<"FunSheet::funNode::addParameter::企图给参数使用其他语义种类"<<endl;
            }
            ans.second=this->parmSheet.add(addName,addCat,(TypeSheet::typePoint)search1.second);
            ans.first= true;
            this->parameterNum++;///参数添加成功，参数个数+1
            return ans;
        }
    }
}

pair<bool, ElemSheet::ElemPoint>
FunSheet::funNode::addVariable(const string &addName, const string &addType) {
    ///返回值初始化
    pair<bool, ElemSheet::ElemPoint> ans;
    ans.first=false;
    ans.second= nullptr;
    ///安全性检查
    auto safeAns=this->find(addName);
    ///检查标识符是否已经定义（局部）
    if(safeAns.second!= nullptr){
        cerr<<"FunSheet::funNode::addVariable::已经存在与变量同名的标识符"<<endl;
        return ans;
    }
    else{
        ///检查类型定义是否存在（全局）
        auto search1=this->search(addType);
        if(search1.first!=CAT::catD){
            cerr<<"FunSheet::funNode::addVariable::企图使用未定义的类型标识符"<<endl;
            return ans;
        }
        else{
            ///将变量加入变量表
            ans.second=this->elemSheet.add(addName,CAT::catV,(TypeSheet::typePoint)search1.second);
            ans.first= true;
            return ans;
        }
    }
}

bool FunSheet::funNode::addConst(const string &addName, const string &addType, const string &addValue) {
    ///安全性检查
    ///检查标识符是否已经定义（局部）
    auto safeAns=this->find(addName);
    if(safeAns.second!= nullptr){
        cerr<<"FunSheet::funNode::addConst::已经存在与常量同名的标识符"<<endl;
        return false;
    }
    else{
        ///检查类型定义是否存在（全局）
        auto search1=this->search(addType);
        if(search1.first!=CAT::catD){
            cerr<<"FunSheet::funNode::addConst::企图使用未定义的类型标识符"<<endl;
            return false;
        }
        else{
            ///将常量加入常量表
            this->constSheet[addName]=make_pair((TypeSheet::typePoint)search1.second,addValue);
            return true;
        }
    }
}

bool FunSheet::funNode::isTmpVariable(const string &tmpName) {
    auto ans=this->tmpSheet.find(tmpName);
    if(ans.second!= nullptr){
        return true;
    }else{
        return false;

    }
}

bool FunSheet::funNode::eraseTmpVariable(ElemSheet::iterator eraseIterator) {
    bool ans=this->tmpSheet.erase(eraseIterator);
    if(ans==0){
        cerr<<"FunSheet::funNode::eraseTmpVariable::临时变量删除错误"<<endl;
    }
    return ans;
}

pair<bool, ElemSheet::ElemPoint> FunSheet::funNode::addTmpVariable(const string &addName, const string &addType,CAT addCat) {
    ///返回值初始化
    pair<bool, ElemSheet::ElemPoint> ans;
    ans.first=false;
    ans.second= nullptr;
    ///安全性检查
    auto safeAns=this->find(addName);
    ///检查标识符是否已经定义（局部）
    if(safeAns.second!= nullptr){
        cerr<<"FunSheet::funNode::addTmpVariable::已经存在与临时变量同名的标识符"<<endl;
        return ans;
    }
    else{
        ///检查类型定义是否存在（全局）
        auto search1=this->search(addType);
        if(search1.first!=CAT::catD){
            cerr<<"FunSheet::funNode::addTmpVariable::企图使用未定义的类型标识符"<<endl;
            return ans;
        }
        else{
            ///将临时变量加入临时变量表
            ans.second=this->tmpSheet.add(addName,addCat,(TypeSheet::typePoint)search1.second);
            ans.first= true;
            return ans;
        }
    }
}

int FunSheet::funNode::len() {
    return this->parmSheet.len()+this->tmpSheet.len()+this->elemSheet.len();
}

pair<bool,TypeSheet::typePoint > FunSheet::funNode::addArrType(const string &arrName, const string &typeName, const int &size) {
    ///返回值初始化
    pair<bool,TypeSheet::typePoint>ans;
    ans.first=false;
    ans.second= nullptr;
    ///安全性检查
    auto safeAns=this->find(arrName);
    if(safeAns.second!= nullptr){
        cerr<<"FunSheet::funNode::addArrType::数组定义企图使用已用的本地标识符"<<endl;
        return ans;
    }
    auto typeSafeAns=this->search(typeName);
    if(typeSafeAns.first!=CAT::catD){
        cerr<<"FunSheet::funNode::addArrType::数组未定义的类型标识符定义数组"<<endl;
        return ans;
    }
    if(size<=0){
        cerr<<"FunSheet::funNode::addArrType::数组大小定义错误"<<endl;
        return ans;
    }
    ans.first=true;
    ans.second=this->typeSheet.addArrType(arrName,(TypeSheet::typePoint)typeSafeAns.second,size);
    return ans;
}

pair<bool, TypeSheet::typePoint>
FunSheet::funNode::addStructType(const string &structName, vector<string> &sonName, vector<string> &sonType) {
    ///返回值初始化
    pair<bool,TypeSheet::typePoint>ans;
    ans.first=false;
    ans.second= nullptr;
    ///安全性检查
    auto safeAns=this->find(structName);
    if(safeAns.second!= nullptr){
        cerr<<"FunSheet::funNode::addStructType::结构体定义企图使用已用的本地标识符"<<endl;
        return ans;
    }
    ///数目匹配检查
    if(sonName.size()!=sonType.size()){
        cerr<<"FunSheet::funNode::addStructType::结构体定义中域名个数与域名类型个数不匹配"<<endl;
        return ans;
    }
    ///类型定义检查
    vector<TypeSheet::typePoint >sonTypePoint;
    for(auto & typeName : sonType) {
        auto typeSafeAns = this->search(typeName);
        if (typeSafeAns.first != CAT::catD) {
            cerr << "FunSheet::funNode::addStructType::结构体定义企图使用未定义的全局类型标识符" << endl;
            return ans;
        }
        sonTypePoint.push_back((TypeSheet::typePoint)typeSafeAns.second);
    }
    ///域名重复检查
    unordered_map<string,bool>sameCheck;
    for(auto & it : sonName){
        if(sameCheck[it]){
            cerr << "FunSheet::funNode::addStructType::结构体定义企图使用相同的域名" << endl;
            return ans;
        }
        sameCheck[it]=true;
    }
    ans.first=true;
    ans.second=this->typeSheet.addStructType(structName,sonName,sonTypePoint);
    return ans;
}

void FunSheet::funNode::setOffSet(int beginOffSet) {
    int next=this->parmSheet.initOffSet(beginOffSet);
    next=this->elemSheet.initOffSet(next);
    this->tmpSheet.initOffSet(next);
}
