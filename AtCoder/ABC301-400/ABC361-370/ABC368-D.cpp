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

// ABC368 https://atcoder.jp/contests/abc368

/*
 * 入力のV[0]を根としてDFSを行う。
 * DFSにて各頂点vを部分木としたときに、指定された頂点の個数をボトムアップで求めていく。
 * これをcnt[v]とすると、cnt>0となる要素数が答。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・入力に対し、答となるグラフはどういう形になっているか？ をぐっとにらんで特徴をとらえる。
 */

using Graph = vector<vector<int>>;

int main(void)
{
	int i;
	int N, K; cin >> N >> K;
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	vector<int> V(K); for(i = 0; i < K; i++) {cin >> V[i]; V[i]--;}
	vector<bool> sel(N, false);  // sel[v]:頂点vが指定された頂点ならtrue
	for(i = 0; i < K; i++) sel[V[i]] = true;

	vector<int> cnt(N);  // cnt[v]:頂点vを部分木としたとき、指定された頂点数
	auto dfs = [&](auto &self, int v, int p = -1) -> int
	{
		if(sel[v]) cnt[v]++;
		for(auto &e : g[v])
		{
			if(e == p) continue;
			cnt[v] += self(self, e, v);
		}
		return cnt[v];
	};
	dfs(dfs, V[0]);

	int ans = 0;
	for(auto &e : cnt) if(e > 0) ans++;
	cout << ans << endl;

	return 0;
}
