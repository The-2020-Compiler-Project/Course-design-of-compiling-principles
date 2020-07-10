//
// Created by Nowidh on 2020/7/8.
//

#include"TargetCodeGenerator.h"
#include<iostream>
#include<algorithm>




using std::ios;
using std::cout;
using std::endl;

extern FunSheet xTable;

//测试用，实际应该使用符号表的函数
bool isTemporary(string nowString)
{
    if (nowString[0] == '_' && nowString.size()>1)
        return true;
    return false;
}

//查看结构体域名的偏移量，测试用
int findSonOffset(string sonName)
{
    return 0;
}

//从文件中读取四元式
void TargetCodeGenerator::read(string filename)
{
    ifstream infile;
    infile.open(filename, ios::in);
    if (!infile.is_open())
    {
        cout << "Open Failed!!!" << endl;
        exit(0);
    }

    string line, th[4];
    while (!infile.eof())
    {
        std::getline(infile, line);

        if(line == "")
            break;
        //book为开始复制的位置，turn为到了哪个操作数（0，1，2，3）
        int book = 1, turn = 0;

        for (int i = 1; i < line.size() - 1; i++)
        {
            if (line[i] == ',')
            {
                th[turn] = "";
                for (int j = book + 1; j < i - 1; j++)
                {
                    th[turn] += line[j];
                }
                turn++;
                book = i + 1;
            }
            if(turn == 3)
                break;
        }

        //因为到了最后一个逗号就停下了，所以要把最后的目标操作数再处理一下
        if(line!="")
        {
            th[turn] = "";
            for (int i = book + 1; i < line.size() - 2; i++)
            {
                th[turn] += line[i];
            }
        }

        //存入所有四元式集中
        quar tq;
        tq.oper = th[0], tq.o1 = th[1], tq.o2 = th[2], tq.target = th[3];
        AllQuarColl.push_back(tq);
    }
    int a = 1;
}

//是否是跳转符，目前有if，else，while，we
//是则返回true，否则返回false
bool TargetCodeGenerator::isSpilt(quar nowQuar)
{
    for (int i = 0; i < SpiltQuarSize; i++)
    {
        if (nowQuar.oper == SpiltQuar[i])
        {
            return true;
        }
    }
    return false;
}

//分割四元式为基本块，存入基本块集中
void TargetCodeGenerator::spiltMidCode()
{
    quar nowQuar;
    set<int> BeginQuarColl;		//基本块入口指令编号集，从0开始
    BeginQuarColl.insert(0);	//第一个四元式加入

    //遍历所有四元式
    for (int i = 1; i < AllQuarColl.size(); i++)
    {

        //如果上一条是跳转语句，则将此条加入
        if (isSpilt(AllQuarColl[i - 1]))
        {
            BeginQuarColl.insert(i);
        }
        //特判，while开始四元式也是开始四元式
        if (AllQuarColl[i].oper == quatName::beginWhile)
        {
            BeginQuarColl.insert(i);
        }
    }

    //当前基本块
    BaseBlock nowBlock;

    //遍历每一个编号，以当前入口语句开始，下一个入口语句结束（不包含），开始复制
    for (auto g = BeginQuarColl.begin(); g != prev(BeginQuarColl.end()); g++)
    {
        nowBlock.BaseBlockQuar.clear();

        auto next = g;
        int nowBegin = *g;				//开始
        int nowEnd = *(++next);			//结束

        //基本块开始和结束的位置编号
        nowBlock.BaseBlockStart = nowBegin;
        nowBlock.BaseBlockEnd = nowEnd - 1;

        //开始复制
        for (int i = nowBegin; i < nowEnd; i++)
        {
            nowBlock.BaseBlockQuar.push_back(AllQuarColl[i]);
        }

        //加入基本块集
        BaseBlockColl.push_back(nowBlock);
    }

    //剩下最后一个开始语句直到所有四元式结束，所以补上
    nowBlock.BaseBlockQuar.clear();
    int nowBegin = *prev(BeginQuarColl.end());
    int nowEnd = AllQuarColl.size();

    nowBlock.BaseBlockStart = nowBegin;
    nowBlock.BaseBlockEnd = nowEnd - 1;

    for (int i = nowBegin; i < nowEnd; i++)
    {
        nowBlock.BaseBlockQuar.push_back(AllQuarColl[i]);
    }
    BaseBlockColl.push_back(nowBlock);

    int a = 1;
}

//判断操作符是否是函数调用，函数声明，主程序声明，过程声明，函数结束，主程序结束，过程结束
int BaseBlock::isHaveActivity(string nowOp)
{
    for (int i = 0; i < notAcitvityCount; i++)
    {
        if (nowOp == notAcitvity[i])
            return 1;
    }
    return 0;
}

//判断操作数是否是空格
int BaseBlock::isBlankOper(string nowOper)
{
    if (nowOper == "_" && nowOper.size()==1)
        return 1;
    return 0;
}

//初始化基本块活跃信息，按照基本块中四元式数量生成对应数量的活跃信息结构体
//临时变量初始化为no，其他初始化为yes
void BaseBlock::initBlockActive()
{
    for (int i = 0; i < BaseBlockQuar.size(); i++)
    {
        //当前四元式的活跃信息，算符没啥影响，所以直接复制
        active nowActivity;
        nowActivity.oper = BaseBlockQuar[i].oper;

        //查看是否是临时变量
        string checkTemp = "";

        //查看是否是空的
        if (!isBlankOper(BaseBlockQuar[i].o1))		//非空，继续
        {
            checkTemp = BaseBlockQuar[i].o1;
            if (isTemporary(checkTemp))				//查看是否是临时变量
            {
                nowActivity.o1 = "no";				//临时变量为no
                tempVariable[checkTemp] = "no";
            }
            else
            {
                nowActivity.o1 = "yes";				//否则为yes
                notTempVariable[checkTemp] = "yes";
            }
        }

        //第二个操作数，同上
        if (!isBlankOper(BaseBlockQuar[i].o2))
        {
            checkTemp = BaseBlockQuar[i].o2;
            if (isTemporary(checkTemp))
            {
                nowActivity.o2 = "no";
                tempVariable[checkTemp] = "no";
            }
            else
            {
                nowActivity.o2 = "yes";
                notTempVariable[checkTemp] = "yes";
            }
        }

        //目标操作数，同上
        if (!isBlankOper(BaseBlockQuar[i].target))
        {
            checkTemp = BaseBlockQuar[i].target;
            if (isTemporary(checkTemp))
            {
                nowActivity.target = "no";
                tempVariable[checkTemp] = "no";
            }
            else
            {
                nowActivity.target = "yes";
                notTempVariable[checkTemp] = "yes";
            }
        }

        BaseBlockActivity.push_back(nowActivity);	//存储
        int a = 1;
    }

}

