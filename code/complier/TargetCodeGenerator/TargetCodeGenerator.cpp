#include"TargetCodeGenerator.h"
#include<iostream>
#include<algorithm>


using std::ios;
using std::cout;
using std::endl;


//���ļ��ж�ȡ��Ԫʽ
void TargetCodeGenerator::read(string filename)
{
	ifstream infile;
	infile.open("../quar.txt", ios::in);
	if (!infile.is_open())
	{
		cout << "Open Failed!!!" << endl;
		exit(0);
	}

	string line, th[4];
	while (!infile.eof())
	{
		std::getline(infile, line);

		//bookΪ��ʼ���Ƶ�λ�ã�turnΪ�����ĸ���������0��1��2��3��
		int book = 1, turn = 0;	

		for (int i = 1; i < line.size() - 1; i++)
		{
			if (line[i] == ',')
			{
				th[turn] = "";
				for (int j = book; j < i; j++)
				{
					th[turn] += line[j];
				}
				turn++;
				book = i + 1;
			}
		}

		//��Ϊ�������һ�����ž�ͣ���ˣ�����Ҫ������Ŀ��������ٴ���һ��
		th[turn] = "";
		for (int i = book; i < line.size() - 1; i++)
		{
			th[turn] += line[i];
		}

		//����������Ԫʽ����
		quar tq;
		tq.oper = th[0], tq.o1 = th[1], tq.o2 = th[2], tq.target = th[3];
		AllQuarColl.push_back(tq);
	}
	int a = 1;
}

//�Ƿ�����ת����Ŀǰ��if��else��while��we
//���򷵻�true�����򷵻�false
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

