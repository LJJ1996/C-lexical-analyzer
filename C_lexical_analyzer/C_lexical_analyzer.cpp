// lexical_analyzer2.cpp.cpp : 定义控制台应用程序的入口点。
//程序的问题：对于转移字符的分析存在错误，对词法分析器程序做分析时，正确的程序也会报错

#include "stdafx.h"
//#include <iostream>
#include <cstdio>
#include <string>
#include <ctype.h>
#include<fstream>
using namespace std;

const int RESERVEWORDNUM = 73; //73个
const int OPERATORNUM = 37;	//30个
const int SPERATEOPERATOR = 15; // 15个
const int DELIMITERSNUM = 11; //11个
//const int IDENTIFIERNUM = 1000;
//string IDentifierTbl[IDENTIFIERNUM];

int lineNumber = 1;
string errorMessage = "";

ofstream cout("Result.out");
ifstream cin("Resource.in");

//关键字
static string ReserveWord[RESERVEWORDNUM] = {
	"asm" , "do" , "if" , "return" , "typedef" , "auto" , "double" , "inline" , "short" , "typeid" , "bool" ,   "dynamic_cast" , "int" , "signed" , "typename" , "break" , "else" , "long" , "sizeof" , "union" , "case" ,	"enum" , "mutable" , "static" , "unsigned" , "catch" , "explicit" , "namespace" , "static_cast" ,	"using" , "char" , "export" , "new" , "struct" , "virtual" , "class" , "extern" , "operator" , "switch" , "void" , "const" , "false" , "private" , "template" , "volatile" , "const_cast" , "float" , "protected" ,	"this" , "wchar_t" , "continue" , "for" , "public" , "throw" , "while" , "default" , "friend" , "register" ,	"true" , "delete" , "goto" , "reinterpret_cast" , "try" , "alignas" , "alignof" , "char16_t" , "char32_t" ,	"constexpr" , "decltype" , "noexcept" , "nullptr" , "static_assert" , "thread_local" };
//运算符
static char SperateOperator[SPERATEOPERATOR] = { '+' , '-' , '*' , '/' , '>' , '=' , '<' , '!' , '^' , '&' , '|' , '%' , '~' , '\?' , ':' };
static string Operator[OPERATORNUM] = { "+" , "-" , "*" , "/" , "++" , "--" , "<" , "<=" , ">" , ">=" , "=" , "==" , "!=" , "^" , "&" , "&&" ,
"|" , "||" , "!" , "%" , "~" , "<<" , ">>" , "." , "\?" , ":" , "+=" , "-=" , "/=" , "*=" , "%=" , ">>=", "<<=" , "&=", "^=" , "|=" ,"->"};
//阶符
static char Delimiters[DELIMITERSNUM] = { ';' , '(' , ')' , ',' , '#' , '[' , ']' , '{' , '}' , ':' , '.' };

//判断是否是关键字
static bool isReserveWord(string str) {
	for (int i = 0; i < RESERVEWORDNUM; i++) {
		if (str == ReserveWord[i])
			return true;
	}
	return false;
}

//判断是否是操作符
static bool isOperator(string str) {
	for (int i = 0; i < OPERATORNUM; i++) {
		if (str == Operator[i])
			return true;
	}
	return false;
}

//判断是否是阶符
static bool isDelimiters(char ch) {
	for (int i = 0; i < DELIMITERSNUM; i++) {
		if (ch == Delimiters[i])
			return true;
	}
	return false;
}