//计算活跃信息，用的上课ppt的方法
void BaseBlock::checkBlockActive()
{
    //首先初始化当前基本块的活跃信息
    initBlockActive();

    int a = 1;
    //开始填活跃信息表，倒着来
    for (int i = BaseBlockActivity.size() - 1; i >= 0; i--)
    {
        if (isHaveActivity(BaseBlockQuar[i].oper))
            continue;

        string nowVariable = "";
        string changeMessage = "";

        if (!isBlankOper(BaseBlockQuar[i].target))
        {
            nowVariable = BaseBlockQuar[i].target;		//当前操作数
            if (isTemporary(nowVariable))						//如果是临时变量的操作
            {
                changeMessage = tempVariable[nowVariable];
                BaseBlockActivity[i].target = changeMessage;	//从表中找到值，赋给活跃信息表
                tempVariable[nowVariable] = "no";				//目标操作数变为no
            }
            else												//如果不是临时变量的操作
            {
                changeMessage = notTempVariable[nowVariable];
                BaseBlockActivity[i].target = changeMessage;
                notTempVariable[nowVariable] = "no";
            }
        }

        if (!isBlankOper(BaseBlockQuar[i].o2))
        {
            nowVariable = BaseBlockQuar[i].o2;
            if (isTemporary(nowVariable))
            {
                changeMessage = tempVariable[nowVariable];
                BaseBlockActivity[i].o2 = changeMessage;		//从表中找到值，赋给活跃信息表
                tempVariable[nowVariable] = "yes";				//操作数2变为yes
            }
            else
            {
                changeMessage = notTempVariable[nowVariable];
                BaseBlockActivity[i].o2 = changeMessage;
                notTempVariable[nowVariable] = "yes";
            }
        }

        if (!isBlankOper(BaseBlockQuar[i].o1))
        {
            nowVariable = BaseBlockQuar[i].o1;
            if (isTemporary(nowVariable))
            {
                changeMessage = tempVariable[nowVariable];
                BaseBlockActivity[i].o1 = changeMessage;		//从表中找到值，赋给活跃信息表
                tempVariable[nowVariable] = "yes";				//操作数1变为yes
            }
            else
            {
                changeMessage = notTempVariable[nowVariable];
                BaseBlockActivity[i].o1 = changeMessage;
                notTempVariable[nowVariable] = "yes";
            }
        }
        int a = 1;
    }
}





//计算变量存储开始单元相对于old sp存储位置(使用oldsp低位的位置)的偏移量，这个函数是我应该实现的
int TargetCodeGenerator::findBpOffset(string tempName)
{
    //查看层次，暂时使用测试函数
    int tempLevel = itStack.back().getLevel(tempName);

    //返回值(2)+display((层次+1)*2)
    //由于变量偏移位置给出时也是从零开始，所以一开始可以直接加2
    //应该加1，因为计算后指向display表的最后的低位，再加1才是存储开始位置
    int spOffset = 2 + 2 * (tempLevel + 1) + 1;
    return spOffset;
}
//计算所求变量相对于所在层sp的偏移量，这个函数是我应该实现的
int TargetCodeGenerator::findTrueOffset(string tempName)
{
    //在这时我将会把偏移量转化为指向低位的
    ElemSheet::iterator tmpIter = itStack.back().getElemIterator(tempName);
    int trueOffset = findBpOffset(tempName) + tmpIter.offSet() + 1;
    return trueOffset;
}


//生成数据段和堆栈段,初始化,在准备生成代码时调用
void TargetCodeGenerator::initAsm()
{
    code nowCode;

    //生成数据段因为所有数据都存在堆栈段，所以数据段为空
    nowCode.name = "sseg", nowCode.oper = "segment", nowCode.dest = "stack";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";


    nowCode.name = blank, nowCode.oper = "Temp", nowCode.dest = "db", nowCode.source = "'0000H','$'";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

    nowCode.name = blank, nowCode.oper = "N", nowCode.dest = "dw", nowCode.source = "0";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

    nowCode.name = blank, nowCode.oper = "dw", nowCode.dest = "512", nowCode.source = "dup(?)";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";


    nowCode.name = "sseg", nowCode.oper = "ends";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";


    //开始程序段，初始化数据段，堆栈段，程序段
    nowCode.name = "cseg", nowCode.oper = "segment";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";


    nowCode.name = blank, nowCode.oper = "assume",nowCode.dest="cs:cseg,",nowCode.source="ds:sseg";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";


    nowCode.name = "	assume", nowCode.oper = "ss:sseg";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";


    /* nowCode.name = "PrintAx", nowCode.oper = "proc", nowCode.dest = "", nowCode.source = "";
     targetCodeArea.push_back(nowCode);
     nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

     nowCode.name = blank, nowCode.oper = "push", nowCode.dest = "si", nowCode.source = "";
     targetCodeArea.push_back(nowCode);
     nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

     nowCode.name = blank, nowCode.oper = "push", nowCode.dest = "cx", nowCode.source = "";
     targetCodeArea.push_back(nowCode);
     nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

     nowCode.name = blank, nowCode.oper = "push", nowCode.dest = "dx", nowCode.source = "";
     targetCodeArea.push_back(nowCode);
     nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

     nowCode.name = blank, nowCode.oper = "mov", nowCode.dest = "si,", nowCode.source = "offset Temp+3";
     targetCodeArea.push_back(nowCode);
     nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

     nowCode.name = blank, nowCode.oper = "xor", nowCode.dest = "cx,", nowCode.source = "cx";
     targetCodeArea.push_back(nowCode);
     nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

     nowCode.name = blank, nowCode.oper = "mov", nowCode.dest = "cl,", nowCode.source = "4";
     targetCodeArea.push_back(nowCode);
     nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

     nowCode.name = "MainPart:", nowCode.oper = "mov", nowCode.dest = "dh,", nowCode.source = "al";
     targetCodeArea.push_back(nowCode);
     nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

     nowCode.name = blank, nowCode.oper = "shr", nowCode.dest = "ax,", nowCode.source = "1";
     targetCodeArea.push_back(nowCode);
     nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

     nowCode.name = blank, nowCode.oper = "shr", nowCode.dest = "ax,", nowCode.source = "1";
     targetCodeArea.push_back(nowCode);
     nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

     nowCode.name = blank, nowCode.oper = "shr", nowCode.dest = "ax,", nowCode.source = "1";
     targetCodeArea.push_back(nowCode);
     nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

     nowCode.name = blank, nowCode.oper = "shr", nowCode.dest = "ax,", nowCode.source = "1";
     targetCodeArea.push_back(nowCode);
     nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";*/



}

