#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC181 https://atcoder.jp/contests/abc181

/*
 * 公式解説と同じ考え方だった。
 *   https://atcoder.jp/contests/abc181/editorial/259
 * 
 * まず、Sが1および2桁のときだけは個別に処理しておく。
 * ある値Nが8の倍数かは、下3桁が8の倍数になっていればよい。
 * よって100～999までの8の倍数をiとしたときに、各iについて
 *   (1)Sにおける1～9の登場回数
 *   (2)iにおける1～9の登場回数
 * を求め、(1)>=(2)になっていればよい。
 */

bool solve(void)
{
	string s; cin >> s;
	if(s.size() == 1)
	{
		return (stoi(s)%8 == 0);
	}
	else if(s.size() == 2)
	{
		if(stoi(s)%8 == 0) return true;
		reverse(s.begin(), s.end());
		if(stoi(s)%8 == 0) return true;
		return false;
	}
	// 以下、Sが3桁以上
	vector<int> s_cnt(10);
	for(auto &e : s) s_cnt[e-'0']++;
	for(int i = 104; i < 1000; i+=8)
	{
		vector<int> i_cnt(10);
		for(auto &e : to_string(i)) i_cnt[e-'0']++;

		bool ok = true;
		for(int j = 0; j < 10; j++)
		{
			if(s_cnt[j] >= i_cnt[j]) {;} // ok
			else ok = false;
		}
		if(ok) return true;
	}
	return false;
}

int main(void)
{
	YesNo(solve());

	return 0;
}
