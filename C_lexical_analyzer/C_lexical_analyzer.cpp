// lexical_analyzer2.cpp.cpp : �������̨Ӧ�ó������ڵ㡣
//��������⣺����ת���ַ��ķ������ڴ��󣬶Դʷ�����������������ʱ����ȷ�ĳ���Ҳ�ᱨ��

#include "stdafx.h"
//#include <iostream>
#include <cstdio>
#include <string>
#include <ctype.h>
#include<fstream>
using namespace std;

const int RESERVEWORDNUM = 73; //73��
const int OPERATORNUM = 37;	//30��
const int SPERATEOPERATOR = 15; // 15��
const int DELIMITERSNUM = 11; //11��
//const int IDENTIFIERNUM = 1000;
//string IDentifierTbl[IDENTIFIERNUM];

int lineNumber = 1;
string errorMessage = "";

ofstream cout("Result.out");
ifstream cin("Resource.in");

//�ؼ���
static string ReserveWord[RESERVEWORDNUM] = {
	"asm" , "do" , "if" , "return" , "typedef" , "auto" , "double" , "inline" , "short" , "typeid" , "bool" ,   "dynamic_cast" , "int" , "signed" , "typename" , "break" , "else" , "long" , "sizeof" , "union" , "case" ,	"enum" , "mutable" , "static" , "unsigned" , "catch" , "explicit" , "namespace" , "static_cast" ,	"using" , "char" , "export" , "new" , "struct" , "virtual" , "class" , "extern" , "operator" , "switch" , "void" , "const" , "false" , "private" , "template" , "volatile" , "const_cast" , "float" , "protected" ,	"this" , "wchar_t" , "continue" , "for" , "public" , "throw" , "while" , "default" , "friend" , "register" ,	"true" , "delete" , "goto" , "reinterpret_cast" , "try" , "alignas" , "alignof" , "char16_t" , "char32_t" ,	"constexpr" , "decltype" , "noexcept" , "nullptr" , "static_assert" , "thread_local" };
//�����
static char SperateOperator[SPERATEOPERATOR] = { '+' , '-' , '*' , '/' , '>' , '=' , '<' , '!' , '^' , '&' , '|' , '%' , '~' , '\?' , ':' };
static string Operator[OPERATORNUM] = { "+" , "-" , "*" , "/" , "++" , "--" , "<" , "<=" , ">" , ">=" , "=" , "==" , "!=" , "^" , "&" , "&&" ,
"|" , "||" , "!" , "%" , "~" , "<<" , ">>" , "." , "\?" , ":" , "+=" , "-=" , "/=" , "*=" , "%=" , ">>=", "<<=" , "&=", "^=" , "|=" ,"->"};
//�׷�
static char Delimiters[DELIMITERSNUM] = { ';' , '(' , ')' , ',' , '#' , '[' , ']' , '{' , '}' , ':' , '.' };

//�ж��Ƿ��ǹؼ���
static bool isReserveWord(string str) {
	for (int i = 0; i < RESERVEWORDNUM; i++) {
		if (str == ReserveWord[i])
			return true;
	}
	return false;
}

//�ж��Ƿ��ǲ�����
static bool isOperator(string str) {
	for (int i = 0; i < OPERATORNUM; i++) {
		if (str == Operator[i])
			return true;
	}
	return false;
}

//�ж��Ƿ��ǽ׷�
static bool isDelimiters(char ch) {
	for (int i = 0; i < DELIMITERSNUM; i++) {
		if (ch == Delimiters[i])
			return true;
	}
	return false;
}

//�ж��Ƿ��ǵ���������
static bool isSperateOperator(char ch) {
	for (int i = 0; i < SPERATEOPERATOR; i++) {
		if (ch == SperateOperator[i])
			return true;
	}
	return false;
}

