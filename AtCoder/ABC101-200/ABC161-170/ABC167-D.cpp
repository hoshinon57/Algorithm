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

// ABC167 https://atcoder.jp/contests/abc167

/*
 * ダブリングで解く。
 * 
 * doubling[k][i]:町iから2^k進んだ町
 * として定義する。
 * doubling[0][i]は、入力のAiとなる。
 * doubling[k][i]は、"町iから2^(k-1)進んだ町" から "2^(k-1)進んだ町" にて求められる。
 * ここまでが前処理。
 * 
 * K個先の町を求めるには、Kを2進数で見たときにビットが立っている部分について、
 * doubling[]を使って遷移先を求めればよい。
 * 
 * 計算量は、前処理にO(N*logK), クエリにO(logK)となる。
 */
int main(void)
{
	// 0-indexed
	ll i, k;
	ll N, K;
	cin >> N >> K;
	vector<ll> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		a[i]--;
	}
	const int LOG_K = 60;  // Kの最大は10^18 < 2^60
	// doubling[k][i]:町iから2^k進んだ町を指す
	vector<vector<ll>> doubling(LOG_K+1, vector<ll>(N));

	// doubling[0][*]を計算
	for(i = 0; i < N; i++)
	{
		doubling[0][i] = a[i];
	}

	// doubling[k][*]を計算
	for(k = 1; k <= LOG_K; k++)
	{
		for(i = 0; i < N; i++)
		{
			doubling[k][i] = doubling[k-1][doubling[k-1][i]];
		}
	}

	// 例えばK=13(b'1101)であれば、
	// 1個進んだ先、4個進んだ先、8個進んだ先 を組み合わせれば求められる
	// それぞれ 2^0, 2^2, 2^3 に相当する
	int answer = 0;
	int n = 0;
	while(K > 0)
	{
		if((K&1) == 1)  // bitが立っている場合
		{
			answer = doubling[n][answer];
		}
		n++;
		K >>= 1;
	}
	cout << answer+1 << endl;  // 0-indexedで計算していたので、+1

	return 0;
}
