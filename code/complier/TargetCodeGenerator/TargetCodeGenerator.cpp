#include"TargetCodeGenerator.h"
#include<iostream>
#include<fstream>
#include<algorithm>


using std::ifstream;
using std::ofstream;
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
		int inparent = 0;	//�Ƿ��������У�����������������Ķ���

		for (int i = 1; i < line.size() - 1; i++)
		{
			if (line[i] == '(')
			{
				inparent = 1;
				continue;
			}
			if (line[i] == ')')
			{
				inparent = 0;
				continue;
			}
			if (line[i] == ','&&!inparent)
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

		pair<string, string> nowPair[4];
		//����o1��o2��target�����ݣ����ַ�����Ϊpaie<string,string>
		for (int i = 1; i <= 3; i++)
		{
			string first="", second="";		//��һ���͵ڶ����ַ���
			int isSecond = 0, pis = 1;		//�Ƿ��ǵڶ����ַ����͵ڶ����ַ�����ʼλ��
			for (int j = 1; j < th[i].size() - 1; j++)
			{
				if (th[i][j] == ',')					//��һ���ַ�����ͷ��
				{
					for (int k = 1; k < j; k++)
					{
						first += th[i][k];
					}
					pis = j + 1;
					isSecond = 1;
				}
				if (isSecond && j == th[i].size() - 2)	//�ڶ����ַ���&&��ͷ��
				{
					for (int k = pis; k <= j; k++)
					{
						second += th[i][k];
					}
				}
			}
			nowPair[i] = make_pair(first, second);
		}

		//����������Ԫʽ����
		quar tq;
		tq.oper = th[0], tq.o1 = nowPair[1], tq.o2 = nowPair[2], tq.target = nowPair[3];
		AllQuarColl.push_back(tq);
	}
}

//�Ƿ�����ת����Ŀǰ��if��else��while��we
//���򷵻�true�����򷵻�false
bool TargetCodeGenerator::isJump(quar nowQuar)
{
	for (int i = 0; i < JumpQuarSize; i++)
	{
		if (nowQuar.oper == Jump[i])
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
		/*��ʱ���ϣ���Ԫʽ�в������Ŀ����Ԫʽ

		//�������ת��䣬��Ŀ����Ԫʽ����
		if (isJump(AllQuarColl[i]))
		{
			int jumpId = atof(AllQuarColl[i].target.second.c_str());	//Ŀ����
			BeginQuarColl.insert(jumpId);
		}
		*/

		//�����һ������ת��䣬�򽫴�������
		if (isJump(AllQuarColl[i - 1]))
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
}


//�����ã�ʵ��Ӧ��ʹ�÷��ű�ĺ���
bool isTemporary(string nowString)
{
	if (nowString[0] == 't')
		return true;
	return false;
}

//�жϲ�����pair�Ƿ�Ϊȫ��,���շ���0����շ���1���ҿշ���2��ȫ�շ���3
int BaseBlock::isEmpty(pair<string, string> nowPair)
{
	int emptyCount = 0;
	if (nowPair.first == "")
		emptyCount = 1;
	if (nowPair.second == "")
	{
		if (emptyCount)
			emptyCount = 3;
		else
			emptyCount = 2;
	}
	return emptyCount;
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
		int ise = isEmpty(BaseBlockQuar[i].o1);	
		if (ise == 0 || ise == 1 || ise == 2)		//�ǿգ�����
		{
			checkTemp = BaseBlockQuar[i].o1.second;
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

		ise = isEmpty(BaseBlockQuar[i].o2);			//�ڶ�����������ͬ��
		if (ise == 0 || ise == 1 || ise == 2)
		{
			checkTemp = BaseBlockQuar[i].o2.second;
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

		ise = isEmpty(BaseBlockQuar[i].target);		//Ŀ���������ͬ��
		if (ise == 0 || ise == 1 || ise == 2)
		{
			checkTemp = BaseBlockQuar[i].target.second;
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
	}
}

//�����Ծ��Ϣ���õ��Ͽ�ppt�ķ���
void BaseBlock::checkBlockActive()
{
	//���ȳ�ʼ����ǰ������Ļ�Ծ��Ϣ
	initBlockActive();
	
	//��ʼ���Ծ��Ϣ��������
	for (int i = BaseBlockActivity.size() - 1; i >= 0; i--)
	{
		string nowVariable = "";
		string changeMessage = "";
		
		int ise = isEmpty(BaseBlockQuar[i].target);
		if (ise == 0 || ise == 1 || ise == 2)
		{
			nowVariable = BaseBlockQuar[i].target.second;		//��ǰ������
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

		ise = isEmpty(BaseBlockQuar[i].o2);
		if (ise == 0 || ise == 1 || ise == 2)
		{
			nowVariable = BaseBlockQuar[i].o2.second;
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

		ise = isEmpty(BaseBlockQuar[i].o1);
		if (ise == 0 || ise == 1 || ise == 2)
		{
			nowVariable = BaseBlockQuar[i].o1.second;
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
}