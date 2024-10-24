#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
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
 * 公式解説の方針。
 *   https://atcoder.jp/contests/abc368/editorial/10730
 * 
 * 元のグラフから
 *   指定された頂点Vxではなく、
 *   次数が1である
 * 頂点を削除していったものが答となる。
 * 
 * 削除候補の頂点をキューに積みながら、キューから取り出した頂点vを
 *   削除し、
 *   vに隣接する頂点eの次数を1減らし、頂点eが削除可能ならキューに積む
 * という操作をキューが空になるまで処理すればよい。
 */

using Graph = vector<vector<int>>;

int main(void)
{
	int i;
	int N, K; cin >> N >> K;
	Graph g(N);
	vector<int> deg(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
		deg[a]++; deg[b]++;
	}
	vector<bool> sel(N, false);
	for(i = 0; i < K; i++)
	{
		int v; cin >> v;
		v--;
		sel[v] = true;
	}
	vector<bool> del(N, false);

	// 頂点vが削除可能ならtrue (次数が1かつ指定されていない、が条件)
	auto check = [&](int v) -> bool
	{
		if(deg[v] == 1 && !sel[v]) return true;
		else return false;
	};
	queue<int> que;
	for(i = 0; i < N; i++)
	{
		if(check(i)) que.push(i);
	}
	
	int ans = N;
	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		del[v] = true;
		ans--;  // 頂点vを削除したので答から1引く
		for(auto &e : g[v])
		{
			if(del[e]) continue; // 削除済み
			deg[e]--;
			if(check(e)) que.push(e);
		}
	}
	cout << ans << endl;

	return 0;
}
