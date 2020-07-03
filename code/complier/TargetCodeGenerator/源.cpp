#include"TargetCodeGenerator.h"

int main()
{
	TargetCodeGenerator tarCode;
	tarCode.read("quar.txt");
	tarCode.spiltMidCode();
	tarCode.generateCode();
}