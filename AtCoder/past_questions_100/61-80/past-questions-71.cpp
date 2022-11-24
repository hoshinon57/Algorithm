#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1000000007;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題71
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/s8pc-1/tasks/s8pc_1_e

/*
 * べき乗と累積和で解く。
 * 
 * dist[i]を、街1から街iまでの距離とする。累積和で持つ形になる。
 * すると、例えば街3->街5への距離は、dist[5]-dist[3]になる。
 * これをC1～CQまで加算していけばよい。
 * 
 * ハマった点として、街5->街3への距離も、dist[5]-dist[3]で求める必要があるっぽい。
 * 街番号が大きい方から小さい方を引く。
 * (MODの計算があるためか、dist[3]-dist[5]では正しく計算できなかった)(※)
 * 
 * (※ 少し具体例)
 * MOD 100として、
 *   87 --(+10)--> 97 --(+5)--> 2
 * のとき、 97-2 の形で距離の5を求めるのが面倒。 (2+MOD-97)%MOD が自然。
 * もしくは、 abs(dist[to]+MOD-dist[from]) % MOD な感じで左右両方向を一般化して計算できそうだが、ちょっと面倒かな…
 */

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
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
	// 1-indexed
	int i;
	int N, Q;
	cin >> N >> Q;
	vector<ll> dist(N+1);  // dist[i]:街1から街iまでの距離 (i=1～N)
	vector<ll> a(N+1);
	for(i = 1; i <= N; i++)
	{
		cin >> a[i];
		if(i == 1) continue;

		// 以下、iが2以上
		// dist[i] = dist[i-1] + a[i-1]^a[i]
		dist[i] = dist[i-1] + modpow(a[i-1], a[i], MOD);
		dist[i] %= MOD;
	}

	ll answer = 0;
	ll prev_c = 1;
	for(i = 1; i <= Q; i++)
	{
		ll c;
		cin >> c;
		// 街の番号が大きい方から小さい方を引く必要がある
		if(c > prev_c)
		{
			answer += dist[c] + MOD - dist[prev_c];
		}
		else
		{
			answer += dist[prev_c] + MOD - dist[c];
		}
		answer %= MOD;
		prev_c = c;
	}
	// 最後にCQ->1のルートを別途計算
	answer += dist[prev_c] + MOD - dist[1];
	answer %= MOD;

	cout << answer << endl;

	return 0;
}
