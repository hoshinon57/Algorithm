#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題9.2

/*
 * スタックを用いて逆ポーランド既報の数式を処理する。
 * GitHubの解説ではかなり簡易なもので、入力にて2桁の数値が判定できない。
 * それを判定できるように少し改善した。
 * 
 * 主な処理はmain()およびanalyze()を参照。
 */

const int MAX = 100000;  // スタックサイズ
int st[MAX];
int top;  // スタックの先頭を指す添え字

const int ADD = 1;
const int SUB = 2;
const int MUL = 3;
const int DIV = 4;
const int KIND_NUM = 0;
const int KIND_OPE = 1;

// スタック初期化
void init(void)
{
	top = 0;
}

// スタックが空かどうか
bool isEmpty(void)
{
	return (top == 0);
}

// スタックが満杯かどうか
bool isFull(void)
{
	return (top == MAX);
}

// スタックに要素を挿入する
void push(int num)
{
	if(isFull())
	{
		cout << "error: stack is Full." << endl;
		return;
	}
	st[top] = num;
	top++;
}

// スタックから要素を取り出す
int pop(void)
{
	if(isEmpty())
	{
		cout << "error: stack is Empty." << endl;
		return -1;
	}
	top--;
	return st[top];
}

// 式expのi文字目から数値や演算子を1つ得て、kindおよびparamに設定する。
// 戻り値：読み終えた文字の次
int analyze(string &exp, int i, int &kind, int &param)
{
	// 先頭の空白を読み捨てる
	while(exp[i] ==' ') i++;

	switch(exp[i])
	{
	case '+':
		kind = KIND_OPE;
		param = ADD;
		return i+1;
	case '-':
		kind = KIND_OPE;
		param = SUB;
		return i+1;
	case '*':
		kind = KIND_OPE;
		param = MUL;
		return i+1;
	case '/':
		kind = KIND_OPE;
		param = DIV;
		return i+1;
	default:
		kind = KIND_NUM;
		int j = i;
		while(exp[j] != ' ') j++;  // 空白が来るまで読む
		param = stoi(exp.substr(i, j-i));
		return j+1;
	}
}

int main(void)
{
	init();

//	string exp = "3 4 + 1 2 - *";  // -7
	string exp = "12 2 / 5 + 4 2 * +";  // =19
	int kind;
	int param;
	int i = 0;
	while(i < exp.size())
	{
		i = analyze(exp, i, kind, param);
		// 入力が数値ならばスタックにプッシュ。
		// 演算子ならばスタックから2つポップして演算する。
		if(kind == KIND_NUM)
		{
			push(param);
		}
		else
		{
			int a, b, c;
			b = pop();
			a = pop();
			switch(param)
			{
			case ADD:
				c = a + b;
				break;
			case SUB:
				c = a - b;
				break;
			case MUL:
				c = a * b;
				break;
			case DIV:
				c = a / b;
				break;
			}
			push(c);  // 演算結果をスタックへ
		}
	}
	cout << pop() << endl;

	return 0;
}
