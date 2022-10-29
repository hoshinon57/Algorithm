#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題61
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc012/tasks/abc012_4

/*
 * ワーシャルフロイド法で解く。
 * 
 * まず入力に対して、ワーシャルフロイド法にて各バス停間の最短時間を求める。これをdist[][]とする。
 * 次に、
 *   (1)dist[i][*]の最大値を求める。これは問題文におけるi番目のバス停の「最悪のケース」を指す。
 *   (2)全てのiに対して、この最小値を求める。
 * (2)の最小値が答となる。
 */

int main(void)
{
	// 0-indexed
	int i, j, k;
	int N, M;
	cin >> N >> M;
	vector<vector<ll>> dist(N, vector<ll>(N, INF64));  // dist[i][j]:i->jへの最短距離
	for(i = 0; i < M; i++)
	{
		int a, b;
		ll t;
		cin >> a >> b >> t;
		a--;  // 0-indexedに変換
		b--;
		// 無向グラフ
		dist[a][b] = dist[b][a] = t;
	}
	for(i = 0; i < N; i++) dist[i][i] = 0;  // 同じ地点は距離0で初期化

	// ワーシャルフロイド法で全バス停間の最短時間を求める
	for(k = 0; k < N; k++)
	{
		for(i = 0; i < N; i++)
		{
			for(j = 0; j < N; j++)
			{
				// dist[i][j] <- dist[i][k] + dist[k][j]
				if(dist[i][k] == INF64 || dist[k][j] == INF64) continue;  // 辺が繋がっていなければスルー
				dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);
			}
		}
	}
	// 問題文より、負閉路は無く、またdist[i][j]=INFとなるようなi,jは存在しないと考えて良い。
	// ＞その移動に何分かかるかを表す数字 ti(1≦ti≦1000)
	// ＞辿り着けないバス停のペアは存在しないことが保証されている。

	// dist[i][*] の最大値を求める
	// 全てのiに対して、この最大値が最も小さいものが答となる
	ll answer = INF64;
	for(i = 0; i < N; i++)
	{
		ll tmp = 0;  // 自宅を地点iとしたときに、問題文における「最悪のケース」を求める
		for(j = 0; j < N; j++)
		{
			tmp = max(tmp, dist[i][j]);
		}
		answer = min(answer, tmp);
	}
	cout << answer << endl;

	return 0;
}