void lexical_analyze(string resource, int location) {
	int len = resource.length();
	while (location < len) {
		//�жϿո�
		if (resource[location] == ' ') {
			location++;
		}
		//�жϲ��ɴ�ӡ�ַ�
		else if (!isprint(resource[location])) {
			//	cout << resource[location] << endl;
			if (resource[location] == '\n') {
				lineNumber++;
			}
			location++;
		}

		//�ж�ע��
		else if (resource[location] == '/' && (resource[location + 1] == '*' || resource[location + 1] == '/')) {
			string temp_annotation = "";
			if (resource[location + 1] == '/') {
				while (resource[location] != '\n') {
					location++;
				}
				//һ��ע�͹��˽���
				lineNumber++;
				location++;
			} else if (resource[location + 1] == '*') {
				//	cout << "������ע������" << endl;
				int tmp_loc = location + 2;
				int temp_line = lineNumber;
				while (tmp_loc < len - 1 && !(resource[tmp_loc] == '*' && resource[tmp_loc + 1] == '/')) {
					//	cout << "��ע�͵Ľ���λ��" << endl;
					if (resource[tmp_loc] == '\n') {
						temp_line++;
					}
					tmp_loc++;
				}
				if (tmp_loc >= len - 1) {
					//	cout << "��" << lineNumber << "��ע���д���" << endl;
					string tmp = "�� " + to_string(lineNumber) + " ��ע���д���\n";

					errorMessage = errorMessage + tmp;
					location += 2; // �ų������ע�ͷ���
				} else {
					location = tmp_loc + 2;
					lineNumber = temp_line;
				}
			}
		}

		//�жϱ�ʶ��
		else if (isalpha(resource[location]) || resource[location] == '_') {
			string temp_identifier = "";
			bool isWrong = false;
			temp_identifier = temp_identifier + resource[location];
			location++;
			while (!(resource[location] == ' ' || isSperateOperator(resource[location]) || isDelimiters(resource[location]) || resource[location] == '\n')) {
				temp_identifier = temp_identifier + resource[location];
				if (!(isalnum(resource[location]) || resource[location] == '_')) {
					isWrong = true;
				}
				location++;
			}
			if (isReserveWord(temp_identifier)) {
				cout << "< " << temp_identifier << " , �ؼ��� >" << endl;
			} else if(!isWrong){
				cout << "< ��ʶ�� , " << temp_identifier << " >" << endl;
			} else {
				string tmp = "�� " + to_string(lineNumber) + " �б�ʶ��" + temp_identifier + "�д���\n";
				errorMessage = errorMessage + tmp;
			}
		}

		//�жϳ���
		else if (isdigit(resource[location])) {
			string temp_number = "";
			enum States { Start, One, Two, Three, Four, Five, Six, Seven, Err };
			int i = 0, flag = 0;
			char ch; 
			enum States state = Start;
			while (ch = resource[location]) {
				if (flag) {
					break;
				}
				switch (state) {
				case Start:
					if (isdigit(ch))
						state = One;
					else if (ch == '.')
						state = Six;
					else {
						state = Err;
						break;
					}
					if (state != Err || state != Seven) {
						temp_number = temp_number + ch;
					}
					location++;
					break;
				case One:
					if (isdigit(ch))
						state = One;
					else if (ch == '.')
						state = Two;
					else if (isDelimiters(ch) || ch == ' ') {
						state = Seven;
						break;
					} else if (ch == 'E' || ch == 'D' || ch == 'e' || ch == 'd')
						state = Three;
					else if (ch == '\n') {
						state = Seven;
						break;
					}else{
						state = Err;
						break;
					}
					if (state != Err && state != Seven) {
						temp_number = temp_number + ch;
					}
					location++;
					break;
				case Two:
					if (isdigit(ch))
						state = Two;
					else if (ch == 'E' || ch == 'D' || ch == 'e' || ch == 'd')
						state = Three;
					else if (isDelimiters(ch) || ch == ' ') {
						state = Seven;
						break;
					} else {
						state = Err;
						break;
					}
					if (state != Err && state != Seven) {
						temp_number = temp_number + ch;
					}
					location++;
					break;
				case Three:
					if (ch == '+' || ch == '-')
						state = Four;
					else if (isdigit(ch))
						state = Five;
					else {
						state = Err;
						break;
					}
					if (state != Err && state != Seven) {
						temp_number = temp_number + ch;
					}
					location++;
					break;
				case Four:
					if (isdigit(ch))
						state = Five;
					else {
						state = Err;
						break;
					}
					if (state != Err && state != Seven) {
						temp_number = temp_number + ch;
					}
					location++;
					break;
				case Five:
					if (isdigit(ch))
						state = Five;
					else if (isDelimiters(ch) || ch == ' ') {
						state = Seven;
						break;
					} else {
						state = Err;
						break;
					}
					if (state != Err && state != Seven) {
						temp_number = temp_number + ch;
					}
					location++;
					break;
				case Six:
					if (isdigit(ch)) {
						state = Two;
					} else {
						state = Err;
						break;
					}
					if (state != Err && state != Seven) {
						temp_number = temp_number + ch;
					}
					location++;
					break;
				case Seven:
					if (ch == '\n'){
						lineNumber++;
						location++;
					} 
					flag = 1;
					break;
				case Err:
					if (flag == 0) {
						while (!(ch == ' ' || isSperateOperator(ch) || isDelimiters(ch) || ch == '\n')) {
							temp_number += resource[location];
							location++;
							ch = resource[location];
						}
						string tmp = "�� " + to_string(lineNumber) + " �г���" + temp_number + "�д���\n";
						errorMessage = errorMessage + tmp;
						flag = 1;
						//cout << "< ���� , " << temp_number << " >" << endl;
					}
					break;
				}
			}
			if (state == One || state == Two || state == Five || state == Seven)
				cout << "< ���� , " << temp_number << " >" << endl;
		}

		//�ж��ַ���
		else if (resource[location] == '"') {
			string temp_string = "";
			cout << "< " << resource[location] << " , - >" << endl;
			//location++;
			int tmp_loc = location + 1;
			while (resource[tmp_loc] != '"' && resource[tmp_loc] != '\n') {
				temp_string = temp_string + resource[tmp_loc];
				tmp_loc++;
			}
			//�ҵ��ַ����Ľ�����־�����������β��û���ҵ��ͱ���
			if (resource[tmp_loc] == '"') {
				if (tmp_loc - location > 1) {
					cout << "< �ַ��� , " << temp_string << " >" << endl;
				}
				cout << "< " << resource[tmp_loc] << " , - >" << endl;
				location = tmp_loc;
				location++;
			} else if (resource[tmp_loc] == '\n') {
				//	cout << "���ж��ַ���DFA�У���" << lineNumber << "���д��󣬶���˫���ţ�" << endl;
				string tmp = "�� " + to_string(lineNumber) + " ���д��󣬶���˫���ţ�\n";
				errorMessage = errorMessage + tmp;
				lineNumber++;
				location++;
			}
		}

		//�ж��ַ�
		else if (resource[location] == '\'') {
			cout << "< " << resource[location] << " , - >" << endl;
			location++;
			//�ж��ַ�
			if (resource[location] == '\'') {
				cout << "< " << resource[location + 1] << " , - >" << endl;
			} else if (resource[location] == '\\' && resource[location + 2] == '\'') {
				cout << "< �ַ����� , " << resource[location] << resource[location + 1] << " >" << endl;
				cout << "< " << resource[location + 2] << " , - >" << endl;
				location += 3;
			} else if (resource[location + 1] == '\'') {
				cout << "< �ַ����� , " << resource[location] << " , - >" << endl;
				cout << "< " << resource[location + 1] << " , - >" << endl;
				location += 2;
			} else {
				//	cout << "���ж��ַ�DFA�У���" << lineNumber << "���д��󣬶��൥���ţ�" << endl;
				string tmp = "�� " + to_string(lineNumber) + " ���д��󣬶��൥���ţ�\n";
				errorMessage = errorMessage + tmp;
				if (resource[location] == '\n') {
					lineNumber++;
				}
			}
		}

		//�жϽ׷�
		else if (isDelimiters(resource[location])) {
			cout << "< " << resource[location] << " , - >" << endl;
			location++;
		}

		//�ж����
		else if (isSperateOperator(resource[location])) {
			if (resource[location] == '<' && resource[location + 1] == '<' && resource[location + 2] == '=') {
				cout << "< " << resource[location] << resource[location + 1] << resource[location + 2] << " , - >" << endl;
				location += 3;
			} else if (resource[location] == '>' && resource[location + 1] == '>' && resource[location + 2] == '=') {
				cout << "< " << resource[location] << resource[location + 1] << resource[location + 2] << " , - >" << endl;
				location += 3;
			} else if (isSperateOperator(resource[location + 1])) {
				string tmp_Operator = "";
				tmp_Operator = tmp_Operator + resource[location];
				tmp_Operator = tmp_Operator + resource[location + 1];
				if (isOperator(tmp_Operator)) {
					cout << "< " << tmp_Operator << " , - >" << endl;
				} else {
					string tmp = "�� " + to_string(lineNumber) + " ���д���\n";
					errorMessage = errorMessage + tmp;
				}
				location += 2;
			} else {
				cout << "< " << resource[location] << " , - >" << endl;
				location += 1;
			}
		}
	
		else {
			string tmp = "�� " + to_string(lineNumber) + " �в���ʶ����ַ�" + resource[location] + "! \n";
			errorMessage += tmp;
			location++;
		}
	}
	if (errorMessage != "") {
		cout << endl;
		cout << "�������ط���" << endl;
		cout << errorMessage << endl;
	}
}

int main() {
	string str;
	string tmp_str = "";
	while (getline(cin, str) && str != "EOF") {
		tmp_str = tmp_str + str + '\n';
	}
	lexical_analyze(tmp_str, 0);
	cout << "Դ������������" << lineNumber << endl;
	return 0;
}
