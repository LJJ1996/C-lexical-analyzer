# C-lexical-analyzer

项目的环境为：Microsoft Visual Studio 2015

程序是采用C/C++编写,主要对C/C++语言做词法分析

根据词法分析器的三个功能编写该词法分析器

1. 把高级语言源程序变成单词字符串
2. 删除多余的空格、回车、注释
3. 指出源程序中的错误

C_lexical_analyzer.cpp
    这是主应用程序源文件。

Resource.in	
	这是等待分析的源程序
	
Result.out
	这是分析的结果
	
分析结果分为三部分
- 把源程序分解成单词字符串
- 指出源程序中的错误
- 源程序的总行数
