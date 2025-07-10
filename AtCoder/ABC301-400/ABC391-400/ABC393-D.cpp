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

// ABC393 https://atcoder.jp/contests/abc393

/*
 * コンテスト時に考えた内容、を解説を見ながら少し変更したもの。
 * 中央値に着目して解く。
 *   https://atcoder.jp/contests/abc393/editorial/12258
 *   https://x.com/kyopro_friends/status/1890760152917115232
 * 
 * 上記sounansya氏の解説の通りで、Sにおけるi個目の'1'の位置をp[i]とし、また1の個数をMとする。
 * 1を[k,k+M)の区間に集めるとすると、i個目の'1'は k+i に持ってくることになる。
 * この移動量を最小化したいので、
 *   Σ|p[i]-(k+i)| = Σ|k-(p[i]-i)|
 * の最小値となり、このときkは (p[i]-i) の中央値となる。 [典型]
 * (p[i]-i)はiに対して増加していくので、 k=p[M/2]-(M/2) で良い。
**/

int main(void)
{
	// 0-indexed
	ll i;
	ll N; cin >> N;
	string s; cin >> s;
	vector<ll> p;  // p[i]:'1'がi個目に登場する位置 i=0～
	for(i = 0; i < N; i++)
	{
		if(s[i] == '1') p.push_back(i);
	}
	ll m = (ll)p.size();
	ll k = p[m/2]-(m/2);

	ll ans = 0;
	for(i = 0; i < m; i++)
	{
		ans += abs(k - (p[i]-i));
	}
	cout << ans << endl;

	return 0;
}
