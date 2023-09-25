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

// ABC309 https://atcoder.jp/contests/abc309

/*
 * 深さ優先探索で解く。
 * 
 * child[i][]：人iの子リスト
 * hoken[i]：人iは何代先までの保険に入っているか
 * と定義する。
 * 人0を頂点とした木構造になるため、0を起点にDFSする。
 * DFS完了後、hoken[*]>=0となる要素数が答。
 */

vector<vector<int>> child;
vector<int> hoken;

void dfs(int v)
{
	for(auto &e : child[v])
	{
		hoken[e] = max(hoken[e], hoken[v]-1);
		dfs(e);
	}
}

int main(void)
{
	// 0-indexed
	int i;
	int N, M; cin >> N >> M;
	child.resize(N);
	hoken.resize(N, -1);
	for(i = 1; i < N; i++)
	{
		int p; cin >> p;  // p->i
		p--;
		child[p].push_back(i);
	}
	for(i = 0; i < M; i++)
	{
		int x, y; cin >> x >> y;
		x--;
		hoken[x] = max(hoken[x], y);
	}

	dfs(0);

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		if(hoken[i] >= 0) ans++;
	}
	cout << ans << endl;

	return 0;
}
