#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC226 https://atcoder.jp/contests/abc226

/*
 * DFSで解く。
 * 
 * 各技から、その技を習得するために必要な技へと、有向辺を張ったグラフを考える。
 * 技Nを起点にグラフを探索し、通った頂点（に対応する技）を全て覚える必要があるので、
 * DFSにて探索して通った頂点をリストアップすればよい。
 */

using Graph = vector<vector<int>>;
Graph G;
vector<bool> must;  // must[i]:技Nの習得には、技iの習得が必要かどうか

// 技vを起点に、技vの習得に必要な技をDFSで探索
void dfs(int v)
{
	if(must[v]) return;
	must[v] = true;

	for(auto &e : G[v])
	{
		dfs(e);
	}
}

int main(void)
{
	// 0-indexed
	int i;
	int N;
	cin >> N;
	vector<int> T(N);
	G.resize(N);
	must.resize(N, false);
	for(i = 0; i < N; i++)
	{
		cin >> T[i];
		int k;
		cin >> k;
		for(int j = 0; j < k; j++)
		{
			int a;
			cin >> a;
			a--;
			G[i].push_back(a);
		}
	}

	// 技Nを起点に、必要な技をDFSで探索
	dfs(N-1);

	ll answer = 0;
	for(i = 0; i < N; i++)
	{
		if(must[i]) answer += T[i];
	}
	cout << answer << endl;

	return 0;
}
