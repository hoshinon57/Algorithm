#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC396 https://atcoder.jp/contests/abc396

/*
 * bitごとに独立して解く。以下解説と基本的に同じ。
 *   https://x.com/kyopro_friends/status/1898368645711233066
 *   https://atcoder.jp/contests/abc396/editorial/12390
 * 
 * bitごとに考える。以下、着目するbitをbとする。
 * 頂点X,Yの間に重み付きの辺を張る。
 * 辺の重みはZのb bit目の値とする。つまり0 or 1.
 * 
 * 適当な頂点から探索したときに、辺の重みをxorしていって矛盾が起きないかをチェックする。
 * 矛盾が起きれば-1を出力して終了。
 * 矛盾が無ければ、各頂点のdistは0 or 1になっている。総和最小を求めたいので、
 *   0の個数の方が多い：distが0の頂点を0にする
 *   1の個数の方が多い：distが1の頂点を0にする
 * とすればよい。
 * 
 * 作ったグラフが連結とは限らない点に注意。
 * 連結成分ごとにチェックしていく必要がある。
**/

// 以下、bは0-indexed
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;

int main(void)
{
	ll i, b;
	ll N, M; cin >> N >> M;
	vector<ll> x(M), y(M), z(M); for(i = 0; i < M; i++) {cin >> x[i] >> y[i] >> z[i]; x[i]--; y[i]--;}

	vector<ll> ans(N);
	for(b = 0; b < 31; b++)  // bit bについて考える
	{
		Graph g(N);
		for(i = 0; i < M; i++)
		{
			ll w = ((z[i]>>b)&1);
			g[x[i]].push_back({(int)y[i], w});
			g[y[i]].push_back({(int)x[i], w});
		}
		vector<ll> dist(N, -1);
		vector<int> li;

		ll cnt0, cnt1;
		auto dfs = [&](auto &self, int v, ll di) -> void
		{
			if(dist[v] != -1)
			{
				if(dist[v] != di)
				{
					cout << -1 << endl;
					exit(0);
				}
				return;
			}
			else dist[v] = di;
			if(dist[v] == 0) cnt0++;
			else cnt1++;
			li.push_back(v);

			for(auto &e : g[v])  // e:辺
			{
				ll nd = di^e.weight;
				self(self, e.to, nd);
			}
		};
		// 連結とは限らないので注意 連結成分ごとにチェック必要
		for(i = 0; i < N; i++)
		{
			if(dist[i] != -1) continue;
			li.clear();
			cnt0 = 0;
			cnt1 = 0;
			dfs(dfs, i, 0);
			if(cnt0 >= cnt1)  // 0を0,1を1へ
			{
				for(auto &e : li)
				{
					if(dist[e] == 1) setbit(ans[e], b);
				}
			}
			else
			{
				for(auto &e : li)
				{
					if(dist[e] == 0) setbit(ans[e], b);
				}
			}
		}
	}

	int sz_ = (int)ans.size();
	// cout << sz_ << endl;
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}
