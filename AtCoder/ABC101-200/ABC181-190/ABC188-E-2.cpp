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

// ABC188 https://atcoder.jp/contests/abc188

/*
 * 公式解説の解法2を実装した。
 * ABC188-E.cppより計算量は劣るが、本問題の制約に限らず一般のグラフでも解けるアルゴリズムとなる。
 * ※https://atcoder.jp/contests/abc188/editorial/477
 * 
 * まず、最も売買金額の安い町x1で金を買うことを考える。
 * そこから辿れる町をBFSで探索し、各町で売ったとして利益の最大値を保持しておく。
 * 
 * 次に、2番目に売買金額の安い町x2で金を買い、そこからBFSで探索する。
 * ただしこのとき、x1からたどった町については探索は打ち切って良い。
 * 
 * これをx3,x4,...と繰り返していくことで、利益の最大値が求まる。
 * 以前にたどった町については再探索はしないため、計算量もO(N+M)で抑えられる。
 */

using Graph = vector<vector<int>>;
vector<ll> a;
vector<pair<ll,int>> city;  // {売買金額, 町番号}
ll ans = -INF64;

// 頂点sを起点にBFS.
// distは未探索部分をINF32で埋めていること。
void bfs_graph(Graph &G, int s, vector<int> &dist)
{
	queue<int> que;
	// 本実装では、スタート地点のdistは更新しない。
	// スタート地点で買い、またスタート地点に戻ってきて売るケースを想定するため。
	// ※制約により本問題ではありえないが、制約がない場合にも解ける形になる
//	dist[s] = 0;
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
			ans = max(ans, a[e] - a[s]);  // スタート地点sで買い、町eで売る
		}
	}
	return;
}

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	a.resize(N);
	city.resize(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		city[i] = {a[i], i};  // {売買金額, 町番号}
	}
	sort(city.begin(), city.end());  // 売買金額でソート
	for(i = 0; i < M; i++)
	{
		int x, y; cin >> x >> y;
		x--; y--;
		g[x].push_back(y);  // x->y
	}

	vector<int> dist(N, INF32);
	// 売買金額が安い順(安値で買える順)に始点としてBFS
	for(auto &e : city)
	{
		int no = e.second;
		if(dist[no] != INF32) continue;
		bfs_graph(g, no, dist);
	}
	cout << ans << endl;

	return 0;
}
