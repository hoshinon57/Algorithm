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

// ABC179 https://atcoder.jp/contests/abc179

/*
 * ダブリングで解こうとするも自力で解けず、解説サイトを見た。
 *   https://drken1215.hatenablog.com/entry/2020/09/20/091500
 * 
 * ダブリング用の変数を、以下のように2つ用意する。
 *   nxt[k][i]:A=iの要素の、2^k個先のAの値
 *   sum[k][i]:A=iの要素から数えて、2^k個ぶんの総和
 * 事前計算は以下のようになる。
 * (1)[0][i]の算出
 *   A(n+1) = An^2なので、nxt[0][i] =i*i
 *   A=iの要素から2^0=1個ぶんの総和はiそのものなので、sum[0][i]=i
 * (2)[k][i]の算出
 *   nxt[k][i] = nxt[k-1][nxt[k-1][i]]
 *   A=iから2^(k-1)個ぶんと、そこからさらに2^(k-1)個ぶんの総和として考えて、sum[k][i] = sum[k-1][i] + sum[k-1][nxt[k-1][i]]
 * 
 * 後はXをスタート地点として、
 * Nのビットが立っている部分について計算していけばよい。
 * 
 * 計算量は事前計算の部分で O(MlogN).
 * ※ただし実装ではNの最大で決め打ちしている
 * 
 * 【自力で解けなかった反省点】
 * 最初、ダブリング用の変数db[k][i]を考えたときに、
 * db[*][0]またはdb[*][1]を初期値A1=Xとして解こうとしてしまった。
 * やるべきはA1=Xならばdb[*][X]を使うのが筋かなと。
 */
int main(void)
{
	ll i, k;
	const ll LOG_N = 40;  // Nの最大は10^10 < 2^40
	ll N, X, M;
	cin >> N >> X >> M;
	// nxt[k][i]:A=iの2^k先のAの値
	// sum[k][i]:A=iの要素から数えて2^k個ぶんの総和
	vector<vector<ll>> nxt(LOG_N+1, vector<ll>(M));  // nxt[LOG_N+1][M]
	vector<vector<ll>> sum(LOG_N+1, vector<ll>(M));  // sum[LOG_N+1][M]
	for(i = 0; i < M; i++)
	{
		nxt[0][i] = (i*i)%M;
		sum[0][i] = i;  // A=iとなる要素から1個分の和、つまりi
	}
	for(k = 1; k <= LOG_N; k++)
	{
		for(i = 0; i < M; i++)
		{
			nxt[k][i] = nxt[k-1][nxt[k-1][i]];
			sum[k][i] = sum[k-1][i] + sum[k-1][nxt[k-1][i]];  // A=iから2^(k-1)個ぶんと、そこからさらに2^(k-1)個ぶんの総和
		}
	}

	ll answer = 0;
	ll cur = X;
	for(k = 0; k <= LOG_N; k++)
	{
		if( ((N>>k)&1) == 1 )
		{
			answer += sum[k][cur];
			cur = nxt[k][cur];
		}
	}
	cout << answer << endl;

	return 0;
}
