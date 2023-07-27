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

// ABC238 https://atcoder.jp/contests/abc238

/*
 * 幅優先探索で解く。
 * 考え方はABC238-E.cppと同様であり、省略する。
 */

using Graph = vector<vector<int>>;

bool solve(void)
{
	// 0-indexed
	int i;
	int N, Q;
	cin >> N >> Q;
	Graph g(N+1);  // N+1頂点で考える
	for(i = 0; i < Q; i++)
	{
		int l, r;
		cin >> l >> r;
		l--;  r--;  // 0-indexedへの変換
		r++;  // 半開区間への変換
		// [l,r)
		g[l].push_back(r);
		g[r].push_back(l);
	}

	// 頂点0からBFS
	queue<int> que;
	vector<bool> seen(N+1, false);
	que.push(0);
	seen[0] = true;
	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		for(auto &e : g[v])
		{
			if(seen[e]) continue;
			seen[e] = true;
			que.push(e);
		}
	}

	// 頂点Nにたどり着けるか
	return seen[N];
}

int main(void)
{
	YesNo(solve());

	return 0;
}
