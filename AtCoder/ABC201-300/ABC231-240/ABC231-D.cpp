#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;
using Graph = vector<vector<int>>;

// ABC231 https://atcoder.jp/contests/abc231

/*
 * グラフと幅優先探索(BFS)で解く。
 * 
 * 人を頂点、隣り合っていることを辺とする。
 * 横一列のため、まず辺が3つ以上の頂点があればNGとなる。
 * 
 * 同じく横一列のため、グラフにサイクルが存在するかどうかを判別するために
 * 各頂点を起点とした幅優先探索を行う。
 * 探索済みの頂点が出てきたらループ発生とみなせる。
 * ※このとき、キューには各頂点の親を登録しておくことで、逆流を防ぐ。
 */
int main(void)
{
	// 0-indexedに変換して解く
	int i;
	int N, M;
	cin >> N >> M;
	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int a, b;
		cin >> a >> b;
		a--;  // 0-indexedに変換
		b--;
		graph[a].push_back(b);
		graph[b].push_back(a);
	}

	// 辺が3つ以上の頂点があればNG(横一列のため)
	for(i = 0; i < N; i++)
	{
		if(graph[i].size() >= 3)
		{
			cout << "No" << endl;
			return 0;
		}
	}

	vector<bool> seen(N, false);
	// 頂点iを起点とした幅優先探索を行う
	for(i = 0; i < N; i++)
	{
		if(seen[i]) continue;  // 起点が探索済みならスルー
		queue<pair<int,int>> que;  // {探索する頂点, その親}  第2引数が-1ならば、親無し
		seen[i] = true;
		que.push(make_pair(i,-1));  // 頂点iを起点とするので、第2引数は-1(親無し)
		while(!que.empty())
		{
			// vについて探索
			pair<int,int> v;
			v = que.front();
			que.pop();
			for(auto &e : graph[v.first])  // 探索している頂点と繋がっている頂点
			{
				if(e == v.second) continue;  // eがvの親ならスルー
				if(seen[e])
				{
					// ループ発生
					cout << "No" << endl;
					return 0;
				}
				seen[e] = true;
				que.push(make_pair(e, v.first));
			}
		}
	}

	cout << "Yes" << endl;

	return 0;
}
