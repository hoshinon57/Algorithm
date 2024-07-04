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

// ABC338 https://atcoder.jp/contests/abc338

/*
 * ワーシャルフロイド法、およびbitDPで解く。
 *   https://atcoder.jp/contests/abc338/editorial/9170
 *   https://x.com/kyopro_friends/status/1751241385968374200
 *   https://torus711.hatenablog.com/entry/2024/01/28/185642
 * 
 * 基本は上記解説の通り。
 * dp[S][v]を以下のように定義する。
 *   Sの各ビットを各頂点に割り当てて、
 *   1となっている頂点を(1回以上)通り、最後に頂点vにいるときの
 *   通る辺の重みの最小値。
 * dp[(1<<N)-1][*] の最小値が答。
 * 
 * 入力例1では2->1->2->3と通るが、bitDP的には2->1->3と計算していくイメージ。
 * 
 * 頂点u->vへ行けない場合のdist[]やdp[]の値をINFとしている。
 * 負閉路は無いが負辺はあるので、非INFでもNoが正解であるケースがありうる。そのため
 * ・dist[]やdp[]の値がINFならスキップ
 * ・答出力にてNoのしきい値をINF/2とする
 * などの配慮を行っている。
 * (前者をやれば後者は不要だったかも)
 */

int N, M;
vector<vector<ll>> dist;  // dist[i][j]:辺i->jへのコスト 辺が無ければINF64
vector<vector<ll>> dp;
vector<vector<bool>> memo;

// dp[S][v]を返す
ll bitdp(int S, int v)
{
	ll &d = dp[S][v];
	if(memo[S][v]) return d;
	memo[S][v] = true;

	// ベースケース
	if(__builtin_popcount(S) == 1) return d = 0;

	int Sprev = S & (~(1<<v));
	d = INF64;
	for(int i = 0; i < N; i++)  // 頂点i->v
	{
		if((Sprev&(1<<i)) == 0) continue;  // iビット目が立っていない
		if(dist[i][v] == INF64) continue;  // i->vへたどれない場合

		ll tmp = bitdp(Sprev, i);
		if(tmp == INF64) continue;
		chmin(d, tmp + dist[i][v]);
	}

	return d;
}

int main(void)
{
	int i, j, k;
	cin >> N >> M;
	dist.resize(N, vector<ll>(N, INF64));
	for(i = 0; i < N; i++) dist[i][i] = 0;  // 負閉路は無い
	for(i = 0; i < M; i++)
	{
		ll u, v, w; cin >> u >> v >> w;
		u--; v--;
		dist[u][v] = w;
	}
	for(k = 0; k < N; k++)
	{
		for(i = 0; i < N; i++)
		{
			for(j = 0; j < N; j++)
			{
				if(dist[i][k] != INF64 && dist[k][j] != INF64)  // i-k, k-j間がどちらも通れる場合
				{
					dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);
				}
			}
		}
	}

	dp.resize(1<<N, vector<ll>(N, INF64));
	memo.resize(1<<N, vector<bool>(N, false));
	ll ans = INF64;
	for(i = 0; i < N; i++)
	{
		ll tmp = bitdp((1<<N)-1, i);
		chmin(ans, tmp);
	}
	if(ans >= INF64/2) cout << "No" << endl;  // /2は不要かも bitdp()内で判定しているので
	else cout << ans << endl;

	return 0;
}