//处理程序开始四元式
void TargetCodeGenerator::programBegin(quar nowQuar)
{
    //start
    code nowCode;
    nowCode.name = "start:", nowCode.oper = "mov",nowCode.dest="ax,",nowCode.source="sseg";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";


    nowCode.name = blank, nowCode.oper = "mov", nowCode.dest = "ds,", nowCode.source = "ax";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";


    nowCode.name = blank, nowCode.oper = "mov", nowCode.dest = "ax,", nowCode.source = "sseg";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";


    nowCode.name = blank, nowCode.oper = "mov", nowCode.dest = "ss,", nowCode.source = "ax";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";

    //开始分配程序的堆栈空间
    //首先将bp存入，为old sp
    stackManager.push(targetCodeArea, blank, "bp");
    //mov bp，sp
    stackManager.movp(targetCodeArea, blank, "bp,", "sp");

    //确定需要的存储单元，查表确定数据存储区的长度后计算，old sp已经加入了，就不用算了
    //现在bp指向old sp的低位，通过[bp]可以取出old sp
    //(返回值(2)+display((当前函数层次数+1)*2)+数据存储区长度（不用乘，可以直接得到存储单元数))
    FunSheet::iterator nowIt = itStack[itStack.size()-1];
    int dataLenth = nowIt.len();
    int nowLevel = nowIt.level();
    int storeLenth = 2 + (nowLevel + 1) * 2 + dataLenth;

    //通过减小sp来开辟存储空间
    stackManager.subSp(targetCodeArea, blank, to_string(storeLenth));

    //计算display表，之前已经获取过当前层次了，直接用就行了
    //因为是程序开始，所以直接2个存储单元赋值就行了
    MOV(targetCodeArea, blank, "word ptr [bp-4],", "bp");

}

//在display表中查找位置，输入变量层次，返回指向低位的偏移量
int TargetCodeGenerator::findInDisplay(int Level)
{
    //返回值(2)+(Level+1)*2
    int place = 2 + (Level + 1) * 2;
    return place;
}

//处理赋值
void TargetCodeGenerator::assignCalculation(quar nowQuar, string name)
{
    string o1 = nowQuar.o1;

    //首先如果有标号，生成标号
    returnLabel(name);

    //开始进行LD过程
    string o1Code = "";
    o1Code = findXxx(o1, blank);

    string nowRe = "";
    //生成mov指令
    MOV(targetCodeArea, blank, "ax,", o1Code);
    nowRe = "ax";

    //开始进行ST过程,target一定不是数字
    nowRdlTemp = nowQuar.target;


    //开始寻址
    string target = nowQuar.target;
    string tarCode = "";
    tarCode = findXxx(target, blank);
    MOV(targetCodeArea, blank, tarCode + ",", "ax");
    //更改nowRdlTemp的值
    nowRdlTemp = "0";	//清空

}

//处理加减
void TargetCodeGenerator::addCalculation(quar nowQuar, string name)
{
    //首先如果有标号，生成标号
    returnLabel(name);

    //首先进行LD过程
    //判断操作数1是否是数字
    string o1 = nowQuar.o1;
    string o1Code = "";

    o1Code = findXxx(o1, blank);
    nowRdlTemp = o1;
    string nowRe = "";
    MOV(targetCodeArea, blank, "ax,", o1Code);
    nowRe = "ax";


    //之后开始add/sub过程
    string o2 = nowQuar.o2;
    string o2Code = "";

    o2Code = findXxx(o2, blank);
    nowRdlTemp = nowQuar.target;
    if (nowQuar.oper == quatName::ADD)
    {
        ADD(targetCodeArea, blank, nowRe + ",", o2Code);
    }
    else
    {
        SUB(targetCodeArea, blank, nowRe + ",", o2Code);
    }

    //之后开始ST过程
    string target = nowQuar.target;
    string tarCode = "";

    //target必定为变量，不用判断（虽然判断也不麻烦）
    tarCode = findXxx(target, blank);
    nowRdlTemp = target;
    MOV(targetCodeArea, blank, tarCode + ",", nowRe);

}

//处理乘
void TargetCodeGenerator::mulCalculation(quar nowQuar, string name)
{
    //首先如果有标号，生成标号
    returnLabel(name);

    //进行LD过程
    string o1 = nowQuar.o1;
    string o1Code = "";
    string nowRe = "";
    o1Code = findXxx(o1, blank);
    nowRdlTemp = o1;	//ax寄存器内的东西

    //这里有优化的空间，可以用ppt的方法
    XOR(targetCodeArea, blank, "ax,", "ax");	//清空ax
    XOR(targetCodeArea, blank, "dx,", "dx");	//清空dx，dx暂时只会在乘除用到

    MOV(targetCodeArea, blank, "ax,", o1Code);
    nowRe = "ax";

    //开始mul过程
    string o2 = nowQuar.o2;
    string o2Code = "";

    o2Code = findXxx(o2, blank);
    MOV(targetCodeArea, blank,"cx,",o2Code);
    nowRdlTemp = nowQuar.target;
    IMUL(targetCodeArea, blank, "cx");

    //开始ST过程
    string target = nowQuar.target;
    string tarCode = "";

    tarCode = findXxx(target, blank);
    MOV(targetCodeArea, blank, tarCode + ",", "ax");

}

