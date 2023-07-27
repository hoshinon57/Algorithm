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

// ABC258 https://atcoder.jp/contests/abc258

/*
 * 累積和＆二分探索とダブリングで解く。
 * 
 * potato_num[i]を以下で定義する。
 *   i番目のじゃがいもから箱詰めした場合に、その箱に入るじゃがいもの個数
 *   この求め方は後述。
 * ダブリング用の変数db[k][i]を以下で定義する。
 *   i番目のじゃがいもから箱詰めしていって、
 *   2^k番目の箱まで詰めたあとに、次は何番目のジャガイモから入れ始めるか
 * 
 * db[]の計算は以下。
 *   db[0][i] = i+potato_num[i]
 *   db[k][i] = db[k-1][db[k-1][i]]
 *   -> i番目から2^(k-1)個の箱まで詰めた後、さらにそこから2^(k-1)個詰めると、全体で2^k個になる
 * 
 * 各クエリKに対して、(K-1)個箱詰めした結果を、potato_num[]に入れたものが答となる。
 * 
 * 最後にpotato_num[i]の計算方法について。
 * i番目から愚直にw[i]を足していってX以上になるまで、とするのは
 * X=10^9, 各w[*]=1のようなときにO(10^9)の計算量になってしまう。
 * よって
 * ・loop_num = 重さがXに到達するまでにW(0)～W(N-1)を何周するか
 * ・zan = 上記の端数の重さ
 * とすると、loop_num*Nがループ分の個数。
 * 端数の重さの個数は、累積和＆二分探索で求める。
 * このとき、周期をN*2としておくと計算が楽。
 * 
 * 計算量は、
 *   potato_num[]の事前計算：O(NlogN)
 *   ダブリングの事前計算：O(NlogK)  ただし実装はK=10^12程度で決め打ちしている
 *   クエリ処理：O(QlogK)
 * となり、これらの和になると考える。
 */
int main(void)
{
	ll i, k;
	const int LOG_K = 45;  // K=10^12 < 2^40
	ll N, Q, X;
	cin >> N >> Q >> X;
	vector<ll> w(N*2);  // 2周ぶん確保
	vector<ll> w_sum(N*2+1);  // 累積和
	ll w_total = 0;  // w[0]～w[N-1]の総和
	for(i = 0; i < N; i++)
	{
		cin >> w[i];
		w[N+i] = w[i];  // 2周目
		w_total += w[i];
	}
	// 2周ぶんの累積和を計算
	for(i = 0; i < N*2; i++)
	{
		w_sum[i+1] = w_sum[i] + w[i];
	}

	vector<ll> potato_num(N);  // [i]:i番目のじゃがいもから箱詰めした場合に、その箱に入るじゃがいもの個数
	for(i = 0; i < N; i++)
	{
		// 重さがXに到達するまでにW(0)～W(N-1)を何周するかと、その端数
		ll loop_num = X / w_total;
		ll zan = X % w_total;
		// ループ分
		potato_num[i] = loop_num * N;
		// 端数は累積和＆二分探索で求める
		// 種類iから数えて"zan"以上になるまでなので、w_sum[i]+zanが境界
		auto itr = lower_bound(w_sum.begin(), w_sum.end(), w_sum[i]+zan) - w_sum.begin();
		potato_num[i] += itr - i;
	}

	// ダブリング事前計算
	vector<vector<ll>> db(LOG_K+1, vector<ll>(N));
	// db[0][*]の計算
	for(i = 0; i < N; i++)
	{
		db[0][i] = (i + potato_num[i]) % N;
	}
	// db[k][*]の計算
	for(k = 1; k <= LOG_K; k++)
	{
		for(i = 0; i < N; i++)
		{
			db[k][i] = db[k-1][db[k-1][i]];
		}
	}

	// クエリ処理
	while(Q > 0)
	{
		ll K, nxt = 0;
		cin >> K;
		K--;  // (K-1)個箱詰めした結果を、potato_num[]に入れたものが答
		for(k = 0; k <= LOG_K; k++)
		{
			if((K&1) == 1)
			{
				nxt = db[k][nxt];
			}			
			K >>= 1;
		}
		cout << potato_num[nxt] << endl;
		Q--;
	}

	return 0;
}
