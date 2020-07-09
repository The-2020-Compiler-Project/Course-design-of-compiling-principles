#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>
using namespace std;

enum nodeType{_VAR_,_CONST_,_TMP_,_VOID_};

inline bool isDigit(char c){
    return c>='0'&&c<='9';
}

struct DAG_node{
    string name;
    nodeType type;
    DAG_node():name("_"),type(_VOID_){}
    DAG_node(string& s);
    bool operator==(const DAG_node& n){
        return n.name==name&&n.type==type;
    }
};

struct DAG{
    string op;
    int id;
    vector<DAG_node> var;
    int lchild;
    int rchild;
    DAG():id(0),lchild(0),rchild(0),op("Assign"){}
};

class Optimizer{
public:
    Optimizer(const string& s);
    void optimize(ifstream&);                                             //对外接口
private:
    ofstream ifile;                                                      //四元式序列文件
    vector<DAG> dag;                                                      //DAG图数据结构
    vector<vector<string>> TAC;                                           //分块后的四元式序列
    map<string,int> mp;

    void divBlock(ifstream&);                                             //分块函数

    void createDAG(vector<string>&);                                      //构建DAG图

    int findNode(DAG_node&);                                              //查找结点，返回位置，0表示未找到

    int findOperation(string& op,DAG_node& src1,DAG_node& src2);          //查找公共表达式，返回位置，0表示未找到

    void createByBlock();                                                 //分块构建      

    pair<int,bool> new_node(DAG_node&);                                   //创建新节点，返回位置，若已存在返回<位置，true>

    void erase_ID(int id,DAG_node& str);                                  //从某节点中删除标识符

    void insert_ID(int id, DAG_node& str);                                //在某节点中插入标识符

    string calculate2(string& op,string& a,string& b);                     //计算常数表达式

    string calculate1(string& op,string&);

    void insert(int loc,DAG_node&,DAG_node&);

    bool isVarShielded(DAG_node&,int);                                          //判断变量是否被屏蔽

    void rebuild(vector<vector<string>>::iterator& it);                   //重组四元式


};