#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <array>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC108 https://atcoder.jp/contests/abc108

/*
 * 自力で解き切れず、解説を見た。
 *   https://drken1215.hatenablog.com/entry/2018/09/02/020500
 * 
 * 基本はけんちょん氏の解説とほとんど同じ方針。
 * 2^rから残った部分の辺の構築がわずかに違う程度。
 * 
 * まず、Lが2のべき乗、つまり2^r=Lであるケースを考えると、
 * 頂点を(r+1)個用意し、
 *   0->1 コスト0,1
 *   1->2 コスト0,2
 *   2->3 コスト0,4
 *   ...
 * というグラフを構築すればよい。
 * これはLの上限が約2^20でNに近く、bitごとに考えることで思いつける。
 * 
 * ここまでは思いついたが、2^rを超える部分の構築ができなかった。
 * 2^r<=Lを満たす最大のrを考えてみる。
 * 例えばL=13のときはr=3で、コストが[0,8)までは前述のように構築できる。
 * 残りは13-8=5で b'101 
 * addの初期値を2^rとして、bitが立っているところを下の桁から
 *   頂点iから末尾へ、コストaddの辺を引く
 *   add+=2^i
 * とすると上手くいく。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・構築系で2のべき乗がポイントになるとき、端数をどう考えるか。
 *   端数は L-2^r になるので、これをまたbitごとに考えてみる。
**/

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)

// 2^r<=Lを満たす最大のrを返す
// 制約のL>=2を前提としている
ll cal(ll L)
{
	ll r;
	for(r = 1; r <= 20; r++)
	{
		if(1LL<<(r+1) > L) break;
	}
	return r;
}

int main(void)
{
	ll i;
	ll L; cin >> L;
	ll r = cal(L);
	ll N = r+1;
	vector<array<ll,3>> e;  // {u, v, w}
	for(i = 0; i < N-1; i++)
	{
		e.push_back({i, i+1, 0});
		e.push_back({i, i+1, 1LL<<i});
	}

	ll x = L-(1LL<<r);  // 残り
	ll add = 1LL<<r;
	for(i = 0; i < 22; i++)
	{
		if(isbiton(x, i))
		{
			e.push_back({i, N-1, add});
			add += 1LL<<i;
		}
	}
	ll M = (ll)e.size();
	cout << N << " " << M << endl;
	for(auto &[u, v, w] : e)
	{
		cout << u+1 << " " << v+1 << " " << w << endl;
	}

	return 0;
}
