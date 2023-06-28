#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC307 https://atcoder.jp/contests/abc307

/*
 * スタック的に考える。
 * kyopro_friends氏の解説とほぼ同じ考え方。 https://twitter.com/kyopro_friends/status/1672602828270092289
 * 
 * 今見ている文字列が、「何個のカッコ() に囲まれているか」を保持しておく。
 * i個のカッコに囲まれている場合、vector<string> stackとしてstack[i]に文字を入れていく。
 * '('が来たらiを+1,
 * ')'が来たらstack[i]を削除し、iを-1する。ただしi=0のときの')'は普通の文字と同一に扱う。
 * 
 * 最後にstackを[0]から順に出力していったものが、答となる。
 */
int main(void)
{
	int N; cin >> N;
	string S; cin >> S;
	vector<string> stack = {""};  // stack[i]:i個のペアのカッコに囲まれた部分の文字列
	int layer = 0;
	for(auto &e : S)
	{
		if(e == '(')
		{
			layer++;
			stack.push_back("");
			stack[layer] += e;
		}
		else if(e == ')' && layer > 0)
		{
			stack.pop_back();
			layer--;
		}
		else  // 1つも'('が無い状態で')'が来た場合も、こちらに遷移
		{
			stack[layer] += e;
		}
	}

	for(auto &e : stack)
	{
		cout << e;
	}
	cout << endl;

	return 0;
}
