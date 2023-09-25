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
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC305 https://atcoder.jp/contests/abc305

/*
 * ダイクストラ法で解く。
 * 
 * ※条件について、頂点ではなく警備員に着目して考えるとやりやすい。
 *   つまり、頂点pにいる体力hの警備員は、そこから距離hまでの頂点を警備できる、という考え方。
 * 
 * まず通常、ダイクストラ法は頂点の距離が小さい順に処理するので、
 * 警備員のHPを-1倍したものを初期値として与えることとする。
 * 以下の手順、考え方で解けばよい。
 *   (1)警備員がいる頂点の距離distを、体力の-1倍で初期化し、優先度付きキューに登録する。
 *   (2)辺の重みは全て1であるとして、ダイクストラ法を適用する。
 *   (3)最後に、distが0以下である頂点が、警備されている頂点である。
 */

using Graph = vector<vector<int>>;
using pii = pair<int,int>;

int main(void)
{
	int i;
	int N, M, K;
	cin >> N >> M >> K;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int a, b;
		cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}

	vector<int> dist(N, INF32);
	priority_queue<pii, vector<pii>, greater<pii>> que;  // {その頂点の距離, 頂点番号}
	for(i = 0; i < K; i++)
	{
		int p, h;
		cin >> p >> h;
		p--;
		dist[p] = -h;  // -1倍
		que.push({dist[p], p});
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

	int ans_count = 0;
	vector<int> ans;
	for(i = 0; i < N; i++)
	{
		if(dist[i] <= 0)
		{
			ans_count++;
			ans.push_back(i+1);
		}
	}

	cout << ans_count << endl;
	for(auto &e : ans) cout << e << " ";
	cout << endl;

	return 0;
}
