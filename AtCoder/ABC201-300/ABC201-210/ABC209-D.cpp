#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC209 https://atcoder.jp/contests/abc209

/*
 * キーワード：木は二分グラフ
 *   https://twitter.com/kyopro_friends/status/1413856854132682761
 * 
 * 頂点を赤青の二分グラフで塗り分けるようにしたときに(同じ色の頂点は隣接しない)、
 * 移動ごとに赤青が入れ替わる。
 * よって2人のスタート地点が同じ色ならTown, 違う色ならRoadとなる。
 * ⇒各頂点について根からの距離を求めておき、その偶奇を使えば簡単。
 * 
 * 自分の実装は公式解説とほぼ同一だが、
 *   https://atcoder.jp/contests/abc209/editorial/2229
 * 解いたときの思考は
 * ・LCAではなく根まで戻るようにしても同じっぽいなあ。
 * ・根までの距離の偶奇が同じならTown, ちがうならRoadっぽいなあ。
 * というのをぼんやり考えて解いたのが実態。
 */

using Graph = vector<vector<int>>;
Graph g;
vector<int> dist;

void dfs(int v, int d = 0, int p = -1)
{
	dist[v] = d;
	for(auto &e : g[v])
	{
		if(e == p) continue;
		dfs(e, d+1, v);
	}
}

int main(void)
{
	int i;
	int N, Q; cin >> N >> Q;
	g.resize(N);
	dist.resize(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	dfs(0);

	while(Q > 0)
	{
		int c, d; cin >> c >> d;
		c--; d--;
		string ans = "Town";
		if(dist[c]%2 != dist[d]%2) ans = "Road";
		cout << ans << endl;
		Q--;
	}

	return 0;
}