//处理除
void TargetCodeGenerator::divCalculation(quar nowQuar, string name)
{
    //首先如果有标号，生成标号
    returnLabel(name);

    //进行LD过程
    string o1 = nowQuar.o1;
    string o1Code = "";
    string nowRe = "";

    o1Code = findXxx(o1, blank);
    nowRdlTemp = o1;	//ax寄存器内的东西

    //这里有优化的空间，可以用ppt的方法
    XOR(targetCodeArea, blank, "ax,", "ax");	//清空ax
    XOR(targetCodeArea, blank, "dx,", "dx");	//清空dx，dx暂时只会在乘除用到

    MOV(targetCodeArea, blank, "ax,", o1Code);
    nowRe = "ax";

    //开始div过程
    string o2 = nowQuar.o2;
    string o2Code = "";

    o2Code = findXxx(o2, blank);
    MOV(targetCodeArea, blank,"cx,",o2Code);
    nowRdlTemp = nowQuar.target;
    IDIV(targetCodeArea, blank, "cx");

    //开始ST过程
    string target = nowQuar.target;
    string tarCode = "";

    tarCode = findXxx(target, blank);
    MOV(targetCodeArea, blank, tarCode + ",", "ax");

}

//处理关系运算
void TargetCodeGenerator::relCalculation(quar nowQuar, string name)
{
    //首先如果有标号，生成标号
    returnLabel(name);

    //首先进行cmp o1，o2环节
    //先LD
    string o1 = nowQuar.o1;
    string o1Code = "";
    string nowRe = "";

    o1Code = findXxx(o1, blank);
    nowRdlTemp = o1;	//ax寄存器内的东西

    MOV(targetCodeArea, blank, "ax,", o1Code);
    nowRe = "ax";

    //LD之后就是寻址cmp了
    string o2 = nowQuar.o2;
    string o2Code = "";

    o2Code = findXxx(o2, blank);
    //寄存器不变，就不存了
    //该cmp了
    CMP(targetCodeArea, blank, nowRe + ",", o2Code);

    //现在该瞎鸡儿跳转了，手动生成标号的时候注意点
    //条件跳转指令
    string relJmpCode = getRelAsm(nowQuar.oper);
    relJmp(targetCodeArea, blank, relJmpCode, "");
    //标号入栈
    ifSEM.push_back(targetCodeArea.size() - 1);
    //为假时mov ax,0
    MOV(targetCodeArea, blank, "ax,", "0");
    //无条件跳转到开始给t赋值
    relJmp(targetCodeArea, blank, "jmp", "");
    //入栈
    elSEM.push_back(targetCodeArea.size() - 1);
    //现在生成假跳的目标位置
    string nowLabel = "x" + to_string(labelId++);
    MOV(targetCodeArea, nowLabel, "ax,", "1");
    //弹对应的栈
    int nowJmp = ifSEM[ifSEM.size() - 1];
    ifSEM.pop_back();
    targetCodeArea[nowJmp].source = nowLabel;

    //开始把ax的值赋给对应的变量
    nowLabel = "x" + to_string(labelId++);
    string target = nowQuar.target;
    string tarCode = "";

    tarCode = findXxx(target, nowLabel);

    //反填
    nowJmp = elSEM[elSEM.size() - 1];
    elSEM.pop_back();
    targetCodeArea[nowJmp].source = nowLabel;
    MOV(targetCodeArea, blank, tarCode + ",", "ax");
    nowRdlTemp = target;
}

//处理if跳转(假跳)
void TargetCodeGenerator::ifCalculation(quar nowQuar, string name)
{
    //首先如果有标号，生成标号
    returnLabel(name);

    //开始寻址
    string o1 = nowQuar.o1;
    string o1Code = "";

    o1Code = findXxx(o1, blank);

    //该cmp了
    MOV(targetCodeArea, blank, "ax,", "0");
    CMP(targetCodeArea, blank, "ax,", o1Code);
    //该跳转了
    relJmp(targetCodeArea, blank, "je", "");
    //入if栈
    ifSEM.push_back(targetCodeArea.size() - 1);
    nowRdlTemp = "const";
}

//处理else跳转(无条件)
void TargetCodeGenerator::elCalculation(quar nowQuar, string name)
{
    returnLabel(name);
    //直接跳转就行，之后反填
    relJmp(targetCodeArea, blank, "jmp", "");
    ifJmpName = "x"+ to_string(labelId++);
    //入else栈
    ifSEM.push_back(targetCodeArea.size() - 1);

}

//处理endIf(无动作，NOP,为了嵌套才搞了这个)
void TargetCodeGenerator::endIfCalculation(quar nowQuar, string name)
{
    returnLabel(name);
    ifJmpName = "x"+ to_string(labelId++);
}

//处理wh语句
void TargetCodeGenerator::whCalculation(quar nowQuar, string name)
{
    NOP(targetCodeArea, name);
    whSEM.push_back(targetCodeArea.size() - 1);
}

//处理do跳转(假跳)
void TargetCodeGenerator::doCalculation(quar nowQuar, string name)
{
    //首先如果有标号，生成标号
    returnLabel(name);

    //开始寻址
    string o1 = nowQuar.o1;
    string o1Code = "";

    o1Code = findXxx(o1, blank);

    //该cmp了
    MOV(targetCodeArea, blank, "ax,", "0");
    CMP(targetCodeArea, blank, "ax,", o1Code);
    //该跳转了
    relJmp(targetCodeArea, blank, "je", "");
    //入do栈
    doSEM.push_back(targetCodeArea.size() - 1);
    nowRdlTemp = "const";
}

//处理we跳转(无条件)
void TargetCodeGenerator::weCalculation(quar nowQuar, string name)
{
    returnLabel(name);
    //手动取出标号
    int nowReturn = whSEM[whSEM.size() - 1];
    string label = targetCodeArea[nowReturn].name;
    whSEM.pop_back();
    relJmp(targetCodeArea, blank, "jmp", label);
}

//处理数组取值getAddress(address,a,b,t)
void TargetCodeGenerator::getAdrCalculation(quar nowQuar, string name)
{
    returnLabel(name);

    //计算真实偏移量,原因查看测试ifwhile文档
    int trueOffset = findTrueOffset(nowQuar.o1);

    //查display表确定这个变量所在的活动记录开始位置
    //表的前缀可以计算得出，所有的东西计算后变为汇编代码即可
    //查看目标操作数的层次
    int o1Level = itStack.back().getLevel(nowQuar.o1);

    //在display表中查找目标位置
    int o1Dis = findInDisplay(o1Level);

    //将地址装到bx中，通过[bp-xxx]找到
    string strDis = to_string(o1Dis);

    //因为地址为2个存储单元，所以为word ptr
    strDis = "word ptr [bp-" + strDis + "]";

    //装入si中
    MOV(targetCodeArea, blank, "si,", strDis);

    //si先减去真实偏移量
    string strTrueOffset = to_string(trueOffset);
    SUB(targetCodeArea, blank, "si,", strTrueOffset);

    //再减去数组偏移量
    string arrOffset = findXxx(nowQuar.o2, blank);
    SUB(targetCodeArea, blank, "si,", arrOffset);
    SUB(targetCodeArea, blank, "si,", arrOffset);
    MOV(targetCodeArea, blank, "ax,", "si");

    //之后寻址target
    string tarCode = findBpxxx(nowQuar.target, blank);
    MOV(targetCodeArea, blank, tarCode + ",", "ax");

}

