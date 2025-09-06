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

// ABC421 https://atcoder.jp/contests/abc421

/*
 * コンテスト時はBITを用いて転倒数を求める方針だった。
 *   https://atcoder.jp/contests/abc421/submissions/68915091
 * Aを登場順に0,2,4,...
 * Bを同じく1,3,5,...
 * と置き換えたときの転倒数が答となる。
 * 
 * 以下は公式解説を元にした実装。
 * ABABAB... を作ることを考えると、
 *   Aは登場順に0,2,4,... を最終的な位置に動かすことになる
 *   Aが決まればBは自動的に決まっている
 *   'AA'をswapするのは意味が無いため、'AB'または'BA'という「意味のあるところ」からswapしていけば、
 *   操作回数は最小になるっぽい
 * となる。
 * BABABA... を作るケースも計算して、回数が少ない方が答。
**/

int main(void)
{
	ll i;
	ll N; cin >> N;
	string s; cin >> s;
	vector<ll> p;  // p[i]: Aのi個目の登場位置
	for(i = 0; i < N*2; i++)
	{
		if(s[i] == 'A') p.push_back(i);
	}

	auto chk = [&](void) -> ll
	{
		ll ret = 0;
		ll t = 0;  // 目標位置
		for(auto &e : p)
		{
			ret += abs(e-t);
			t += 2;
		}
		return ret;
	};
	ll ans = chk();

	// A,B交換
	for(auto &e : s)
	{
		if(e == 'A') e = 'B';
		else e = 'A';
	}
	p.clear();
	for(i = 0; i < N*2; i++)
	{
		if(s[i] == 'A') p.push_back(i);
	}
	chmin(ans, chk());

	cout << ans << endl;

	return 0;
}
