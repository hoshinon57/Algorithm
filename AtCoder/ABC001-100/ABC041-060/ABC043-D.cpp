#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC043 https://atcoder.jp/contests/abc043

// 例えば5文字で条件を満たすものを考えてみると、
//   a*a*a
//   aa**a
// といったケースになる。
// でもこれって結局3文字ないし2文字の(連続)部分文字列で検出できるのでは？となる。
//
// 「連続3文字で先頭末尾が同じ文字」「連続2文字が同じ文字」の2パターンを考えれば十分。
// 
// [ACまでの思考の流れ]
// ・どういった入力が条件を満たすかを考える。
//   |S|=5で考えてみると、連続2文字ないし連続3文字を考えれば十分ではないか？となった。

int main(void)
{
	int i;
	string s; cin >> s;
	int sz = (int)s.size();

	auto out = [](int l, int r) -> void
	{
		cout << l << " " << r << endl;
	};

	// 2文字連続パターン
	for(i = 0; i < sz-1; i++)
	{
		if(s[i] == s[i+1])
		{
			out(i+1, i+2);
			return 0;
		}
	}

	// 3文字パターン
	for(i = 0; i < sz-2; i++)
	{
		if(s[i] == s[i+2])
		{
			out(i+1, i+3);
			return 0;
		}
	}

	out(-1, -1);

	return 0;
}
