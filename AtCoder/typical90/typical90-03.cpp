#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:03 https://atcoder.jp/contests/typical90/tasks/typical90_c

/*
 * グラフの直径は最短距離計算を2回やる、という典型。
 * https://twitter.com/e869120/status/1377752658149175299
 * を参考に、BFSによる各頂点への距離計算を2回実施する。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・問題で求めているのは、グラフ(木)における頂点どうしの距離の最大値。
 *   これはグラフの直径を求める問題になる。
 * 
 * ※自力AC時は「グラフの直径」というのを知らずに解いた。
 *   https://atcoder.jp/contests/typical90/submissions/49607732
 *   とはいえ以下のけんちょん氏解説を見ると、結果としてDFSでグラフの直径を求める実装になってたのかな…と思う。
 *   https://drken1215.hatenablog.com/entry/2021/06/12/181400
 */

using Graph = vector<vector<int>>;

// vを起点としたときの各頂点への距離を、distに格納する
void bfs(Graph &g, int s, vector<int> &dist)
{
	queue<int> que;
	dist[s] = 0;
	que.push(s);

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		for(auto &e : g[v])
		{
			if(dist[e] != INF32) continue;
			dist[e] = dist[v] + 1;
			que.push(e);
		}
	}
}

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}

	// 距離計算1回目
	vector<int> dist(N, INF32);
	bfs(g, 0, dist);

	// 最も遠い頂点を探す
	int maxd = 0;
	int max_v;
	for(i = 0; i < N; i++)
	{
		if(maxd < dist[i])
		{
			maxd = dist[i];
			max_v = i;
		}
	}

	// 距離計算2回目
	dist.assign(N, INF32);
	bfs(g, max_v, dist);
	
	// 最も遠い頂点への距離が直径、本問ではその+1が答
	int ans = *max_element(dist.begin(), dist.end()) + 1;
	cout << ans << endl;

	return 0;
}
