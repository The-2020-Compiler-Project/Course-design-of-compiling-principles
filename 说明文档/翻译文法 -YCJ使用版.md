### 标识符定义性文法

<程序>					::=    program <标识符>$\color{#0000FF}{【创建主函数】}$$\color{red}{【函数迭代器,pushFunStack】}$$\color{red}{【-，beginProgram】}$：<分程序>  .

$\color{red}{【函数迭代器,pushFunStack】}$===函数迭代器压入函数栈，用来寻找其余的运算对象

$\color{red}{【-，beginProgram】}$===生成函数定义开始的四元式，用于目标代码生成阶段获取函数迭代器



<分程序>				::=    [<类型定义部分>] [<常量说明部分>] [<变量说明部分>] {[<过程说明部分>]|[<函数说明部分>]}$\color{red}{【-，beginFunction】}$<复合语句>$\color{red}{【-,endFunction】}$

$\color{red}{【-，beginFunction】}$：生成函数代码段开始四元式。

$\color{red}{【-,endFunction】}$：弹函数栈，生成函数定义结束四元式。



<类型定义部分>	::=	type <类型定义> ; { <类型定义> ; }



<类型定义>		    ::=	<标识符> $\color{#0000FF}{【标识符，压入类型定义标识符栈】}$= ( <数组类型定义> | <结构体类型定义> )



<数组类型定义>	::=	array  '['<无符号整数>']' $\color{#0000FF}{【记录常数值】}$of <标识符>$\color{#0000FF}{【增加数组类型定义，类型定义标识符栈弹栈】}$

$\color{blue}{【记录】}$：指的是记录下的string在本语句中被使用

$\color{blue}{【压入某某栈】}$：标识一个标识符用于其他语句



<结构体类型定义>::=	record <域名定义>; { <域名定义>;} end$\color{#0000FF}{【增加结构体类型定义，域名向量、域名类型向量清空】}$



<域名定义>		    ::=	<标识符>$\color{#0000FF}{【域名标识加入，域名向量】}$ : <类型>$\color{#0000FF}{【类型标识加入，域名类型向量】}$ 



<常量说明部分>	::=    const <常量定义>{ ,<常量定义>} ;



<常量定义>			::=    <标识符>$\color{#0000FF}{【记录常量标识符】}$ ＝<常量>$\color{#0000FF}{【增加常量定义】}$



<变量说明部分>	::=    var <变量说明> ; {<变量说明> ; }	



<变量说明>			::=    <标识符>$\color{#0000FF}{【 记录 标识符】}$ { , <标识符>$\color{#0000FF}{【记录标识符 】}$} : <类型>$\color{#0000FF}{【增加变量定义】}$



<过程说明部分>	::=    <过程首部><分程序>;



<过程首部>			::=    procedure<标识符>$\color{#0000FF}{【增加过程】}$$\color{red}{【函数迭代器；pushFunStack】}$$\color{red}{【-，beginProgram】}$[<形式参数表>];



<函数说明部分>	::=    <函数首部><分程序>;



<函数首部>			::=    function<标识符>[<形式参数表>] : <基本类型>;$\color{#0000FF}{【增加函数】}$ $\color{red}{【函数迭代器；pushFunStack】}$ $\color{red}{【-，beginProgram】}$



<形式参数表>		::=    '( '<形式参数段> { ; <形式参数段>} ')'



<形式参数段>		::=    [ var ] <标识符> $\color{#0000FF}{【记录标识符，注意类型的不同】}$ { , <标识符>$\color{#0000FF}{【记录标识符】}$ } : <基本类型>$\color{#0000FF}{【增加参数】}$



### 标识符使用阶段，此阶段所有的标识符类型确定

<复合语句>			::=    begin<语句>{;<语句>}end



<语句>					::=   **<输出语句>**|**<输入语句>**| <赋值语句>|<返回值语句>|<条件语句>|<调用语句>|<while循环语句>|<复合语句>|<空>

**<输出语句>** 		   ::=	output<表达式>$\color{red}{【-，output】}$

**<输入语句>**			::=	input<变量>$\color{red}{【-，input】}$

$\color{red}{【-，output】}$：运算对象弹栈，生成输出四元式。

$\color{red}{【-，input】}$：运算对象弹栈，生成输入四元式。



<赋值语句>			::=    <变量> := <表达式> $\color{red}{【-，Assign】}$

$\color{red}{【-，Assign】}$：从运算对象栈中弹出两个元素a、b，生成b赋值给a的四元式

**赋值语句是否选择扩展，给指针所指向的对象赋值（point，vN），正常赋值）？不太可行，指针、换名形参可能在多个位置；注意判断运算对象的cat，如果为cat::point,cat::vN，则增加一个计算地址的目标代码；如果为V或者VF则检测它的层次号和当前函数的层次号是否符合，如果不符合，则计算地址**

**关键在于地址计算**





<变量>					::= 	<形参/变量 类码为基本类型 的标识符>$\color{red}{【string,pushObjectStack】}$ | <变量、类码为数组 的标识符>$\color{red}{【string,pushObjectStack】}$ '['<表达式>']'$\color{red}{【-，getAddress】}$ | <变量、类码为结构体 的标识符> $\color{red}{【string,pushObjectStack】}$  .    <属于前一个标识符的域名 标识符>$\color{red}{【string,pushObjectStack】}$$\color{red}{【-，getSonAddress】}$

//变量的结果是运算对象栈+1，且一定只+1

$\color{red}{【-，getAddress】}$：从运算对象栈中弹出两个元素，生成数组取地址四元式，加入point（cat）临时变量。

$\color{red}{【-，getSonAddress】}$：从栈顶取弹出元素a、b，加入point临时变量t，生成结构体取地址四元式



<返回值语句>		::=    result := <表达式> $\color{red}{【-，assignResult】}$**//result作为特殊标识**

$\color{red}{【-，assignResult】}$：运算对象弹栈，生成result赋值语句



<条件语句>			 ::=    if<条件>then$\color{red}{【-，beginIf】}$<语句>[else$\color{red}{【-,Else】}$<语句>]$\color{red}{【-,endIf】}$



$\color{red}{【-，beginIf】}$:运算对象弹栈，生成if语句

$\color{red}{【-,Else】}$:生成四元式

$\color{red}{【-,endIf】}$：生成四元式



<条件>					 ::=    <表达式><关系运算符>$\color{red}{【string，pushOpeStack】}$<表达式>$\color{red}{【-，Relation】}$

$\color{red}{【-，Relation】}$：算符弹栈，运算对象弹栈两个，加入临时变量，生成四元式



<while循环语句>    ::=    while$\color{red}{【-，beginWhile】}$ <条件> do$\color{red}{【-，Do】}$<语句>$\color{red}{【-，endWhile】}$

$\color{red}{【-，beginWhile】}$:生成四元式

$\color{red}{【-，Do】}$：运算给对象弹栈，生成四元式

$\color{red}{【-，endWhile】}$:生成四元式



<调用语句>			 ::=	<函数调用语句>$\color{red}{【-，popObjectStack】}$|<过程调用语句>

$\color{red}{【-，popObjectStack】}$：单纯的调用语句的返回值没用，所以弹栈



<函数调用语句>     ::=    <函数 标识符>$\color{red}{【string，pushFunCallStack】}$[<实在参数表>]$\color{red}{【-,moveParameter】}$$\color{red}{【-，Call】}$$\color{red}{【-，getRes ult】}$$\color{red}{【-，popFunCallStack】}$



$\color{red}{【string，pushFunCallStack】}$：将函数表示符压栈，用于函数调用语句生成参数传递

$\color{red}{【-,moveParameter】}$：读取函数调用标识符栈的栈顶，判断参数的个数与类型，然后弹运算对象栈，调整顺序，生成相对应的参数传递四元式，传值、传偏移量。

$\color{red}{【-，Call】}$：读取函数调用栈栈顶，生成函数调用四元式

$\color{red}{【-，getRes ult】}$：用函数调用栈的栈顶获取函数信息，生成对应类型的临时变量，然后生成获取返回值的四元式，临时变量符号压入操作对象栈

$\color{red}{【-，popFunCallStack】}$：函数标识符没用了，弹栈



<过程调用语句>     ::=    <过程 标识符>$\color{red}{【string，pushFunCallStack】}$[<实在参数表>]$\color{red}{【-,moveParameter】}$$\color{red}{【-，Call】}$$\color{red}{【-，popFunCallStack】}$



<实在参数表>		 ::=    '(' <实在参数> { ,<实在参数>} ')'



<实在参数>			 ::=    <表达式> 		$\color{red}{//表达式部分已经压栈}$





//表达式最终一定会而且只会产生一个变量加入栈中

<表达式>				::=    <项> {<加法运算符>$\color{red}{【string，pushOpeStack】}$<项>$\color{red}{【-，Arithmetic】}$ }

$\color{red}{【-，Arithmetic】}$：弹栈顶运算符、弹栈顶两个操作数，生成一个中间变量压栈，生成四元式



<项>						::=    <因子> {<乘法运算符>$\color{red}{【string，pushOpeStack】}$<因子>$\color{red}{【-，Arithmetic】}$}



<因子>					::=   <变量>|<类码为整型 常量标识符>$\color{red}{【0+string,pushObjectStack】}$|<无符号整数>$\color{red}{【0+string,pushObjectStack】}$|'('<表达式>')'|<函数调用语句>



$\color{red}{【标识符取值压栈】}$：将标识符的常量值压入运算对象栈，这样运算对象中存在变量标识符/常量值，无需做区分

$\color{red}{【0+string,pushObjectStack】}$：常数string压栈



<类型>					::=    <基本类型> |<类型 标识符>

<基本类型>			::=    integer|char|bool

<布尔值>				::=    true|false





//可以通过token直接区分出来的单词

<加法运算符>		::=    +|-

<乘法运算符>		::=    \*|/

<关系运算符>		::=    <|<=|>|>=|=|<>

<字母>					::=    a|b|c|d…x|y|z |A|B…|Z

<数字>					::=    0|1|2|3…8|9

<常量>					::=    [-] <无符号整数>|<字符>

<字符>					::=    '<字母>'|'<数字>'

<字符串>				::=    "{十进制编码为32,33,35-126的ASCII字符}"

<无符号整数>		::=    <数字>{<数字>}

<标识符>				::=    <字母>{<字母>|<数字>}



## 规律

四元式生成的语义动作好像只需要加载标识符后边以及广义的算符后面



## 翻译文法部分的设计

翻译文法的接口以类的形式实现、依赖于文法分析的结果，在文法分析的语义动作后执行

完成的工作有：四元式生成、语义分析

重复定义的检查在文法分析阶段可以完成大部分



关键字增加 result,true,false,call



```
program ex;
　　var
　　　　x:real;
　　　　y:integer;
　　　　funciont  x( a,b,c,d,e):bool;
　　　　result:=123;
　　　　begin
　　　　end;
　　　　function t(a,b,c,d):bool;
　　　　begin
　　　　end;
　　begin
　　
　　	
	end.
	
	
program ex;
　　var
　　　　x:real;
　　　　y:integer;
　　begin
　　　　if x>0　then y:=1｛x>0时，y的值为1｝
　　　　　else ｛x≤0时｝
　　　　　　if x=0　then y:=0
　　　　　　else y:=-1;
　　　　writeln('x=',x:6:2,'y=',y);
	end.
```

