#include "Optimizer.h"
#include "../XTable/FunSheet.h"
bool inline blockend(string& s){
    return s =="beginProgram" || s== "beginFunction" ||s == "endFunction" ||s == "beginIf" || s == "Else" || s =="endIf" || s=="endProgram" ||
           s == "beginWhile" || s == "Do" || s == "endWhile" || s == "Call" ;
}

bool inline specialJudge(string& s){
    return s == "assignResult" || s== "moveTurePar" || s=="moveFalsePar" || s=="output";
}

void trim(string &s)
{
    if (s.empty())
    {
        return ;
    }
    s.erase(0,s.find_first_not_of(' '));
    s.erase(s.find_last_not_of(' ') + 1);
}

stack<FunSheet::iterator> funS;

DAG_node::DAG_node(string& s):name(s){
    if(isDigit(s[0]))
        type=_CONST_;
    else if(s=="_")
        type=_VOID_;
    else{
        type=_VAR_;
        auto it=funS.top();
        if(it.isTmpVariable(s))
            type = _TMP_;
        //查找符号表，判断类型
    }
}

Optimizer::Optimizer(const string& s):ifile(s.c_str())
{
    DAG_node T;
    DAG tmp;
    tmp.var.push_back(T);
    dag.push_back(tmp);
    auto f_it=xTable.getMain();
    funS.push(f_it);
}

void Optimizer::divBlock(ifstream& file){
    bool first = true;
    if(file.is_open())
        cout<<"open!"<<endl;
    string line="";
    vector<string> t;
    while(getline(file,line)){
        string temp="";
        for(char c:line){
            if(c!=','&&c!='(')
                temp+=c;
            else if(c==',')
                break;
        }
        trim(temp);
        if(blockend(temp)){

            t.push_back(line);
            TAC.push_back(t);
            t.clear();
        }
        else
        {
            t.push_back(line);
        }

    }
    if(!t.empty())
        TAC.push_back(t);
}


int Optimizer::findNode(DAG_node& node){
    auto it=dag.rbegin();
    for(;it!=dag.rend();it++){
        for(auto elem:it->var){
            if(elem==node&&it->op!="NaN")
                return it->id;
        }
    }
    if(it==dag.rend())
        return 0;
}


int Optimizer::findOperation(string& op,DAG_node& src1,DAG_node& src2){
    // auto it=dag.rbegin();
    // for(;it!=dag.rend();it++){
    //     if(it->op==op){
    //         vector<DAG_node>& varL=dag[it->lchild].var;
    //         auto _it=find(varL.begin(),varL.end(),src1);
    //         if(_it != varL.end()){
    //             if(src2.name == "_" && it->rchild == 0)
    //                 return it->id;
    //             else if(it->rchild != 0){
    //                 vector<DAG_node>& varR=dag[it->rchild].var;
    //                 auto __it=find(varR.begin(),varR.end(),src2);
    //                 if(__it != varR.end())
    //                     return it->id;
    //             }
    //         }
    //     }
    // }
    int left=0, right=0;
    left = findNode(src1);
    right = findNode(src2);
    if(left == 0 && right == 0)
        return 0;
    auto it=dag.rbegin();
    for(;it!=dag.rend();it++){
        if(it->op==op && it->lchild == left && it->rchild == right)
        {
            return it->id;
        }
    }
    if(it == dag.rend())
        return 0;
}

pair<int,bool> Optimizer::new_node(DAG_node& node)
{
    int id=findNode(node);
    if(id){
        return make_pair(id,true);
    }
    else{
        DAG dnode;
        dnode.id=dag.size();
        dnode.var.push_back(node);
        dag.push_back(dnode);
        mp[node.name]=dnode.id;
        return make_pair(dnode.id,false);
    }
}

void Optimizer::erase_ID(int id,DAG_node& node){
    vector<DAG_node>& var=dag[id].var;
    if(var[0]==node)
        return;
    else{
        for(auto it=var.begin();it!=var.end();it++){
            if(*it == node){
                var.erase(it);
                return;
            }
        }
    }
}