//判断是否是单个操作符
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
		//判断空格
		if (resource[location] == ' ') {
			location++;
		}
		//判断不可打印字符
		else if (!isprint(resource[location])) {
			//	cout << resource[location] << endl;
			if (resource[location] == '\n') {
				lineNumber++;
			}
			location++;
		}

		//判断注释
		else if (resource[location] == '/' && (resource[location + 1] == '*' || resource[location + 1] == '/')) {
			string temp_annotation = "";
			if (resource[location + 1] == '/') {
				while (resource[location] != '\n') {
					location++;
				}
				//一行注释过滤结束
				lineNumber++;
				location++;
			} else if (resource[location + 1] == '*') {
				//	cout << "进入了注释里面" << endl;
				int tmp_loc = location + 2;
				int temp_line = lineNumber;
				while (tmp_loc < len - 1 && !(resource[tmp_loc] == '*' && resource[tmp_loc + 1] == '/')) {
					//	cout << "找注释的结束位置" << endl;
					if (resource[tmp_loc] == '\n') {
						temp_line++;
					}
					tmp_loc++;
				}
				if (tmp_loc >= len - 1) {
					//	cout << "第" << lineNumber << "行注释有错误" << endl;
					string tmp = "第 " + to_string(lineNumber) + " 行注释有错误\n";

					errorMessage = errorMessage + tmp;
					location += 2; // 排除错误的注释符号
				} else {
					location = tmp_loc + 2;
					lineNumber = temp_line;
				}
			}
		}

		//判断标识符
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
				cout << "< " << temp_identifier << " , 关键字 >" << endl;
			} else if(!isWrong){
				cout << "< 标识符 , " << temp_identifier << " >" << endl;
			} else {
				string tmp = "第 " + to_string(lineNumber) + " 行标识符" + temp_identifier + "有错误！\n";
				errorMessage = errorMessage + tmp;
			}
		}

		//判断常数
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
						string tmp = "第 " + to_string(lineNumber) + " 行常数" + temp_number + "有错误！\n";
						errorMessage = errorMessage + tmp;
						flag = 1;
						//cout << "< 数字 , " << temp_number << " >" << endl;
					}
					break;
				}
			}
			if (state == One || state == Two || state == Five || state == Seven)
				cout << "< 数字 , " << temp_number << " >" << endl;
		}

		//判断字符串
		else if (resource[location] == '"') {
			string temp_string = "";
			cout << "< " << resource[location] << " , - >" << endl;
			//location++;
			int tmp_loc = location + 1;
			while (resource[tmp_loc] != '"' && resource[tmp_loc] != '\n') {
				temp_string = temp_string + resource[tmp_loc];
				tmp_loc++;
			}
			//找到字符串的结束标志，如果到达行尾还没有找到就报错
			if (resource[tmp_loc] == '"') {
				if (tmp_loc - location > 1) {
					cout << "< 字符串 , " << temp_string << " >" << endl;
				}
				cout << "< " << resource[tmp_loc] << " , - >" << endl;
				location = tmp_loc;
				location++;
			} else if (resource[tmp_loc] == '\n') {
				//	cout << "（判断字符串DFA中）第" << lineNumber << "行有错误，多余双引号！" << endl;
				string tmp = "第 " + to_string(lineNumber) + " 行有错误，多余双引号！\n";
				errorMessage = errorMessage + tmp;
				lineNumber++;
				location++;
			}
		}

		//判断字符
		else if (resource[location] == '\'') {
			cout << "< " << resource[location] << " , - >" << endl;
			location++;
			//判断字符
			if (resource[location] == '\'') {
				cout << "< " << resource[location + 1] << " , - >" << endl;
			} else if (resource[location] == '\\' && resource[location + 2] == '\'') {
				cout << "< 字符常量 , " << resource[location] << resource[location + 1] << " >" << endl;
				cout << "< " << resource[location + 2] << " , - >" << endl;
				location += 3;
			} else if (resource[location + 1] == '\'') {
				cout << "< 字符常量 , " << resource[location] << " , - >" << endl;
				cout << "< " << resource[location + 1] << " , - >" << endl;
				location += 2;
			} else {
				//	cout << "（判断字符DFA中）第" << lineNumber << "行有错误，多余单引号！" << endl;
				string tmp = "第 " + to_string(lineNumber) + " 行有错误，多余单引号！\n";
				errorMessage = errorMessage + tmp;
				if (resource[location] == '\n') {
					lineNumber++;
				}
			}
		}

		//判断阶符
		else if (isDelimiters(resource[location])) {
			cout << "< " << resource[location] << " , - >" << endl;
			location++;
		}

		//判断算符
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
					string tmp = "第 " + to_string(lineNumber) + " 行有错误！\n";
					errorMessage = errorMessage + tmp;
				}
				location += 2;
			} else {
				cout << "< " << resource[location] << " , - >" << endl;
				location += 1;
			}
		}
	
		else {
			string tmp = "第 " + to_string(lineNumber) + " 行不可识别的字符" + resource[location] + "! \n";
			errorMessage += tmp;
			location++;
		}
	}
	if (errorMessage != "") {
		cout << endl;
		cout << "代码出错地方：" << endl;
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
	cout << "源程序总行数：" << lineNumber << endl;
	return 0;
}
