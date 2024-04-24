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
const ll MOD = 1e9+7;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * ダブリングで解く。(想定解法は行列累乗だった)
 * 
 * db[i][j][k]を以下のように定義する。
 *   頂点i->jへ、距離2^kで到達できるパスの数
 * これはダブリングの要領で各要素を求めることができる。
 * 
 * 長さKの有向パスは、Kにてbitが立っている部分を加算していけばよい。
 * 実装を参照。
 */

#if 1
const ll LOG_K = 60;
const ll MAX_N = 50;
#else  // dbg
const ll LOG_K = 2;
const ll MAX_N = 4;
#endif
// db[i][j][k]
// 頂点i->jへ、距離2^kで行けるパス数
ll db[MAX_N][MAX_N][LOG_K+1];

int main(void)
{
	// 0-indexed
	ll i, j, k;
	ll N, K; cin >> N >> K;
	vector<vector<ll>> a(N, vector<ll>(N, 0));
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			cin >> a[i][j];
			db[i][j][0] = a[i][j];  // 2^0=距離1
		}
	}

	// forの順番はkを外側に置く必要がある (内側においてWAが取れずハマった)
	for(k = 1; k <= LOG_K; k++)  // この時点で db[*][*][k-1] は全て確定している
	{
		for(i = 0; i < N; i++)
		{
			for(j = 0; j < N; j++)
			{
				for(ll x = 0; x < N; x++)
				{
					db[i][j][k] += db[i][x][k-1] * db[x][j][k-1];
					db[i][j][k] %= MOD;
				}
			}
		}
	}

	vector<ll> tmp1(N, 1);  // tmp1[i]:頂点iを終点とするパスの数 初期値は各頂点に1
	for(k = 0; k <= LOG_K; k++)
	{
		vector<ll> tmp2(N, 0);
		if((K&1) == 1)
		{
			for(i = 0; i < N; i++)
			{
				for(j = 0; j < N; j++)
				{
					// i->jへ距離2^kのパスの数を、tmp[j]へ
					tmp2[j] += tmp1[i] * db[i][j][k];
					tmp2[j] %= MOD;
				}
			}
			swap(tmp1, tmp2);
		}
		K >>= 1;
	}
	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		ans += tmp1[i];
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}
