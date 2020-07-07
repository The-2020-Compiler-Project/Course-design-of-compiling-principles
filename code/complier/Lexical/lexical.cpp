/*
    lexical.cpp
    �ʷ���������ʵ�֡�
*/
#include <ctype.h>
#include <map>
#include<math.h>
#include<string>
#include<iostream>
#include "lexical.h"
using namespace std;
static char ch;         // ��һ�������������ģ���û�����������ַ�
                        //  �����������Ŀ�������Ч��

static map<string, symbolType> reservedWords;   // �����ֱ�����ÿ����������������֮ǰ�Ĺ�ϵ

// �����Ĺ��캯��
lexical::lexical(const string& file) : sourceFile(file.c_str()) {
    // ���ļ���������Ϊ����
// �������ļ��еĵ�һ���ַ�����
    ch = sourceFile.get();
    //��ʼ�������ֱ�                          
#define INSERT_RES(str, type)    \
        reservedWords.insert(pair<string, symbolType>(str, type))   // ����꣬��������
    INSERT_RES("procedure", PROCEDURE);
    INSERT_RES("function", FUNCTION);
    INSERT_RES("const", CONST);
    INSERT_RES("var", VAR);
    INSERT_RES("array", ARRAY);
    INSERT_RES("of", OF);
    INSERT_RES("integer", INTEGER);
    INSERT_RES("char", CHAR);
    INSERT_RES("begin", BEGIN);
    INSERT_RES("end", END);
    INSERT_RES("for", FOR);
    INSERT_RES("do", DO);
    INSERT_RES("while", WHILE);
    INSERT_RES("if", IF);
    INSERT_RES("then", THEN);
    INSERT_RES("else", ELSE);
    INSERT_RES("read", READ);
    INSERT_RES("write", WRITE);
    INSERT_RES("result", RESULT);
    INSERT_RES("true", TRUE);
    INSERT_RES("false", FALSE);
    INSERT_RES("call", CALL);
    INSERT_RES("program", PROGRAM);
    INSERT_RES("type", TYPE);
    INSERT_RES("record", RECORD);

#undef  INSERT_RES                                                 // ȡ������ INSERT_RES ��

    if (sourceFile.is_open())
        cout << "open!" << endl;

}

