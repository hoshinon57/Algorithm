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

// ABC286 https://atcoder.jp/contests/abc286

/*
 * 解説を元に、ワーシャルフロイド法で解いてみた版。	
 *   https://x.com/kyopro_friends/status/1616795218204069888
 *   https://atcoder.jp/contests/abc286/editorial/5572
 *   https://atcoder.jp/contests/abc286/submissions/38289903  snuke氏
 * 
 * {距離, (おみやげの総和)の-1倍} をキーにしたワーシャルフロイド法を実施する。
 * (自作のライブラリから、distの型をll->pllにして他も少しいじれば完了した)
 * おみやげは-1倍することで、キーは「小さい方が良い」になり処理が楽。
 * 
 * 入力にてi->jへの直行便があるならば、 {1,-A[j]} を設定する。
 * 
 * クエリu->vについて、始点uのおみやげを加算していないので、クエリ時に加算する。
 */

using pll = pair<ll,ll>;

// dist:{距離, おみやげの-1倍} 小さい方が良い
void Warshall_Floyd(vector<vector<pll>> &dist)
{
	int i, j, k;
	int N = dist.size();

	for(k = 0; k < N; k++)
	{
		for(i = 0; i < N; i++)
		{
			for(j = 0; j < N; j++)
			{
				if(dist[i][k].first != INF64 && dist[k][j].first != INF64)  // i-k, k-j間がどちらも通れる場合
				{
					pll tmp = {dist[i][k].first + dist[k][j].first, dist[i][k].second + dist[k][j].second};
					dist[i][j] = min(dist[i][j], tmp);
				}
			}
		}
	}
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<vector<pll>> dist(N, vector<pll>(N, {INF64, 0}));
	for(i = 0; i < N; i++) dist[i][i].first = 0;
	for(i = 0; i < N; i++)
	{
		string s; cin >> s;
		for(j = 0; j < N; j++)
		{
			if(s[j] == 'Y') dist[i][j] = {1, -a[j]};  // 距離1, (行き先のおみやげ)の-1倍
		}
	}
	Warshall_Floyd(dist);

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		int u, v; cin >> u >> v;
		u--; v--;
		auto [d,val] = dist[u][v];
		if(d == INF64)
		{
			cout << "Impossible" << endl;
		}
		else
		{
			// おみやげについて、-1倍で登録しているので反転 また始点のおみやげは加算していなかったのでここで加算
			cout << d << " " << (-val)+a[u] << endl;
		}
	}

	return 0;
}
