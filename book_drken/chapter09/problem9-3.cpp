#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題9.3

/*
 * スタックを用いてカッコの対応を判定する。
 * 主な処理はsolve()の処理およびコメントを参照。
 */

const int MAX = 100000;  // スタックサイズ
int st[MAX];
int top;  // スタックの先頭を指す添え字

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

// str:()からなる入力文字列
// answer:何文字目の'('と')'が対応しているかのペア 0-indexed
// 戻り値:カッコ列が整合していればtrue
bool solve(string &str, vector<pair<int,int>> &answer)
{
	int i;
	bool ok = true;

	for(i = 0; i < str.size(); i++)
	{
		// '('が来たらスタックに積み、')'が来たらスタックから取り出す。
		// スタックから取り出せない(スタックが空)であれば不整合。')'が多い。
		// また、文字列の最後まで処理した後に、スタックが空でない場合も不整合。'('が多い。
		if(str[i] == '(')
		{
			push(i);
		}
		else  // ')'
		{
			if(isEmpty())
			{
				ok = false;
				break;
			}
			int left = pop();
			answer.push_back(make_pair(left, i));
		}
	}
	if(!isEmpty())  // 最後はスタックが空でないとNG '('が多い
	{
		ok = false;
	}

	return ok;
}

int main(void)
{
//	string str = "(()(())())(()())";
	string str;
	cin >> str;

	init();

	vector<pair<int,int>> answer;  // {'('の位置, ')'の位置}
	bool ok = solve(str, answer);
	if(ok)
	{
		cout << "string OK" << endl;
		for(auto &e : answer)  // カッコの対応を1-indexedで出力
		{
			cout << e.first+1 << "," << e.second+1 << endl;
		}
	}
	else
	{
		cout << "string NG" << endl;
	}

	return 0;
}