void Optimizer::insert_ID(int id,DAG_node& node){
    vector<DAG_node>& var=dag[id].var;
    int loc=findNode(node);
    if(var.size() == 0){
        var.push_back(node);
    }
    else if(var[0].type == _CONST_){
        var.push_back(node);
    }
    else if(node.type == _CONST_ || (node.type == _VAR_ && var[0].type== _TMP_)){
        var.push_back(node);
        swap(var.front(),var.back());
    }
    else
    {
        var.push_back(node);
    }
    if(loc){
        erase_ID(loc,node);
    }
    mp[node.name]=id;
}

string Optimizer::calculate2(string& op,string& a,string& b){
    stringstream ss;
    string res="0";
    int na,nb,ans;
    na=stoi(a.substr(1));
    nb=stoi(b.substr(1));
    if(op=="ADD") ans=na+nb;
    else if(op=="SUB") ans=na-nb;
    else if(op=="MUL") ans=na*nb;
    else if(op=="DIV") ans=na/nb;
    else if(op=="GT") ans=(na>nb);
    else if(op=="LT") ans=(na<nb);
    else if(op=="GE") ans=(na>=nb);
    else if(op=="LE") ans=(na<=nb);
    else if(op=="EQ") ans=(na==nb);
    else if(op == "NEQ") ans=(na != nb);
    else if(op == "AND") ans=(na && nb);
    else if(op == "OR") ans=(na || nb);
    res += to_string(ans);
    return res;
}

string Optimizer::calculate1(string& op,string& a){
    string res="0";
    int na,ans;
    na=stoi(a.substr(1));
    if(op == "NOT") ans=!na;
    else if(op == "NEG") ans=-na;
    res += to_string(ans);
    return res;
}

void Optimizer::insert(int loc,DAG_node& src,DAG_node& dest){
    if(src.type==_VAR_ && isVarShielded(src,loc)){
        DAG dnode;
        dnode.id=dag.size();
        if(dest.type != _VOID_)
            dnode.var.push_back(dest);
        dag.push_back(dnode);
        insert_ID(dag.size()-1,src);
    }
    else{
        insert_ID(loc,src);
    }
}

bool Optimizer::isVarShielded(DAG_node& a,int loc){
    auto it=dag.rbegin();
    for(;it!=dag.rend();it++){
        if(it->var[0]==a){
            if((dag.rend()-it)>loc)
                return true;
        }
    }
    return false;
}

