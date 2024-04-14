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

// ABC349 https://atcoder.jp/contests/abc349

/*
 * evima氏の解説を元に実装した内容。
 *   https://atcoder.jp/contests/abc349/editorial/9788
 * 
 * セグメントツリー的な考え方。
 * 入力[L,R)と、良い数列[l,r)を考える。  ※初期は [l,r)=[0,2^60) で開始する
 * 
 * [l,r)が[L,R)にすっぽり収まっていれば、[l,r)を採用する。
 * m=(l+r)/2として、
 * [m,r)が[L,R)の範囲外であれば、右半分は捨てる。
 * [l,r)が[L,R)の範囲外であれば、左半分は捨てる。
 * それ以外ならば、[l,m)と[m,r)をそれぞれ調べる。
 * 
 * 以上を再帰で実装する。
 */

ll L, R;
vector<pair<ll,ll>> ans;  // {l,r}

void func(ll nl, ll nr)
{
	if(L<=nl && nr<=R)
	{
		ans.push_back({nl, nr});
		return;
	}
	ll m = (nl+nr)/2;
	if(R <= m)  // [m,r)は範囲外なので捨てる
	{
		func(nl, m);
	}
	else if(m <= L)  // [l,m)は範囲外なので捨てる
	{
		func(m, nr);
	}
	else
	{
		func(nl, m);
		func(m, nr);
	}
}

int main(void)
{
	cin >> L >> R;
	func(0, 1LL<<60);

	cout << ans.size() << endl;
	for(auto [l,r] : ans)
	{
		cout << l << " " << r << endl;
	}

	return 0;
}
