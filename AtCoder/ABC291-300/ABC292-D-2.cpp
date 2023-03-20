#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC292 https://atcoder.jp/contests/abc292

/*
 * BFSで解く。
 * 
 * 未探索の頂点を起点にBFSし、その際たどった頂点と辺の数をカウントしていく。
 * BFS終了後、頂点と辺の数が一致するかどうかを判定する。
 * ただし辺は両方向でカウントされるため、/2して計算する。
 */

using Graph = vector<vector<int>>;

int main(void)
{
	// 0-indexed
	int i;
	int N, M;
	cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v;
		cin >> u >> v;
		u--;
		v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}

	auto check = [&](void) -> bool
	{
		vector<bool> seen(N, false);
		queue<int> que;
		int v_count, e_count;  // それぞれ頂点の数、辺の数

		for(i = 0; i < N; i++)
		{
			if(seen[i]) continue;  // 探索済み
			seen[i] = true;
			que.push(i);
			v_count = 0;
			e_count = 0;

			while(!que.empty())
			{
				int v = que.front();
				que.pop();
				v_count++;
				for(auto &e : g[v])
				{
					e_count++;
					if(!seen[e])
					{
						seen[e] = true;
						que.push(e);
					}
				}
			}
			// 辺は両方向でカウントされるので/2して計算
			if(v_count != e_count/2) return false;
		}

		return true;
	};

	YesNo(check());

	return 0;
}
