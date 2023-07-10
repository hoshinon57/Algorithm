#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC309 https://atcoder.jp/contests/abc309

/*
 * ダイクストラ法で解く。
 * (ABC305-Eとほぼ同じ解法)
 * 
 * 親から子へ有向辺を張り、{入力yの-1倍, 保険の加入者x}をキューに積んでいく
 * (ダイクストラなので辺の重みを正にしたいため)
 * 辺の重みを1としてダイクストラ法を行い、
 * dist[]が0以下である頂点の数が答となる。
 * 
 * 実装では、(x,y)の入力にてxに対する -y が前回のものより小さい入力のみキューに積む。
 * (より末代まで保障できる入力のみ許可するイメージ)
 *   1 3
 *   1 2
 * のような入力をそのまま処理すると、dist[1]が2で上書きされてしまい、正しく計算できないため。
 */

using Graph = vector<vector<int>>;
using pii = pair<int,int>;

int main(void)
{
	// 0-indexed
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 1; i < N; i++)
	{
		int p; cin >> p;
		p--;
		g[p].push_back(i);  // 親から子へ
	}

	vector<int> dist(N, INF32);
	priority_queue<pii, vector<pii>, greater<pii>> que;  // {その頂点の距離, 頂点番号}
	for(i = 0; i < M; i++)
	{
		int x, y; cin >> x >> y;
		x--;
		if(-y < dist[x])  // より末代まで保障できる入力のみ許可
		{
			dist[x] = -y;  // -1倍
			que.push({dist[x], x});
		}
	}

	while(!que.empty())
	{
		int d = que.top().first;
		int v = que.top().second;
		que.pop();

		if(d > dist[v]) continue;

		for(auto &e : g[v])
		{
			if(dist[e] > dist[v]+1)  // 辺の重さは1とする
			{
				dist[e] = dist[v]+1;
				que.push({dist[e], e});
			}
		}
	}

	int ans = 0;
	for(i = 0; i < N; i++)
	{
		if(dist[i] <= 0)
		{
			ans++;
		}
	}
	cout << ans << endl;

	return 0;
}
