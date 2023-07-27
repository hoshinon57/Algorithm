#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC264 https://atcoder.jp/contests/abc264

/*
 * 最初はUnion-FindではなくBFSで解く手法を思いついた。
 * ★ただし当初実装したBFSだと、TLEになるケースが1つだけあり、最後の最後まで回避できなかった。
 *   bfs()の#if 1にて#else側のコードを直すと、改善できた。
 * 
 * Union-FindではなくBFSを使うという点以外は、AB264-E-2.cppとほぼ同じ。
 * 電線を新たにつなぐ際に、両地点のうちどちらか片方だけ電気が繋がっている場合に
 * 新たに電気が通るようになる都市が発生する。
 * これをBFSで解いていく。
 */

using Graph = vector<vector<int>>;

vector<bool> power;  // power[i]:地点iが発電所か、電気が通っている都市ならtrue
int power_city_num = 0;  // 電気が通っている都市の総数

vector<int> u, v;

// graphNoで指定した電線を接続し、
// それによって新たに電気が繋がる都市を幅優先探索していく。
void bfs(int graphNo, Graph &graph, queue<int> &que)
{
	graph[u[graphNo]].push_back(v[graphNo]);
	graph[v[graphNo]].push_back(u[graphNo]);

	// 地点(u,v)を接続するにあたり、片方だけに電気が通っている場合、もう片方に新たに電気が通ることになる。
	// このとき、電気が繋がっていなかった側をキューに積んでBFSする。
	if(!power[u[graphNo]] && power[v[graphNo]])
	{
#if 1
		power[u[graphNo]] = true;
		power_city_num++;
		que.push(u[graphNo]);
#else
		// ★すでに電気が通っているv側をキューに積む方法だと、TLEになるケースが1つだけあった。
		//   TLEを回避するには、u側のpower[], power_city_numを丁寧に更新して、u側をキューに積む必要がある。
		//   else if側も同様。
		que.push(v[graphNo]);
#endif
	}
	else if(power[u[graphNo]] && !power[v[graphNo]])
	{
		power[v[graphNo]] = true;
		power_city_num++;
		que.push(v[graphNo]);
	}
	// 地点(u,v)がともに電気が繋がっている、またはともに電気が繋がっていないときは、
	// 新たに電気が繋がる都市は無い。
	else return;

	// BFS
	while(!que.empty())
	{
		int vertex = que.front();
		que.pop();

		for(auto &e : graph[vertex])
		{
			if(power[e]) continue;  // すでに電気が通っているならスルー(ロジック上、その先も全て通っているので)
			power[e] = true;
			power_city_num++;
			que.push(e);
		}
	}
}

int main(void)
{
	// 0-indexed
	int i;
	int N, M, E;
	cin >> N >> M >> E;
	Graph graph(N+M);
	u.resize(E);
	v.resize(E);
	for(i = 0; i < E; i++)
	{
		cin >> u[i] >> v[i];
		u[i]--;
		v[i]--;
	}
	int Q;
	cin >> Q;
	vector<int> x(Q);
	vector<bool> event_list(E, false);  // イベントにて切れる電線ならtrue
	for(i = 0; i < Q; i++)
	{
		cin >> x[i];
		x[i]--;
		event_list[x[i]] = true;  // イベントで切れる電線
	}
	// ここまで入力

	// 地点iに電気が通っているかを示すpower[]の初期化
	power.assign(N+M, false);
	for(i = N; i < N+M; i++)  // 発電所はtrueにしておく
	{
		power[i] = true;
	}

	queue<int> que;
	for(i = 0; i < E; i++)  // 最後まで残る電線は、ここでグラフ構築
	{
		if(event_list[i]) continue;  // イベントで切れる電線は、ここではスルー
		bfs(i, graph, que);
	}

	vector<int> answer;
	for(i = Q-1; i >= 0; i--)  // イベントを逆方向に見ていく
	{
		answer.push_back(power_city_num);  // 逆方向に見ていくことから、BFSの前に記録しておく
		bfs(x[i], graph, que);
	}

	reverse(answer.begin(), answer.end());
	for(auto &e : answer)
	{
		cout << e << endl;
	}

	return 0;
}
