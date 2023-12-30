#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cassert>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC321 https://atcoder.jp/contests/abc321

/*
 * LCAの考え方で解く。  ※コンテスト時は別の方針で解いた
 * 以下、頂点は1-indexed, 深さは0-indexedとしている。
 * 
 * まず、以下の関数を定義する。
 *   calc(x, k, N) 頂点xから深さkだけ下がったところにある頂点数を返す
 * 上限Nというのをいったん忘れると、[x*2^k, (x+1)*2^k) の半開区間で示される頂点となる。
 * 上限がNなので、上記と[1,N]の重なる範囲を返せばよい。
 * 
 * 頂点Xとの距離がKであるような頂点について、そのLCAを考えると、以下の3パターンの総和となる。
 * (1)X自身がLCAのケース
 * XからKだけ下がった頂点が対象なので、 calc(X, K, N) となる。
 * 
 * (2)XのK個親のケース
 * K>0かつXの深さがK以上のときのみ存在する。
 * 個数は1個。
 * 
 * (3)Xからi上がり、そこから反対側へK-iだけ下りるケース
 * i:1～(Xの深さ)までそれぞれ計算する。ループ回数はO(logN).
 * 「反対側」を簡易に処理するために、
 *   頂点p:Xの"i-1"個だけ親にあり、その兄弟の頂点
 * とすると、pはXの"i-1"個親の、xor 1で求められる。
 * そしてpから(K-i-1)だけ下りた頂点が対象となるので、calc(X, K-i-1,N)となる。
 * 
 * (3)はiごとにO(1)で計算でき、iのループ回数はO(logN)であるため、クエリごとにO(logN).
 * 全体でO(TlogN)となり、間に合う。
 */

// 0-indexedで頂点vの深さを返す
ll depth (ll v)
{
	ll ans = 0;
	while(true)
	{
		v >>= 1;
		if(v == 0) break;
		ans++;
	}
	return ans;
}

// 頂点xから深さkだけ下がったところにある頂点数を返す [1,N]の範囲
ll calc(ll x, ll k, ll N)
{
	ll dx = depth(x);
	ll dn = depth(N);
	if(dx+k > dn) return 0;

	ll l = x * (1LL<<k);
	ll r = (x+1) * (1LL<<k) - 1;
	chmin(r, N);
	if(l > r) return 0;
	
	return r-l+1;
}

void solve(void)
{
	ll N, X, K; cin >> N >> X >> K;
	ll depthX = depth(X);
	ll ans = 0;

	// X自身がLCAのケース
	ll tmp = calc(X, K, N);
	ans += tmp;

	// XのK個親のケース
	if(K > 0 && depthX >= K) ans++;  // K個上がれる  K=0は前のパターンに含まれる

	// Xからi上がり、そこから反対へK-iだけ下りるケース
	ll i;
	for(i = 1; i <= depthX; i++)
	{
		if(K-i <= 0) break;  // 反対側に下りれない
		ll p = (X/(1LL<<(i-1))) ^ 1LL;  // Xのi-1個親で、その反対側の頂点
		// pからK-i-1だけ下りる (K-i-1=0でも可)
		ll tmp2 = calc(p, K-i-1, N);
		ans += tmp2;
	}

	cout << ans << endl;
}

int main(void)
{
#if 0
	assert(depth(1) == 0);
	assert(depth(2) == 1);
	assert(depth(3) == 1);
	assert(depth(8) == 3);
	assert(depth(15) == 3);
	assert(calc(2, 1, 10) == 2);
	assert(calc(2, 2, 10) == 3);
	assert(calc(2, 2, 16) == 4);
	assert(calc(1, 200, INF64) == 0);
	assert(calc(1, 3, INF64) == 8);
	assert(calc(8, 1, 15) == 0);
	assert(calc(3, 2, 12) == 1);
	assert(calc(2, 0, 10) == 1);
	assert(calc(5, 0, 10) == 1);
#endif
	int t; cin >> t;
	while(t > 0) {solve(); t--;}

	return 0;
}
