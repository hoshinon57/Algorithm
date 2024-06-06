#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC291 https://atcoder.jp/contests/abc291

/*
 * 自力で解けず、解説を見た。
 *   https://x.com/kyopro_friends/status/1629840748744151040
 *   https://note.com/uyisl/n/n88a435c83a05
 *   https://atcoder.jp/contests/abc291/editorial/5846
 * 
 * M=10という制約に着目する。
 * 都市kを通らないという小問題について、都市1からNへの移動は以下のルートとなる。
 *   1 -> (kの左側M個のどこか) -> (kの右側M個のどこか) -> N
 * 
 * (kの左側M個のどこか) -> (kの右側M個のどこか)
 * は入力Sを用いてO(M^2)で求められるので、全体でO(N*M^2)となり、十分間に合う。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・1->a->b->N という経路での距離を求めたい場合(a->bは直結)、1->aとb->Nの距離を合わせるという考え方。
 *   b->NはNを起点としたBFSで求められる。
 *   始点/終点からそれぞれBFS(ダイクストラ)をし、その結果を合わせる[典型]
 *   本問ではM=10のため、a,bの組み合わせM^2通りを全探索しても間に合う。
 * ・グラフにおいて、辺i->jが常にi<j, つまり戻る方向の移動ができない場合、
 *   BFSやダイクストラ法を使わずとも、DPでdistを求めることができる。
 */

using Graph = vector<vector<int>>;

// 頂点sを起点にBFS.
// distは未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void bfs_graph(Graph &G, int s, vector<int> &dist)
{
	queue<int> que;
	dist[s] = 0;
	que.push(s);

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		for(auto &e : G[v])
		{
			if(dist[e] != INF32) continue;
			dist[e] = dist[v] + 1;
			que.push(e);
		}
	}
	return;
}

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	Graph g(N), rg(N);
	vector<string> s(N);
	for(i = 0; i < N; i++)
	{
		cin >> s[i];
		for(j = 0; j < M; j++)
		{
			if(i+j+1 >= N) break;
			if(s[i][j] == '0') continue;
			g[i].push_back(i+j+1);
			rg[i+j+1].push_back(i);  // 逆方向
		}
	}

	vector<int> dist(N, INF32), dist_r(N, INF32);
	bfs_graph(g, 0, dist);
	bfs_graph(rg, N-1, dist_r);

	for(int k = 1; k < N-1; k++)
	{
		int ans = INF32;
		int st = max(k-M, 0), ed = min(k+M, N-1);  // kの前後M個
		for(i = st; i < k; i++)
		{
			for(j = k+1; j <= ed; j++)  // i->j
			{
				if(j-i > M) continue;
				if(s[i][j-i-1] == '0') continue;
				int tmp = dist[i] + dist_r[j] + 1;  // +1はi->jへの移動
				chmin(ans, tmp);
			}
		}
		if(ans == INF32) ans = -1;
		cout << ans;
		if(k != N-2) cout << " ";
	}
	cout << endl;

	return 0;
}
