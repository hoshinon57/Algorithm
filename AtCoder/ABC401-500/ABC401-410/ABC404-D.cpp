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

// ABC404 https://atcoder.jp/contests/abc404

/*
 * 3^Nのbit全探索で解く。少し実装が大変。
 * 
 * 各動物園は3回以上行く必要は無い。0 or 1 or 2回のいずれか。
 * よって3^Nのbit全探索を行う。
 * 
 * ※コンテスト時は動物園がN*2個あるとして、2^(2*N)のbit全探索とした。
 *   こちらでも問題なくACできたが、
 *   ・1回行く、を ox と xo の2パターンで重複してカウントしてしまう
 *   ・3^10は6*10^4ぐらい、2^20は10^6ぐらい、と差が大きい
 *   ことから3^Nの方が有利。
**/

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// ※modは素数といった制約は無い
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	// a %= mod;  // a*aにてオーバーフローしそうな場合に有効化
	long long ans = 1;
	while(n > 0)
	{
		if((n&1) != 0)  // nの最下位ビットが立っているなら、答に掛ける
			ans = (ans * a) % mod;
		a = (a * a) % mod;
		n >>= 1;
	}
	return ans;
}

int main(void)
{
	ll i, j;
	ll N, M; cin >> N >> M;
	vector<ll> c(N); for(i = 0; i < N; i++) {cin >> c[i];}
	vector<vector<ll>> d(N);  // d[i][*]:動物園iで見れる動物のリスト
	for(i = 0; i < M; i++)
	{
		ll ki; cin >> ki;
		for(j = 0; j < ki; j++)
		{
			ll a; cin >> a;
			a--;  // 動物iは園aで見れる
			d[a].push_back(i);
		}
	}

	ll ans = INF64;
	// 動物園ごと、0 or 1 or 2回行く -> 3^N
	ll pw3 = modpow(3, N, INF64);
	for(i = 0; i < pw3; i++)
	{
		vector<ll> sum(M);  // sum[x]:動物xを見た回数
		ll val = 0;
		ll tmp = i;
		for(j = 0; j < N; j++)
		{
			ll cnt = tmp%3;  // 動物園jに行く回数
			tmp /= 3;
			for(auto &e : d[j])
			{
				sum[e] += cnt;
			}
			val += c[j]*cnt;
		}

		// 各動物を2回以上見たらtrue
		auto chk = [&](void) -> bool
		{
			for(j = 0; j < M; j++)
			{
				if(sum[j] < 2) return false;
			}
			return true;
		};
		if(chk())
		{
			chmin(ans, val);
		}
	}
	cout << ans << endl;

	return 0;
}
