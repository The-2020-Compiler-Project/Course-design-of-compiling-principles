/*
    lexical.h
*/
#ifndef LEX
#define LEX
#include<vector>
#include<string>
#include<fstream>
#include "../STable/STable.h"
using namespace std;
typedef enum {
    PROCEDURE,          // procedure �ؼ���
    FUNCTION,           // function �ؼ���
    CONST,              // const �ؼ���   
    VAR,                // var �ؼ���
    ARRAY,              // array �ؼ���
    OF,                 // of �ؼ���
    INTEGER,            // integer �ؼ���
    CHAR,               // char �ؼ���
    BEGIN,              // begin �ؼ���
    END,                // end �ؼ���
    FOR,                // for �ؼ���
    DO,                 // do �ؼ���
    WHILE,              // while �ؼ���
    IF,                 // if �ؼ���
    THEN,               // then �ؼ���
    ELSE,               // else �ؼ���
    READ,               // read �ؼ���
    WRITE,              // write �ؼ���
    RESULT,             // result�ؼ���
    TRUE,               // true�ؼ���
    FALSE,              // false�ؼ���
    CALL,               // call�ؼ���
    PROGRAM,            // program�ؼ���
    TYPE,               // type�ؼ���
    RECORD,             // record�ؼ���

    IDENTIFIER,         // ��ʶ��
    NUMBER,             // ����   
    STRING,             // �ַ���
    CHARATER,           // char ���͵��ַ�
    REALNUMBER,         // ʵ��

    LEFT_BRACKET,       // ������
    RIGHT_BRACKET,      // ������
    LEFT_SQUARE,        // ������
    RIGHT_SQUARE,       // �ҷ�����
    SINGLE_QUOTE,       // ������                      
    DOUBLE_QUOTE,       // ˫����
    COMMA,              // ����
    SEMICOLON,          // �ֺ�
    PERIOD,             // ���
    PLUS,               // �Ӻ�
    MINUS,              // ����
    ASTERISK,           // �Ǻţ��˺�
    SLASH,              // б�ܣ�����
    COLON,              // ð��
    EQUAL,              // �Ⱥ�
    ASSIGN,             // ��ֵ��
    LESS,               // С�ں�
    LESS_EQUAL,         // С�ڵ��ں�
    GREATER,            // ���ں�
    GREATER_EQUAL,      // ���ڵ��ں�
    INEQUAL             // ���Ⱥ�
}symbolType;

typedef struct
{
    symbolType symbol; // �·��ŵ�����
    int loc;//ÿ���������������������λ��
    int row;//���ʳ��ֵ�����
}token;

class lexical {
public:
    token next();
    lexical(const string& file);   // ˽�л����캯��������ģʽ
private:
    ifstream sourceFile; // Դ�����ļ�������

    /*vector<string> IT;    //��ʶ����
    vector<char> CT;      //�ַ���
    vector<string> ST;    //�ַ�����
    vector<int> iT;       //������
    vector<double> rT;    //ʵ����*/
    friend class Grammar;
};

#endif