//处理函数声明开始beginFunction
void TargetCodeGenerator::beginFunCalculation(quar nowQuar)
{
    //从函数栈获得当前函数的name
    string nowFunName = itStack.back().name();
    PROC(targetCodeArea, nowFunName);

    //开始分配函数的堆栈空间
    //首先将bp存入，为old sp
    stackManager.push(targetCodeArea, blank, "bp");
    //mov bp，sp
    stackManager.movp(targetCodeArea, blank, "bp,", "sp");

    //确定需要的存储单元，查表确定数据存储区的长度后计算，old sp已经加入了，就不用算了
    //现在bp指向old sp的低位，通过[bp]可以取出old sp
    //(返回值(2)+display((当前函数层次数+1)*2)+数据存储区长度（不用乘，可以直接得到存储单元数))
    int dataLenth = itStack.back().len();
    int nowLevel = itStack.back().level();
    int storeLenth = 2 + (nowLevel + 1) * 2 + dataLenth;

    //通过减小sp来开辟存储空间
    stackManager.subSp(targetCodeArea, blank, to_string(storeLenth));
    //将old sp赋给bx,之后让bx指向display开始的低位
    MOV(targetCodeArea, blank, "bx,", "word ptr [bp]");
    SUB(targetCodeArea, blank, "bx,", "4");

    //将被调函数display开始的低位赋给si
    MOV(targetCodeArea, blank, "si,", "bp");
    SUB(targetCodeArea, blank, "si,", "4");

    //开始复制L条display记录
    for (int i = 0; i < nowLevel - 1; i++)
    {
        MOV(targetCodeArea, blank, "ax,", "word ptr [bx]");
        MOV(targetCodeArea, blank, "word ptr [si],", "ax");
        //让bx和si移向下一条记录
        SUB(targetCodeArea, blank, "bx,", "2");
        SUB(targetCodeArea, blank, "si,", "2");
    }
    //复制了L-1条，剩一条补上
    MOV(targetCodeArea, blank, "ax,", "word ptr [bx]");
    MOV(targetCodeArea, blank, "word ptr [si],", "ax");
    SUB(targetCodeArea, blank, "si,", "2");

    //将被调函数的bp位置加入display
    MOV(targetCodeArea, blank, "word ptr [si],", "bp");

}

//处理返回值getResult
void TargetCodeGenerator::resultCalculation(quar nowQuar, string name)
{
    returnLabel(name);
    //寻址
    string target = nowQuar.target;
    string tarCode = "";
    tarCode = findXxx(target, blank);
    //把ax中的返回值赋给target
    //MOV(targetCodeArea, blank, tarCode + ",", "ax");
    MOV(targetCodeArea, blank, "si,","sp");
    MOV(targetCodeArea, blank, "ax,", "word ptr [si-6]");
    MOV(targetCodeArea, blank, tarCode + ",", "ax");
}

//处理返回值assignResult ？
void TargetCodeGenerator::assResultCalculation(quar nowQuar, string name)
{
    returnLabel(name);
    //寻址
    string o1 = nowQuar.o1;
    string o1Code="";
    o1Code = findXxx(o1, blank);
    //把返回值赋给ax
    MOV(targetCodeArea, blank, "ax,", o1Code);
    //把返回值存到活动记录中
    MOV(targetCodeArea, blank, "word ptr [bp-2],","ax");
}

//处理函数声明结束endFunction
void TargetCodeGenerator::endFunCalculation(quar nowQuar, string name)
{
    returnLabel(name);
    stackManager.movp(targetCodeArea, blank, "sp,", "bp");
    stackManager.pop(targetCodeArea, blank, "bp");
    //获得当前函数name，之后弹栈
    string nowFunName = itStack.back().name();
    itStack.pop_back();
    //生成ret指令
    RET(targetCodeArea, blank);
    //函数结束语句
    ENDP(targetCodeArea, nowFunName);
}

//处理传参数四元式moveTurePar或moveFalsePar
void TargetCodeGenerator::parCalculation(quar nowQuar, string name, int index, int level)
{
    returnLabel(name);
    string o1 = nowQuar.o1;
    string o1Code = "";

    ElemSheet::iterator nowIter = itStack.back().getElemIterator(o1);
    if(isNum(o1))
    {
        for(int i=1;i<o1.size();i++)
        {
            o1Code+=o1[i];
        }
    }
    else
    {
        if (nowIter.cat() == catPoint)
        {
            //计算真实偏移量,原因查看测试ifwhile文档
            int trueOffset = findTrueOffset(o1);

            //查display表确定这个变量所在的活动记录开始位置
            //表的前缀可以计算得出，所有的东西计算后变为汇编代码即可
            //查看目标操作数的层次
            int o1Level = itStack.back().getLevel(o1);

            //在display表中查找目标位置
            int o1Dis = findInDisplay(o1Level);

            //将地址装到bx中，通过[bp-xxx]找到
            string strDis = to_string(o1Dis);

            //因为地址为2个存储单元，所以为word ptr
            strDis = "word ptr [bp-" + strDis + "]";

            //装入bx中
            MOV(targetCodeArea, blank, "bx,", strDis);

            //现在bx里面有对应的开始位置的地址
            //通过偏移量可以确定位置
            //暂时没考虑优化，没省去ppt中st指令，后续可能会加
            string strTrueOffset = to_string(trueOffset);
            SUB(targetCodeArea, blank, "bx,", strTrueOffset);
            o1Code = "[bx]";
        }
        else
        {
            if (nowQuar.oper == quatName::moveTurePar)
            {
                o1Code = findXxx(o1, blank);
            }
            else
            {
                //计算真实偏移量,原因查看测试ifwhile文档
                int trueOffset = findTrueOffset(o1);

                //查display表确定这个变量所在的活动记录开始位置
                //表的前缀可以计算得出，所有的东西计算后变为汇编代码即可
                //查看目标操作数的层次
                int o1Level = itStack.back().getLevel(o1);

                //在display表中查找目标位置
                int o1Dis = findInDisplay(o1Level);

                //将地址装到bx中，通过[bp-xxx]找到
                string strDis = to_string(o1Dis);

                //因为地址为2个存储单元，所以为word ptr
                strDis = "word ptr [bp-" + strDis + "]";

                //装入bx中
                MOV(targetCodeArea, blank, "bx,", strDis);

                //现在bx里面有对应的开始位置的地址
                //通过偏移量可以确定位置
                //暂时没考虑优化，没省去ppt中st指令，后续可能会加
                string strTrueOffset = to_string(trueOffset);
                SUB(targetCodeArea, blank, "bx,", strTrueOffset);
                o1Code = "bx";
            }
        }
    }
    //存到ax中
    MOV(targetCodeArea, blank, "ax,", o1Code);

    //获得被调函数层次号
    int nowOffset = 2 + 2 + 2 + (level + 1) * 2 + 2 * (index+1);
    MOV(targetCodeArea, blank, "si,", "sp");
    string strNow = "word ptr [si-" + to_string(nowOffset) + "]";
    MOV(targetCodeArea, blank, strNow + ",", "ax");
}