//�ָ���ԪʽΪ�����飬��������鼯��
void TargetCodeGenerator::spiltMidCode()
{
	quar nowQuar;
	set<int> BeginQuarColl;		//���������ָ���ż�����0��ʼ
	BeginQuarColl.insert(0);	//��һ����Ԫʽ����
	
	//����������Ԫʽ
	for (int i = 1; i < AllQuarColl.size(); i++)
	{

		//�����һ������ת��䣬�򽫴�������
		if (isSpilt(AllQuarColl[i - 1]))
		{
			BeginQuarColl.insert(i);
		}
		//���У�while��ʼ��ԪʽҲ�ǿ�ʼ��Ԫʽ
		if (AllQuarColl[i].oper == "beginWhile")
		{
			BeginQuarColl.insert(i);
		}
	}

	//��ǰ������
	BaseBlock nowBlock;

	//����ÿһ����ţ��Ե�ǰ�����俪ʼ����һ�������������������������ʼ����
	for (auto g = BeginQuarColl.begin(); g != prev(BeginQuarColl.end()); g++)
	{
		nowBlock.BaseBlockQuar.clear();

		auto next = g;
		int nowBegin = *g;				//��ʼ
		int nowEnd = *(++next);			//����

		//�����鿪ʼ�ͽ�����λ�ñ��
		nowBlock.BaseBlockStart = nowBegin;
		nowBlock.BaseBlockEnd = nowEnd - 1;
		
		//��ʼ����
		for (int i = nowBegin; i < nowEnd; i++)
		{
			nowBlock.BaseBlockQuar.push_back(AllQuarColl[i]);
		}

		//��������鼯
		BaseBlockColl.push_back(nowBlock);
	}

	//ʣ�����һ����ʼ���ֱ��������Ԫʽ���������Բ���
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


//�����ã�ʵ��Ӧ��ʹ�÷��ű�ĺ���
bool isTemporary(string nowString)
{
	if (nowString[0] == 't')
		return true;
	return false;
}

//�жϲ������Ƿ��Ǻ������ã���������������������������������������������������������̽���
int BaseBlock::isHaveActivity(string nowOp)
{
	for (int i = 0; i < notAcitvityCount; i++)
	{
		if (nowOp == notAcitvity[i])
			return 1;
	}
	return 0;
}

//�жϲ������Ƿ��ǿո�
int BaseBlock::isBlankOper(string nowOper)
{
	if (nowOper == "_")
		return 1;
	return 0;
}

//��ʼ���������Ծ��Ϣ�����ջ���������Ԫʽ�������ɶ�Ӧ�����Ļ�Ծ��Ϣ�ṹ��
//��ʱ������ʼ��Ϊno��������ʼ��Ϊyes
void BaseBlock::initBlockActive()
{
	for (int i = 0; i < BaseBlockQuar.size(); i++)
	{
		//��ǰ��Ԫʽ�Ļ�Ծ��Ϣ�����ûɶӰ�죬����ֱ�Ӹ���
		active nowActivity;
		nowActivity.oper = BaseBlockQuar[i].oper;
		
		//�鿴�Ƿ�����ʱ����
		string checkTemp = "";

		//�鿴�Ƿ��ǿյ�
		if (!isBlankOper(BaseBlockQuar[i].o1))		//�ǿգ�����
		{
			checkTemp = BaseBlockQuar[i].o1;
			if (isTemporary(checkTemp))				//�鿴�Ƿ�����ʱ����
			{
				nowActivity.o1 = "no";				//��ʱ����Ϊno
				tempVariable[checkTemp] = "no";
			}
			else
			{
				nowActivity.o1 = "yes";				//����Ϊyes
				notTempVariable[checkTemp] = "yes";
			}
		}

		//�ڶ�����������ͬ��
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

		//Ŀ���������ͬ��
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

		BaseBlockActivity.push_back(nowActivity);	//�洢
		int a = 1;
	}
	
}

//�����Ծ��Ϣ���õ��Ͽ�ppt�ķ���
void BaseBlock::checkBlockActive()
{
	//���ȳ�ʼ����ǰ������Ļ�Ծ��Ϣ
	initBlockActive();
	
	int a = 1;
	//��ʼ���Ծ��Ϣ��������
	for (int i = BaseBlockActivity.size() - 1; i >= 0; i--)
	{
		if (isHaveActivity(BaseBlockQuar[i].oper))
			continue;

		string nowVariable = "";
		string changeMessage = "";
		
		if (!isBlankOper(BaseBlockQuar[i].target))
		{
			nowVariable = BaseBlockQuar[i].target;		//��ǰ������
			if (isTemporary(nowVariable))						//�������ʱ�����Ĳ���
			{
				changeMessage = tempVariable[nowVariable];		
				BaseBlockActivity[i].target = changeMessage;	//�ӱ����ҵ�ֵ��������Ծ��Ϣ��
				tempVariable[nowVariable] = "no";				//Ŀ���������Ϊno
			}
			else												//���������ʱ�����Ĳ���
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
				BaseBlockActivity[i].o2 = changeMessage;		//�ӱ����ҵ�ֵ��������Ծ��Ϣ��
				tempVariable[nowVariable] = "yes";				//������2��Ϊyes
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
				BaseBlockActivity[i].o1 = changeMessage;		//�ӱ����ҵ�ֵ��������Ծ��Ϣ��
				tempVariable[nowVariable] = "yes";				//������1��Ϊyes
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

//���ұ�������ڱ����洢��ʼλ�õ�ƫ�����������ã�ʵ��ʹ����Ҫ���ű�ĺ���
//���Һ�Ҫ������������old spʲô�ģ�
int findTempOffset(string tempName)
{
	//��������ڴ洢��������ƫ������ָ���λ
	if (tempName == "a")
		return 0;
	if (tempName == "b")
		return 2;
	if (tempName == "c")
		return 4;
	if (tempName == "t1")
		return 6;
	if (tempName == "t2")
		return 8;
	if (tempName == "t3")
		return 10;
	if (tempName == "t4")
		return 12;
}

//�鿴�����Ĳ�Σ���0��ʼ��������
int findTempLevel(string tempName)
{
	return 0;
}

//��������洢��ʼ��Ԫ�����old sp�洢λ�õ�ƫ�����������������Ӧ��ʵ�ֵ�
int TargetCodeGenerator::findBpOffset(string tempName)
{
	//�鿴��Σ���ʱʹ�ò��Ժ���
	int tempLevel = findTempLevel(tempName);

	//old sp(2)+����ֵ(2)+display((���+1)*2)
	//���ڱ���ƫ��λ�ø���ʱҲ�Ǵ��㿪ʼ������һ��ʼ����ֱ�Ӽ�2
	int spOffset = 2 + 2 + 2 * (tempLevel + 1);
	return spOffset;
}

//�������������������ڲ�sp��ƫ�����������������Ӧ��ʵ�ֵ�
int TargetCodeGenerator::findTrueOffset(string tempName)
{
	int trueOffset = findBpOffset(tempName) + findTempOffset(tempName);
	return trueOffset;
}

//�������ݶκͶ�ջ��,��ʼ��,��׼�����ɴ���ʱ����
void TargetCodeGenerator::initAsm()
{
	//�������ݶ���Ϊ�������ݶ����ڶ�ջ�Σ��������ݶ�Ϊ��
	asmout << "dseg segment" << endl;
	asmout << "dseg ends" << endl << endl;

	//�������ݶ�
	asmout << "sseg segment	stack" << endl;
	asmout << "	        dw 512 dup(?)" << endl;
	asmout << "sseg ends" << endl << endl;

	//��ʼ����Σ���ʼ�����ݶΣ���ջ�Σ������
	asmout << "cseg segment" << endl;
	asmout << "	        assume cs:cseg,ds:dseg" << endl;
	asmout << "	assume ss:sseg" << endl;
	asmout << "start:	mov ax,sseg" << endl;	//�˴�����θ���
	asmout << "	        mov ds,ax" << endl;
	asmout << "	        mov ax,sseg" << endl;
	asmout << "	        mov ss,ax" << endl << endl;

}

//������ݴ洢�����ȣ�������
int funDataLen()
{
	return 14;
}

int funLevel()
{
	return 0;
}

//�������ʼ��Ԫʽ
void TargetCodeGenerator::programBegin(quar nowQuar)
{
	//���������Ӧ��ͨ�����ű�Ľӿڣ������ű���programbegin����
	//��������ȷŷţ��ȵ����۵�ʱ������

	//��ʼ�������Ķ�ջ�ռ�
	//���Ƚ�bp���룬Ϊold sp
	asmout << "	        ";			//Ϊ�˻��������
	stackManager.push(asmout, "bp");

	//��bp�Ƶ�sp��λ��
	asmout << "	        ";			//Ϊ�˻��������
	stackManager.movp(asmout, "bp", "sp");

	//ȷ����Ҫ�Ĵ洢��Ԫ�����ȷ�����ݴ洢���ĳ��Ⱥ���㣬old sp�Ѿ������ˣ��Ͳ�������
	//(����ֵ(2)+display((��ǰ���������+1)*2)+���ݴ洢�����ȣ����óˣ�����ֱ�ӵõ��洢��Ԫ��))
	int dataLenth = funDataLen();
	int nowLevel = funLevel();
	int storeLenth = 2 + (nowLevel + 1) * 2 + dataLenth;
	
	//ͨ����Сsp�����ٴ洢�ռ�
	asmout << "	        ";			//Ϊ�˻��������
	stackManager.subSp(asmout, to_string(storeLenth));

	//����display��֮ǰ�Ѿ���ȡ����ǰ����ˣ�ֱ���þ�����
	//��Ϊ�ǳ���ʼ������ֱ��2���洢��Ԫ��ֵ������
	asmout << "	        ";			//Ϊ�˻��������
	MOV(asmout, "[bp+5]", "bp");

}

//����ֵ
void TargetCodeGenerator::assignCalculation(quar nowQuar)
{
	string o1 = nowQuar.o1;

	//�жϲ�����1�Ƿ�������
	int isNum = 1;
	for (int i = 0; i < o1.size(); i++)
	{
		if (o1[i]<'0' || o1[i]>'9')
		{
			isNum = 0;
			break;
		}
	}

	if (isNum)		//���������
	{
		string o2 = nowQuar.o2;

	}
}

//����Ӽ��˳�
void TargetCodeGenerator::numCalculation(quar nowQuar)
{
	string nowOper = nowQuar.oper;

	if (nowOper == "ADD")
	{

	}
}

//����Ŀ�����
void TargetCodeGenerator::generateCode()
{
	for (int i = 0; i < BaseBlockColl.size(); i++)
	{
		BaseBlockColl[i].checkBlockActive();
	}
	int a = 1;

	//��Ŀ���ļ�
	asmout.open("../target.asm", ios::out);
	if (!asmout.is_open())
	{
		cout << "Open Failed!!!" << endl;
		exit(0);
	}

	//��ʼ�����ݶΣ���ջ��
	initAsm();

	//�������л�����
	for (int i = 0; i < BaseBlockColl.size(); i++)
	{
		//��������������Ԫʽ
		for (int j = 0; j < BaseBlockColl[i].BaseBlockQuar.size(); j++)
		{
			//ȡ�õ�ǰ��Ԫʽ������
			quar nowQuar = BaseBlockColl[i].BaseBlockQuar[i];

			//������Ϊ����ʼʱ���ö�Ӧ��������
			if (nowQuar.oper == "beginprogram")
			{
				programBegin(nowQuar);
				//������break��������ɾ��
				break;
			}

			//������Ϊ�Ӽ��˳�ʱ���ö�Ӧ��������
			if (nowQuar.oper == "ADD" ||nowQuar.oper == "SUB" ||
				nowQuar.oper == "MUL" ||nowQuar.oper == "DIV")
			{
				numCalculation(nowQuar);
			}
		}

		//������break��������ɾ��
		break;
	}
	asmout.close();
}