#define GET     ch = sourceFile.get()   // ����꣬�򻯴��ļ��ж�ȡһ���ַ��Ĳ���
#define PEEK    ch = sourceFile.peek()  // ����꣬�򻯲鿴�ļ�����һ���ַ��Ĳ���
int m = 1;//�кų�ʼ��Ϊ1
int flag = -1;//�����ֲ�ѯ����λ�ã�flag>=0Ϊλ�ã�flag=-1Ϊδ��ѯ��
int a = -1;//aΪ���صĲ����λ��
string Pt_str = "\0";//��ʱ�洢�����Ϊ�˷����ҵ�λ��
//extern STable sTable;
token token1;
// ��ȡ��һ�����ţ����������ʹ��� symbol,λ�ô���loc,��������row
token lexical::next() {
    while (ch != EOF) {
        while (isspace(ch) && (ch != '\n'))//�����ո��ֱ������������һ���ַ�
            GET;
        while (ch == '\n') {//�������з��ŵ�ʱ��Ҫ��������һ���Ҷ���һ���ַ�/
            m++;
            GET;
        }
        if (isdigit(ch)) {          // �����֣����������һ������������ʵ��
            char c[100];//�����洢ɨ�������
            int n = 0;
            while (ch >= '0' && ch <= '9')//�����ɨ�������������ֵ�ʱ��ֹͣ
            {
                c[n++] = ch;//�ӵ�һ��ɨ�赽�����־ͷ�������c��
                GET;//Ȼ�����һλ�����ж��Ƿ�Ϊ�����֣���������֣������������飬������ǣ�ֱ������ѭ��
            }// ���ѭ������ʱ��ch ��һ���µ�δ������ַ����������֣�������Ҫ�ж����ch�ǲ���С����ĵ�
            if (ch == '.') {//����������ѭ��������ʵ��������Ϊ����
                c[n++] = ch;//ʶ��С���������������c
                GET;//Ȼ�����һ���ַ�����ʼ����С���ĺ�벿�ֵĽ���
                while (ch >= '0' && ch <= '9')//����������ַ������ֵĻ���������������c
                {
                    c[n++] = ch;
                    GET;//����һ���ַ������жϣ����������ַ���������ʱ������ѭ����С���������Ѿ���ȫ��������c��
                }//��������ѭ��ʱ��ch��һ��δ������ַ����������֣�
                c[n] = '\0';//��β
                double num = atof(c); //���ַ���ת����double��
                flag = sTable.findRt(num);//ȥ���Ҹ�ʵ����λ��
                if (flag == -1) {//Ϊ-1��û�в鵽��Ҫ�����ȥ�����õ����ص�λ��ֵ
                    a = sTable.inserRt(num);//aΪ�����ͳ����������λ��
                    token1.loc = a;//a�ǵõ��Ĳ���λ��
                    token1.symbol = REALNUMBER;//����Ϊʵ������
                    token1.row = m;//����ֻ�����������з���ʱ��m�Ż��һ
                    return token1;//����tokenֵ������ͬ��
                }
                else {
                    token1.symbol = REALNUMBER;//flagֵ����-1�Ļ������صľ���������ʵ�λ�ã�ֱ����д�Ϳ���
                    token1.loc = flag;//λ�þ��Ƿ��ص�flagֵ
                    token1.row = m;
                    return token1;
                }
            }
            //���û�н��������ifѭ������ô�����������ͣ����ʱ���chʱһ��δ������ַ�����������Ҳ����С���㣩�������߼�
            c[n] = '\0';
            int num = atof(c); //���ַ���ת����int��
            flag = sTable.findNt(num);//ͬ��
            if (flag == -1) {//δ��ѯ������Ҫ����
                a = sTable.insertNt(num);
                token1.symbol = NUMBER;
                token1.loc = a;
                token1.row = m;
                return token1;
            }
            else {//��ѯ��
                token1.symbol = NUMBER;
                token1.loc = flag;
                token1.row = m;
                return token1;
            }
        }
        // ���ѭ������ʱ��ch ��һ���µ�δ������ַ����߼���ȷ
        else if (isalpha(ch)) {   // ����ĸ�����������һ���Ǳ�ʶ��������
            char word[10];//��������������ı�ʶ�������߱�����
            int x = 0;
            while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch == '_')) {
                word[x++] = ch;//�����������п�����Ϊ��ʶ���ؼ��ֵ��ַ���������word
                GET;
            }//����ѭ��ʱ��ch��һ��δ��������ַ�
            word[x] = '\0';
            // ���£��ж϶����ķ����Ǳ�ʶ�����Ǳ����֣�����������ʹ��� symbol
            map<string, symbolType>::iterator it = reservedWords.find(word);
            if (it == reservedWords.end()) {      // ���ڱ����ֱ��У�˵���Ǳ�ʶ��
                token1.symbol = IDENTIFIER;
                flag = sTable.findIt(word);//ȥ��ʶ������Ѱ��
                if (flag == -1) {//û���ҵ����������
                    a = sTable.insertIt(word);
                    token1.loc = a;
                    token1.row = m;
                    return token1;
                }
                else {
                    token1.loc = flag;
                    token1.row = m;
                    return token1;
                }
            }
            else {                               // �ڱ����ֱ���
                string Kt = it->first;//ȡ����ؼ��ֵ�����
                flag = sTable.findPt(Kt);
                token1.symbol = it->second;// Ҫ�ѱ����ֵľ������ʹ��� symbol
                token1.loc = flag;//�ؼ��ֵ�loc�������ڷ��ű��λ��
                token1.row = m;
                return token1;
            }
        }
        else if (ch == '\"') {//�����ַ���
            token1.symbol = STRING;
            char y[100];
            int l = 0;
            y[l++] = ch;
            GET;
            while (ch != '\"') {
                y[l++] = ch;
                GET;
            }//ѭ������ʱ��ch��˫����
            y[l++] = ch;
            y[l] = '\0';
            GET;//��һ����һ����������ַ�
            //�жϸ��ַ����Ƿ�������ַ�������
            flag = sTable.findSt(y);
            if (flag == -1) {//���ڣ�Ҫ����
                a = sTable.insertSt(y);
                token1.loc = a;
                token1.row = m;
                return token1;
            }
            else {
                token1.loc = flag;
                token1.row = m;
                return token1;
            }
        }
        else if (ch == '\'') {//�����ַ�
            token1.symbol = CHAR;
            char b[100];
            int k = 0;
            b[k++] = ch;
            GET;
            while (ch != '\'') {
                b[k++] = ch;
                GET;
            }//ѭ������ʱ��ch�ǵ�����
            b[k++] = ch;
            b[k] = '\0';
            GET;//��һ����һ����������ַ�
            //�жϸ��ַ��Ƿ�������ַ�����
            flag = sTable.findCt(b);
            if (flag == -1) {
                a = sTable.insertCt(b);
                token1.loc = a;
                token1.row = m;
                return token1;
            }
            else {
                token1.loc = flag;
                token1.row = m;
                return token1;
            }
        }
        else if (ch == '{') {//������ע��һ{����������Ķ���ע�͵�}
            GET;
            while (ch != '}') {
                GET;
            }//����ѭ��ʱ��ch�ǡ�}��
            GET;//��һ��Ҫ���������һ���ַ�
            next();//���ú������������ַ��Ĵ���
        }
        else if (ch == '/') {//������ע�Ͷ�//
            PEEK;//�鿴��һ���ַ��ǲ���/
            if (ch == '/') {//������б��ͬʱ���־��ǵ���ע�ͣ�����Ϊ������Ժ�
                GET;//���ڶ�����б��
                while (ch != '\n') {//ֻ�г��ֻ��з��Ų�˵������ע�ͽ���
                    GET;//��������ֻ��з��ž�һֱ����һ�����ţ�
                }//����ѭ��ʱ��chΪ���з���
                m++;//����mҪ��һ
                GET;//����һ���ַ��Ա�����
                next();//�������������һ���ַ��ķ���
            }
            else {//��һ���ַ����Ƿ�б�ܣ���ô���ǽ�����Ժ�
                Pt_str = "/";
                flag = sTable.findPt(Pt_str);
                token1.symbol = SLASH;//����Ϊ��б�ܣ����Ժţ��ǽ��
                token1.loc = flag;//�ý���ڷ��ű��λ��
                token1.row = m;
                GET;//����ǰ�ַ���Ϊû�д�����ַ�
                return token1;
            }
        }
        else if (ch == '(') {//��������ˡ�������ô�п����Ƕ���ע�ͣ����߽�������ţ�����Ҫȥ�������
            PEEK;//�鿴��һ��ch��ֵ���ж�
            if (ch == '*') {//����ǡ���*������ϣ���ô���Ƕ���ע��
                GET;//��ǰ�ַ�Ϊ*
                for (int j = 0;; j++) {
                    GET;//��ǰ�ַ�Ϊ*����һλ
                    while (ch != '*') {//ֻ�г�����*,���п�����ע�ͽ����ĵط�
                        GET;//��������־�һֱ����һ������
                    }//����ѭ��ʱ��ch��*����Ҫ�����ж���һ�������ǲ��ǡ���������ȷ���Ƿ��Ƕ���ע�ͽ����ĵط�
                    PEEK;//�ж�����Ƿ�Ϊ������
                    if (ch == ')') //���������*������ϣ�˵������ע�ͽ���
                        break;//���������*������ϲſ�������ѭ�������û�г��ּ���������һ���ж�
                }
                GET;//��ǰ����Ϊ��
                GET;//����һ���ַ��Ա�����
                next();//����������з���
            }
            else {//ֻ�������ţ���ô���ǽ��������
                Pt_str = "(";
                flag = sTable.findPt(Pt_str);
                token1.symbol = LEFT_BRACKET;//����Ϊ�����ţ��ǽ��
                token1.loc = flag;//�ý���ڷ��ű��λ��
                token1.row = m;
                GET;//����һ���ַ�������һ�ַ���
                return token1;
            }
        }
        else if (!isspace(ch)) {          //������ǿհ��ַ�
            switch (ch) {
            case '<':
                PEEK;
                if (ch == '=') {
                    GET;//��ǰ����Ϊ=
                    Pt_str = "<=";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = LESS_EQUAL;
                    token1.loc = flag;
                    token1.row = m;
                    GET;//��һ��Ϊ��������ַ�������ͬ��
                    return token1;
                }
                else if (ch == '>') {
                    GET;//��ǰ����Ϊ>
                    Pt_str = "<>";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = INEQUAL;
                    token1.loc = flag;
                    token1.row = m;
                    GET;//��һ��Ϊ��������ַ�������ͬ��
                    return token1;
                }
                else if(ch==' ') {
                    Pt_str = "<";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = LESS;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    GET;
                    return token1;
                }
                else{
                    cout << "�˵���Ϊ���󵥴ʣ�" << endl;
                    GET;
                    exit(1);
                }
            case '>':
                PEEK;
                if (ch == '=') {
                    GET;
                    Pt_str = ">=";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = GREATER_EQUAL;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    return token1;
                }
                else if(ch==' ') {
                    Pt_str = ">";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = GREATER;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    GET;
                    return token1;
                }
                else {
                    cout << "�˵���Ϊ���󵥴ʣ�" << endl;
                    GET;
                    exit(1);
                }
            case ':':
                PEEK;
                if (ch == '=') {
                    GET;
                    Pt_str = ":=";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = ASSIGN;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    return token1;
                }
                else if(ch==' '){
                    Pt_str = ":";
                    flag = sTable.findPt(Pt_str);
                    token1.symbol = COLON;
                    token1.loc = flag;
                    token1.row = m;
                    GET;
                    GET;
                    return token1;
                }
                else {
                    cout << "�˵���Ϊ���󵥴ʣ�" << endl;
                    GET;
                    exit(1);
                }
            case ')':
                GET;
                Pt_str = ")";
                flag = sTable.findPt(Pt_str);
                token1.symbol = RIGHT_BRACKET;
                token1.loc = flag;
                token1.row = m;
                GET;
                return token1;
            case '[':
                Pt_str = "[";
                flag = sTable.findPt(Pt_str);
                token1.symbol = LEFT_SQUARE;
                token1.loc = flag;
                token1.row = m;
                GET;
                return token1;
            case ']':
                Pt_str = "]";
                flag = sTable.findPt(Pt_str);
                token1.symbol = RIGHT_SQUARE;
                token1.loc = flag;
                token1.row = m;
                GET;
                return token1;
            case '\'':
                Pt_str = "\'";
                flag = sTable.findPt(Pt_str);
                token1.symbol = SINGLE_QUOTE;
                token1.loc = flag;
                token1.row = m;
                GET;
                return token1;
            case '\"':
                Pt_str = "\"";
                flag = sTable.findPt(Pt_str);
                token1.symbol = DOUBLE_QUOTE;
                token1.loc = flag;
                token1.row = m;
                GET;
                return token1;
            case ',':
                Pt_str = ",";
                flag = sTable.findPt(Pt_str);
                token1.symbol = COMMA;
                token1.loc = flag;
                token1.row = m;
                GET;
                return token1;
            case ';':
                Pt_str = ";";
                flag = sTable.findPt(Pt_str);
                token1.symbol = SEMICOLON;
                token1.loc = flag;
                token1.row = m;
                GET;
                return token1;
            case '.':
                Pt_str = ".";
                flag = sTable.findPt(Pt_str);
                token1.symbol = PERIOD;
                token1.loc = flag;
                token1.row = m;
                GET;
                return token1;
            case '+':
                Pt_str = "+";
                flag = sTable.findPt(Pt_str);
                token1.symbol = PLUS;
                token1.loc = flag;
                token1.row = m;
                GET;
                return token1;
            case '-':
                Pt_str = "-";
                flag = sTable.findPt(Pt_str);
                token1.symbol = MINUS;
                token1.loc = flag;
                token1.row = m;
                GET;
                return token1;
            case '*':
                Pt_str = "*";
                flag = sTable.findPt(Pt_str);
                token1.symbol = ASTERISK;
                token1.loc = flag;
                token1.row = m;
                GET;
                return token1;
            case '=':
                Pt_str = "=";
                flag = sTable.findPt(Pt_str);
                token1.symbol = EQUAL;
                token1.loc = flag;
                token1.row = m;
                GET;
                return token1;
            default:  //������зǷ��ַ��ı���
                cout << "�˵���Ϊ���󵥴�" << endl;
                GET;
                exit(1);
            }
        }
    }
}