//处理函数调用Call
void TargetCodeGenerator::callCalculation(quar nowQuar, string name)
{
    returnLabel(name);
    //这里应该调用函数迭代器，用来获取函数层次
    FunSheet::iterator callIter = itStack.back().getFunIterator(nowQuar.o1);
    //获得被调函数的层次，计算display长度
    int callLevel = callIter.level();


    //将入栈的四元式生成代码
    for (int i = 0; i < paraColl.size(); i++)
    {
        parCalculation(paraColl[i], blank, i, callLevel);
    }
    paraColl.clear();

    //生成call指令
    CALLP(targetCodeArea, blank, nowQuar.o1);
}

//处理结构体取地址getSonAddres
void TargetCodeGenerator::getSonCalculation(quar nowQuar, string name)
{
    returnLabel(name);
    //计算真实偏移量,原因查看测试ifwhile文档
    int trueOffset = findTrueOffset(nowQuar.o1);

    //查display表确定这个变量所在的活动记录开始位置
    //表的前缀可以计算得出，所有的东西计算后变为汇编代码即可
    //查看目标操作数的层次
    int o1Level = itStack.back().getLevel(nowQuar.o1);

    //在display表中查找目标位置
    int o1Dis = findInDisplay(o1Level);

    //将地址装到bx中，通过[bp-xxx]找到
    string strDis = to_string(o1Dis);

    //因为地址为2个存储单元，所以为word ptr
    strDis = "word ptr [bp-" + strDis + "]";

    //装入si中
    MOV(targetCodeArea, blank, "si,", strDis);

    //si先减去真实偏移量
    string strTrueOffset = to_string(trueOffset);
    SUB(targetCodeArea, blank, "si,", strTrueOffset);

    //获取的是针对结构体第一个的偏移量，第一个是0，第二个是2
    //之后不能加1，因为已经指向了低位
    int sonOffset = itStack.back().getElemIterator(nowQuar.o1).type().sonOffSet(nowQuar.o2);
    //再减去域名偏移量
    string strSon = to_string(sonOffset);
    SUB(targetCodeArea, blank, "si,", strSon);
    MOV(targetCodeArea, blank, "ax,", "si");

    //之后寻址target
    string tarCode = findBpxxx(nowQuar.target, blank);
    //string tarCode = findXxx(nowQuar.target, blank);
    MOV(targetCodeArea, blank, tarCode + ",", "ax");
}

//处理输入
void TargetCodeGenerator::inCalculation(quar nowQuar, string name)
{
    returnLabel(name);
    //清空
    MOV(targetCodeArea, blank, "ax,", "0");
    MOV(targetCodeArea, blank, "N,", "ax");
    //将存储单元的地址赋给si
    LEA(targetCodeArea, blank, "si,", "N");
    CALLP(targetCodeArea, blank, "READ10");
    MOV(targetCodeArea, blank, "ax,", "N");

    //将ax的值赋给操作数
    string target = nowQuar.target;
    string tarCode = findXxx(target, blank);
    //赋值
    MOV(targetCodeArea, blank, tarCode+",", "ax");

}

//处理输出
void TargetCodeGenerator::outCalculation(quar nowQuar, string name)
{
    returnLabel(name);
    //寻址
    string o1 = nowQuar.o1;
    string o1Code = findXxx(o1, blank);
    MOV(targetCodeArea, blank, "ax,", o1Code);

    //call
    CALLP(targetCodeArea, blank, "PrintAx");
}

//处理结束程序
void TargetCodeGenerator::programEnd(quar nowQuar, string name)
{

    returnLabel(name);
    //生成结束中断语句等等阴间东西
    //函数栈弹栈
    itStack.pop_back();
    code nowCode;

    //结束中断
    nowCode.name = blank, nowCode.oper = "mov", nowCode.dest = "ah,", nowCode.source = "4ch";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";


    nowCode.name = blank, nowCode.oper = blank, nowCode.dest = "int", nowCode.source = "21h";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";


    nowCode.name = "cseg", nowCode.oper = "ends", nowCode.dest = "", nowCode.source = "";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";


    nowCode.name = blank, nowCode.oper = "end", nowCode.dest = "start", nowCode.source = "";
    targetCodeArea.push_back(nowCode);
    nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
}

