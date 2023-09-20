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

// ABC199 https://atcoder.jp/contests/abc199

/*
 * bitDPで解く。
 * 
 * dp[S]を以下のように定義する。
 *   |S|をSのうちbitが立っているものの個数とする。
 *   数列aの先頭|S|個が決まっており、それがSにてbitが立っている部分としたときに、
 *   その時点でY,Zに関する条件に「違反していない」選び方の総数
 * dp[(1<<N)-1]が答。
 * 
 * 以下の要領で、メモ化再帰にて計算していく。
 * (1)条件違反が無いか調べる。
 *   x=|S|である{x,y,z}の組み合わせについて、
 *   Sの下からy個のbitにて1がz個以下であればセーフとなる。
 *   違反があれば、dp[S]=0.
 * (2)ベースケースを考える。
 *   |S|=1のときdp[S]=1.
 * (3)再帰を考える。
 *   dp[S]は、Sの各bitを1つ落としたものの総和となる。
 *   数列aにまた1つ数値を追加するイメージ。
 * 
 * 
 * [ACまでの思考の流れ]
 * ・メタ読みだが、N=18という制約からbitDPが見える。
 * ・数列aのうち前半3個が決まっており、次の4個目を考えたいときに、
 *   前半3個のうち「何を使ったか」が重要で、「どんな順番か」は気にする必要がない。
 *   こういった場合はbitDPが活用できる。
 */

vector<ll> dp;  //  dp[1<<N]
vector<vector<pair<ll,ll>>> cond;  // cond[x]={y,z}

ll bitDP(ll S, ll N)
{
	if(dp[S] != -1) return dp[S];  // メモ化済み
	int bitS = __builtin_popcountll(S);

	// 条件違反があれば0
	for(auto &e : cond[bitS])
	{
		// Sの下からy個のbitにて、1がz個以下ならセーフ
		auto& [y, z] = e;
		ll tmp = S&((1LL<<y)-1);
		if(__builtin_popcountll(tmp) > z)  // 条件違反
		{
			return (dp[S] = 0);
		}
	}

	// ベースケース
	if(bitS == 1) return (dp[S] = 1);

	ll d = 0;
	for(int i = 0; i < N; i++)
	{
		// Sから1つbitを落としたものを加算していく
		if((S&(1<<i)) == 0) continue;  // bitが立っていない
		ll S_prev = S & (~(1<<i));
		d += bitDP(S_prev, N);
	}

	return (dp[S] = d);
}

int main(void)
{
	ll N, M; cin >> N >> M;
	dp.resize(1<<N, -1);
	cond.resize(N+1);
	for(int i = 0; i < M; i++)
	{
		ll x, y, z; cin >> x >> y >> z;
		cond[x].push_back({y, z});
	}

	cout << bitDP((1<<N)-1, N) << endl;

	return 0;
}
