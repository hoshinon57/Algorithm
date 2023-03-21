#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC241 https://atcoder.jp/contests/abc241

/*
 * ダブリングで解く。
 * 
 * ダブリング用の変数db[k][i]を以下のように定める。
 *   i番目の皿から操作を2^k回行った時に追加するアメの数
 * 
 * db[0][i]は入力の数列Aから求まる。
 * db[k][i]は、
 *   皿iを起点に操作を2^k回行うには、
 *   (1)皿iから操作を2^(k-1)回行い、
 *      ->db[k-1][i]
 *   (2)そこからさらに操作を2^(k-1)回行う
 *      ->db[k-1][ db[k-1][i]%N ]
 *   にて計算できる。
 * ここまでが事前計算。
 * 
 * 後はクエリKに対して、Kのビットが立っている部分についてdbを用いて計算していけばよい。
 * db[][]はアメの数を保持するので、それを配列の添字に使う場合はNの剰余を使う点に注意。
 * 
 * 計算量は、前処理にO(N*logK), クエリにO(logK)となる。
 * ※実装ではlogKではなくlog(Kが取りうる最大値)としている
 */
int main(void)
{
	ll i, k;
	ll N, K;
	cin >> N >> K;
	vector<ll> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
	}
	const int LOG_K = 45;  // Kの最大は10^12 < 2^45
	vector<vector<ll>> db(LOG_K+1, vector<ll>(N));

	// doubling[0][*]を計算
	for(i = 0; i < N; i++)
	{
		db[0][i] = a[i];
	}

	// doubling[k][*]を計算
	for(k = 1; k <= LOG_K; k++)
	{
		for(i = 0; i < N; i++)
		{
			ll n1, n2;
			n1 = db[k-1][i];
			n2 = db[k-1][(n1+i)%N];
			db[k][i] = n1 + n2;
		}
	}

	ll answer = 0;
	for(i = 0; i <= LOG_K; i++)
	{
		if((K&1) == 1)
		{
			answer += db[i][answer%N];
		}		
		K >>= 1;
	}
	cout << answer << endl;

	return 0;
}