void Optimizer::createDAG(vector<string>& tac){
    static bool first=true;
    for(auto elem1:tac){
        string op="",src1=op,src2=op,dest=op;
        int ptr=1;
        while(elem1[ptr]==' ') ptr++;
        while(elem1[ptr]!=',') op+=elem1[ptr++];
        ptr++;
        while(elem1[ptr]==' ') ptr++;
        while(elem1[ptr]!=',') src1+=elem1[ptr++];
        ptr++;
        while(elem1[ptr]==' ') ptr++;
        while(elem1[ptr]!=',') src2+=elem1[ptr++];
        ptr++;
        while(elem1[ptr]==' ') ptr++;
        while(elem1[ptr]!=')') dest+=elem1[ptr++];
        ptr++;
        trim(op);trim(src1);trim(src2);trim(dest);

        if(op== "beginProgram"){
            if(!first) {
                auto it = funS.top();
                it = it.getFunIterator(src1);
                funS.push(it);
            }
            first = false;
        }


        DAG_node node1(src1),node2(src2),node3(dest);
        pair<int,bool> n1,n2;
        n1.first=n2.first=0;
        if(op!="moveFalsePar") {
            if (src1 != "_")
                n1 = new_node(node1);
            if (src2 != "_")
                n2 = new_node(node2);
        }
        if (op=="Assign")
        {
            int loc=n1.first;
            insert(loc,node3,node1);
        }
        else if(!blockend(op))
        {
            auto n1_head=dag[n1.first].var[0],n2_head=dag[n2.first].var[0];
            if(n1_head.type == _CONST_ && n2_head.type == _CONST_){
                string ans=calculate2(op,n1_head.name,n2_head.name);
                DAG_node ans_node(ans);
                pair<int,bool> n=new_node(ans_node);
                insert(n.first,node3,ans_node);
            }
            else if(n1_head.type == _CONST_ && n2_head.type == _VOID_ && !specialJudge(op)){
                string ans=calculate1(op,n1_head.name);
                DAG_node ans_node(ans);
                pair<int,bool> n=new_node(ans_node);
                insert(n.first,node3,ans_node);
            }
            else{
                int loc = findOperation(op,node1,node2);
                if(loc&&!specialJudge(op)){
                    DAG_node tmmmmp;
                    insert(loc,node3,tmmmmp);
                }
                else if(op=="moveFalsePar"){
                    DAG dnode1,dnode2;
                    dnode1.id=dag.size();
                    dnode1.var.push_back(node1);
                    dnode1.op="NaN";
                    dag.push_back(dnode1);
                    dnode2.id=dag.size();
                    dnode2.op="moveFalsePar";
                    dnode2.var.push_back(node2);
                    dnode2.lchild=dnode1.id;
                    dnode2.rchild=0;
                    dag.push_back(dnode2);
                }
                else{
                    DAG dnode;
                    dnode.id=dag.size();
                    dnode.op =op;
                    dnode.var.push_back(node3);
                    dnode.lchild = n1.first;
                    dnode.rchild = n2.first;
                    dag.push_back(dnode);
                    //mp[node3.name]=dnode.id;
                }
            }
        }

        if(op == "endFunction")
            funS.pop();
    }

}

void Optimizer::rebuild(vector<vector<string>>::iterator& _it){
    string end=_it->back();
    _it->clear();
    for(auto it:dag){
        // if(it.op=="ASSIGN")
        // {
        //     for(int i=1;i<it.var.size();i++)
        //     {
        //         if(it.var[i].type==_VAR_)
        //         {
        //             string tmp="(ASSIGN,";
        //             tmp+=it.var[i].name;
        //             tmp+=",,";
        //             tmp+=it.var[0].name;
        //             tmp+=")";
        //             v.push_back(tmp);
        //         }
        //     }
        // }
        if(it.op != "Assign"&&it.op!="NaN")
        {
            string left=dag[it.lchild].var[0].name;
            string right="_";
            if(it.rchild != 0)
                right = dag[it.rchild].var[0].name;;
            string tmp="( ";
            tmp+=it.op;
            tmp+=" , ";
            tmp+=left;
            tmp+=" , ";
            tmp+=right;
            tmp+=" , ";
            tmp+=it.var[0].name;
            tmp+=" )";
            _it->push_back(tmp);
        }
        for(int i=1;i<it.var.size();i++)
        {
            if(it.var[i].type==_VAR_)
            {
                string tmp="( Assign , ";
                tmp+=it.var[0].name;
                tmp+=" , _ , ";
                tmp+=it.var[i].name;
                tmp+=" )";
                _it->push_back(tmp);
            }
        }

    }
    string temp="";
    for(char c:end){
        if(c!=','&&c!='(')
            temp+=c;
        else if(c==',')
            break;
    }
    trim(temp);
    if(blockend(temp))
        _it->push_back(end);
    dag.clear();
    DAG_node T;
    DAG tmp;
    tmp.var.push_back(T);
    dag.push_back(tmp);
}

void Optimizer::createByBlock(){
    for(auto it=TAC.begin();it!=TAC.end();it++){
        createDAG(*it);
        rebuild(it);
    }
}

void Optimizer::optimize(ifstream& file){
    divBlock(file);
    createByBlock();
    for(auto v: TAC){
        for(auto s:v)
            ifile<<s<<endl;
        //ifile<<endl;
    }
}


//int main(){
//    ifstream f("./test.txt");
//    if(f.is_open())
//        cout<<"opem!"<<endl;
//    Optimizer opt;
//    opt.optimize(f);
//}