//生成目标代码
void TargetCodeGenerator::generateCode()
{
    for (int i = 0; i < BaseBlockColl.size(); i++)
    {
        BaseBlockColl[i].checkBlockActive();
    }

    //打开目标文件
    asmout.open("../Files/target.asm", ios::out);
    if (!asmout.is_open())
    {
        cout << "Open Failed!!!" << endl;
        exit(0);
    }

    //初始化数据段，堆栈段
    initAsm();

    //遍历所有基本块
    for (int i = 0; i < BaseBlockColl.size(); i++)
    {
        //遍历基本块中四元式
        for (int j = 0; j < BaseBlockColl[i].BaseBlockQuar.size(); j++)
        {
            //取得当前四元式操作数
            quar nowQuar = BaseBlockColl[i].BaseBlockQuar[j];

            //对应program xxx
            if (nowQuar.oper == quatName::beginProgram)
            {
                if(!itStack.size())
                {
                    itStack.push_back(xTable.getMain());
                } else
                {
                    FunSheet::iterator nowIt;
                    nowIt = itStack[itStack.size()-1];
                    itStack.push_back(nowIt.getFunIterator(nowQuar.o1));
                }
                itStack.back().setOffSet(0);
                //四元式序号++

            }

            //操作符为程序开始时调用对应函数处理,对应
            if (nowQuar.oper == quatName::beginFunction)
            {
                if (itStack.size() == 1)
                {
                    programBegin(nowQuar);
                }
                else
                {
                    string nowName = "";
                    nowName = getName(prevQuarNum);
                    beginFunCalculation(nowQuar);
                }

            }

            //操作符为赋值时调用对应函数处理
            if (nowQuar.oper == quatName::Assign)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                assignCalculation(nowQuar, nowName);

            }

            //操作符为加减时调用对应函数处理
            if (nowQuar.oper == quatName::ADD ||nowQuar.oper == quatName::SUB)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                addCalculation(nowQuar, nowName);

            }

            //操作符为乘时调用对应函数处理,乘和除可以放在一起，就改几行就行
            if (nowQuar.oper == quatName::MUL)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                mulCalculation(nowQuar, nowName);

            }

            //操作符为除时调用对应函数处理
            if (nowQuar.oper == quatName::DIV)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                divCalculation(nowQuar, nowName);

            }

            //操作符为关系运算时调用对应函数处理
            if (nowQuar.oper == quatName::GT || nowQuar.oper == quatName::GE ||
                nowQuar.oper == quatName::LT || nowQuar.oper == quatName::LE ||
                nowQuar.oper == quatName::EQ || nowQuar.oper == quatName::NEQ)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                relCalculation(nowQuar, nowName);

            }

            //操作符为if时调用对应函数处理
            if (nowQuar.oper == quatName::beginIf)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                ifCalculation(nowQuar, nowName);

            }

            //操作符为else时调用对应函数处理
            if (nowQuar.oper == quatName::Else)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                elCalculation(nowQuar, nowName);

            }

            //操作符为endif时调用对应函数处理
            if (nowQuar.oper == quatName::endIf)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                endIfCalculation(nowQuar, nowName);

            }

            //操作符为beginWhile时调用对应函数处理
            if (nowQuar.oper == quatName::beginWhile)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                whCalculation(nowQuar, nowName);

            }

            //操作符为Do时调用对应函数处理
            if (nowQuar.oper == quatName::Do)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                doCalculation(nowQuar, nowName);

            }

            //操作符为endWhile时调用对应函数处理
            if (nowQuar.oper == quatName::endWhile)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                weCalculation(nowQuar, nowName);

            }

            //操作符为getAddress时调用对应函数处理
            if (nowQuar.oper == quatName::getAddress)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                getAdrCalculation(nowQuar, nowName);

            }

            //操作符为getResult时调用对应函数处理
            if (nowQuar.oper == quatName::getResult)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                resultCalculation(nowQuar, nowName);

            }

            //操作符为assignResult时调用对应函数处理
            if (nowQuar.oper == quatName::assignResult)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                assResultCalculation(nowQuar, nowName);

            }

            //处理传参四元式，直接压栈
            if (nowQuar.oper == quatName::moveTurePar || nowQuar.oper == quatName::moveFalsePar)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                if(nowName!=blank&&nowName!="")
                {
                    NOP(targetCodeArea, nowName);
                }
                paraColl.push_back(nowQuar);

            }

            //处理call四元式
            if (nowQuar.oper == quatName::Call)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                callCalculation(nowQuar, nowName);

            }

            //处理结构体寻址
            if (nowQuar.oper == quatName::getSonAddres)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                getSonCalculation(nowQuar, nowName);

            }

            //操作符为程序结束时调用对应函数处理
            if (nowQuar.oper == quatName::endFunction)
            {
                if (itStack.size() == 1)
                {
                    string nowName = "";
                    nowName = getName(prevQuarNum);
                    programEnd(nowQuar, nowName);
                }
                else
                {
                    string nowName = "";
                    nowName = getName(prevQuarNum);
                    endFunCalculation(nowQuar, nowName);
                }

            }

            //处理输入
            if(nowQuar.oper == quatName::Input)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                inCalculation(nowQuar, nowName);
            }

            //处理输出
            if(nowQuar.oper == quatName::Output)
            {
                string nowName = "";
                nowName = getName(prevQuarNum);
                outCalculation(nowQuar, nowName);
            }

            code nowCode;
            nowCode.name = nowCode.oper = nowCode.dest = nowCode.source = "";
            targetCodeArea.push_back(nowCode);

            prevQuarNum++;
        }

        //清空活跃表
        isActive.clear();
    }
    int a = 1;
    for (int i = 0; i < targetCodeArea.size(); i++)
    {
        if(i==8)
        {
            asmout<<endl<<"PrintAX proc\n"
                          "     push si\n"
                          "     push cx\n"
                          "     push dx    \n"
                          "     mov si,offset Temp+3;保存存储结果的字符串的最后一个字符偏移地址\n"
                          "     xor cx,cx           ;对cX清零\n"
                          "     mov cl,4            ;设置循环次数为4次\n"
                          " MainPart: mov DH,AL           ;将Al的内容传送给DH\n"
                          "     shr AX,1\n"
                          "     shr AX,1\n"
                          "     shr AX,1\n"
                          "     shr AX,1\n"
                          "     and dh,0FH\n"
                          "     add dh,30H\n"
                          "     cmp dh,':' \n"
                          "     ja isLetter \n"
                          "     jb No       \n"
                          " isLetter: add dh,7H   \n"
                          " No: mov [si],dh \n"
                          "     dec si\n"
                          " loop MainPart\n"
                          "print:   mov dx,offset Temp \n"
                          "     mov ah,09\n"
                          "     int 21H\n"
                          "     pop dx\n"
                          "     pop cx\n"
                          "     pop si\n"
                          "     ret\n"
                          " PrintAX endp"<<endl<<endl;

            asmout<<"READ10 PROC NEAR\n"
                    "     XOR DX,DX\n"
                    "     MOV BX,10\n"
                    "RD0:\n"
                    "     MOV AH,1\n"
                    "     INT 21H\n"
                    "     CMP AL,13  ;输入回车结束\n"
                    "     JZ RT0\n"
                    "     AND AX,0FH  ;将AH清零\n"
                    "     XCHG AX,[SI]\n"
                    "     MUL BX      ;将前一位数字乘十和下一位数字相加\n"
                    "     ADD AX,[SI]\n"
                    "     XCHG AX,[SI]\n"
                    "     JMP RD0 \n"
                    "\n"
                    "RT0:     RET\n"
                    "READ10 ENDP"<<endl;
        }
        if(     targetCodeArea[i].name==""&&targetCodeArea[i].name==""&&
                targetCodeArea[i].name==""&&targetCodeArea[i].name=="")
        {
            asmout<<endl;
            continue;
        }

        if (targetCodeArea[i].name.size() > 0)
        {
            asmout << targetCodeArea[i].name;
            if (targetCodeArea[i].name[0] == 'x')
                asmout << ":" + blank;
        }
        if (targetCodeArea[i].oper.size() > 0)
            asmout << " " << targetCodeArea[i].oper;
        if (targetCodeArea[i].dest.size() > 0)
            asmout << " " << targetCodeArea[i].dest;
        if (targetCodeArea[i].source.size() > 0)
            asmout << " " << targetCodeArea[i].source;
        asmout << endl;
    }
    asmout.close();
    cout<<"generate succeed!"<<endl;
}

