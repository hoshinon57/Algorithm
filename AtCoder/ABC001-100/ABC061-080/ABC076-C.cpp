#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC076 https://atcoder.jp/contests/abc076

/*
 * 各iについて、s[i]を先頭にtと一致させられるかを判定する。
 * 一致させられれば、残りの'?'を'a'に置き換えたものを答の候補に入れる。
 * 
 * 答の候補をsetに入れておくと、先頭が最終的な答となる。
**/

// s[idx]を先頭に、tと一致するか
// s[]は書き換えられる点に注意
bool check(string &s, string &t, int idx)
{
	int i;
	for(i = 0; i < (int)t.size(); i++)
	{
		int id = i+idx;
		if(s[id] != '?')
		{
			if(s[id] != t[i]) return false;
		}
		else
		{
			s[id] = t[i];
		}
	}
	return true;
}

string solve(void)
{
	string s, t; cin >> s >> t;	
	const string ng = "UNRESTORABLE";
	if(s.size() < t.size()) return ng;

	int i;
	int sl = (int)s.size();
	int tl = (int)t.size();
	set<string> li;
	for(i = 0; i <= sl-tl; i++)  // s[i]を先頭に、tと一致するか
	{
		string sbk = s;  // sはcheck()で書き換えられる
		if(!check(sbk, t, i)) continue;
		// 一致するなら残りの'?'を'a'に置き換え
		for(auto &e : sbk)
		{
			if(e == '?') e = 'a';
		}
		li.insert(sbk);
	}
	if(li.size() == 0) return ng;
	else return *li.begin();
}

int main(void)
{
	cout << solve() << endl;

	return 0;
}
