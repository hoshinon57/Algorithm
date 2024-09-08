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

// ABC142 https://atcoder.jp/contests/abc142

/*
 * 幅優先探索で解く。
 *   https://atcoder.jp/contests/abc142/editorial/945
 *   https://drken1215.hatenablog.com/entry/2019/09/29/012000
 *   https://blog.hamayanhamayan.com/entry/2019/09/28/230647
 * 
 * 一言で書くと、「グラフ全体における最短のサイクルが答」となる。
 * よって各頂点vからBFSを行い、{始点vを通るサイクルの最短の長さ, v}を更新していく。
 * 最後にvからサイクルを復元すればよい。
 * 
 * 根拠について考える。
 * まず、次数が入出ともに1であることから、誘導部分グラフはサイクルとなる。
 * ただし {a,b,c,d,a} というサイクルに対し b->d という分岐があると、条件を満たさなくなる。
 * この分岐について、遠回りとなる辺を削除していくと、最終的に誘導部分グラフになる。
 * よってサイクル長が最短のものを求めると、それには分岐が存在しないので、誘導部分グラフとなる。
 * 
 * [ACまでの思考の流れ]
 * ・制約的に、各頂点からBFSをやって間に合う。
 * ・答はどういうグラフになるかを考えると、サイクルになりそう。
 * ・ただ例3などを見て、途中に分岐があるとうまくいかない。
 *   注記にあるように、誘導部分グラフの頂点V'を両端点に持つ辺は全て選ぶ必要があるので。
 * ・ここから色々考えて…、多少エスパーっぽい感じはしたが、
 *   サイクル長が最も短いやつを選べば確実に条件を満たしそうだ、となった。
 */

using Graph = vector<vector<int>>;

// 頂点sを起点にBFS.
// distは未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
// 
// [ABC142-F用の変化点]
// 始点sを通るサイクルがある場合、dist[s]を0以外の値に更新する
// 復元用に、1つ前の頂点を保持するpreを追加
void bfs_graph(Graph &G, int s, vector<int> &dist, vector<int> &pre)
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
			if(e == s)
			{
				if(dist[e] != 0) continue;  // 始点は1回だけ更新
			}
			else if(dist[e] != INF32) continue;
			dist[e] = dist[v] + 1;
			pre[e] = v;
			que.push(e);
		}
	}
	return;
}

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
	}

	int v, lp = INF32;  // サイクルの長さが最小となる始点vと、サイクルの長さ
	for(i = 0; i < N; i++)
	{
		vector<int> dist(N, INF32), pre(N);
		bfs_graph(g, i, dist, pre);
		if(dist[i] != 0)  // サイクルがあれば、dist[i]にその長さが保存される
		{
			if(chmin(lp, dist[i])) v = i;
		}
	}
	if(lp == INF32)  // サイクル無し
	{
		cout << -1 << endl;
		return 0;
	}

	// 復元
	vector<int> dist(N, INF32), pre(N);
	bfs_graph(g, v, dist, pre);
	vector<int> ans;
	while(true)
	{
		ans.push_back(v);
		v = pre[v];
		if(v == ans[0]) break;
	}
	cout << ans.size() << endl;
	for(auto &e : ans) cout << e+1 << endl;

	return 0;
}