//判断一个字符串是否是数字
int TargetCodeGenerator::isNum(string nowStr)
{
    int isnum = 1;
    if(nowStr[0]!='0')
        isnum = 0;
    return isnum;
}

//进行寻址操作
string TargetCodeGenerator::findBpxxx(string nowOper, string name)
{
    //计算真实偏移量,原因查看测试ifwhile文档
    int trueOffset = findTrueOffset(nowOper);

    //查display表确定这个变量所在的活动记录开始位置
    //表的前缀可以计算得出，所有的东西计算后变为汇编代码即可
    //查看目标操作数的层次
    int o1Level = itStack.back().getLevel(nowOper);

    //在display表中查找目标位置
    int o1Dis = findInDisplay(o1Level);

    //将地址装到bx中，通过[bp-xxx]找到
    string strDis = to_string(o1Dis);

    //因为地址为2个存储单元，所以为word ptr
    strDis = "word ptr [bp-" + strDis + "]";

    //装入bx中
    MOV(targetCodeArea, name, "bx,", strDis);

    //现在bx里面有对应的开始位置的地址
    //通过偏移量可以确定位置
    //暂时没考虑优化，没省去ppt中st指令，后续可能会加
    string strTrueOffset = to_string(trueOffset);
    //生成字符串
    strTrueOffset = "word ptr [bx-" + strTrueOffset + "]";

    return strTrueOffset;
}

//生成指针对应的操作数代码
string TargetCodeGenerator::findPointxxx(string nowOper, string name)
{
    //计算真实偏移量,原因查看测试ifwhile文档
    int trueOffset = findTrueOffset(nowOper);
    string strOffset = to_string(trueOffset);

    //将当前单元的值存入bx,是一个真实地址
    strOffset = "word ptr [bp-" + strOffset + "]";
    MOV(targetCodeArea, name, "bx,", strOffset);

    //之后通过bx的值间接寻址
    string strPointOffset = "word ptr [bx]";

    return strPointOffset;
}

//封装三种寻址形式
string TargetCodeGenerator::findXxx(string nowOper, string name)
{
    string nowCode = "";
    if (isNum(nowOper))		//如果是数字,则只需将其放到寄存器中
    {
        //计算源操作数形式
        for(int i=1; i < nowOper.size(); i++)
            nowCode += nowOper[i];
        //更改nowRdlTemp的值
        nowRdlTemp = "const";	//const代表常值
    }
    else
    {
        ElemSheet::iterator nowIter = itStack.back().getElemIterator(nowOper);
        CAT nowCat = nowIter.cat();
        if(nowIter.cat() == catVn || nowIter.cat() == catPoint)
        {
            nowCode = findPointxxx(nowOper, name);
            nowRdlTemp = nowOper;
        }
        else
        {
            //判断是否是参数，采取对应的操作
            //现在不用了，直接寻址就行
            nowCode = findBpxxx(nowOper, name);
            //更改nowRdlTemp的值
            nowRdlTemp = nowOper;    //变量名
        }
    }
    return nowCode;
}

//将关系转换成汇编操作符，如LT对应JL
string TargetCodeGenerator::getRelAsm(string relOper)
{
    const int relNum = 6;
    string rel[6] = { "LT","LE","GT","GE","EQ","NEQ" };
    string asmRel[6] = { "JL","JLE","JG","JGE","JE","JNE" };

    for (int i = 0; i < relNum; i++)
    {
        if (relOper == rel[i])
            return asmRel[i];
    }
    return "take it boy";
}

//用于判断之前是否是需要生成标号，if用这个
string TargetCodeGenerator::getName(int prevQuarName)
{
    string nowName = "";
    /*if (AllQuarColl[prevQuarNum].oper == quatName::Else)
    {
        nowName = "x" + to_string(labelId);
        labelId++;
    }
    else if (AllQuarColl[prevQuarNum].oper == quatName::endIf)
    {
        nowName = "x" + to_string(labelId);
        labelId++;
    }*/
    if(ifJmpName!="" &&ifJmpName!=blank)
    {
        nowName = ifJmpName;
        ifJmpName="";
    }
    else if (AllQuarColl[prevQuarNum].oper == quatName::endWhile)
    {
        nowName = "x" + to_string(labelId);
        labelId++;
    }
    else if (AllQuarColl[prevQuarNum + 1].oper == quatName::beginWhile)
    {
        nowName = "x" + to_string(labelId);
        labelId++;
    }
    else
    {
        nowName = blank;
    }
    return nowName;
}

//如果需要的话生成标号，之后反填,if使用
void TargetCodeGenerator::returnLabel(string name)
{
    if (name != blank && name != "")
    {
        //假跳的目标
        NOP(targetCodeArea, name);
        //弹对应的栈
        int nowJmp = 0;
        if (AllQuarColl[prevQuarNum].oper == quatName::Else)
        {
            nowJmp = ifSEM[ifSEM.size() - 1];
            ifSEM.pop_back();
        }
        else if (AllQuarColl[prevQuarNum].oper == quatName::endIf)
        {
            nowJmp = ifSEM[ifSEM.size() - 1];
            ifSEM.pop_back();
        }
        else if (AllQuarColl[prevQuarNum].oper == quatName::endWhile)
        {
            nowJmp = doSEM[doSEM.size() - 1];
            doSEM.pop_back();
        }
        targetCodeArea[nowJmp].source = name;
    }
}
