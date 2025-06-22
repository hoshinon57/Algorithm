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

// ABC401 https://atcoder.jp/contests/abc401

/*
 * コンテスト時に解き切れなかった。
 * 木の直径、累積和、二分探索などを用いて解く。
 * 
 * 基本的には公式解説の通り。
 *   https://atcoder.jp/contests/abc401/editorial/12686
 * 
 * 木1の頂点ごとにO(1)とかO(log)で解きたい、となる。
 * 木1,2間に辺を1つ追加したときに直径はどうなるかを考えると、
 * (1)木1の直径のまま
 * (2)木2の直径のまま
 * (3)(v1から木1の最遠点)+(v2から木2の最遠点)+1
 * の3パターンであり、このうちの最大値となる。
 * (1)(2)は固定値であり、以降 mx = max((1),(2)) とする。
 * 
 * (3)について、まず木1,2ともに全頂点におけるこの値を事前計算する。これは
 *   木において、各頂点から最も遠い頂点は、木の直径の両端のいずれかになる。
 * となる。[典型]
 * これをd1[v1],d2[v2]とする。
 * また木2の方はソート＆累積和を計算しておく。
 * 
 * 木1の頂点v1について、
 *   d1[v1]+d2[v2]+1 と mx
 * の大小関係を考える。式変形して
 *   d2[v2] < mx-d1[v1]-1
 * となるv2については、mxが直径になる。
 * >= となるv2については、この個数をXとすると
 *   (d1[v1]+1)*X
 *   Σd2[k]  (k:上記を満たすようなv2の集合)
 * の和が総和となる。後者のΣはd2をソート＆累積和を事前計算で可能。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・二重シグマは愚直計算は到底間に合わないので、高速化を考える。
 * ・木1の各頂点ごとにまとめて木2側の計算をしたい気持ちになる。
 *   辺を結んだときにどこが直径になるかを考える。
**/

// ソート済のvector<T>a に対して、(未満,以下,より大きい,以上)となるidxと要素数を返す
// {val未満の最右のidx, val未満の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_miman(vector<T> &a, T val) {ll idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}
// {val以上の最左のidx, val以上の要素数} 要素が無ければidx=N
template<typename T> pair<ll,ll> bi_idxnum_ijou(vector<T> &a, T val)  {ll idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx,(ll)a.size()-idx};}

using Graph = vector<vector<int>>;

int mostfar(Graph &G, int s, vector<int> &dist)
{
	queue<int> que;
	dist[s] = 0;
	que.push(s);

	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		for(auto &e : G[v])
		{
			if(dist[e] != INF32) continue;
			dist[e] = dist[v] + 1;
			que.push(e);
		}
	}
	int idx = 0, di = -1;
	for(int i = 0; i < (int)dist.size(); i++) {
		if(chmax(di, dist[i])) idx = i;
	}
	return idx;
}

int main(void)
{
	int i;
	int N1; cin >> N1;
	Graph g1(N1);
	for(i = 0; i < N1-1; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g1[u].push_back(v);
		g1[v].push_back(u);
	}
	int N2; cin >> N2;
	Graph g2(N2);
	for(i = 0; i < N2-1; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g2[u].push_back(v);
		g2[v].push_back(u);
	}

	vector<ll> d1(N1);  // d1[v]: 頂点vから最遠点までの距離
	int dia1;
	{
		vector<int> dist1(N1, INF32), dist2(N1, INF32);
		int s = mostfar(g1, 0, dist1);
		dist1.assign(N1, INF32);
		int t = mostfar(g1, s, dist1);  // s-tが直径
		dia1 = dist1[t];
		mostfar(g1, t, dist2);
		for(i = 0; i < N1; i++) d1[i] = max(dist1[i], dist2[i]);
	}
	vector<ll> d2(N2);
	int dia2;
	{
		vector<int> dist1(N2, INF32), dist2(N2, INF32);
		int s = mostfar(g2, 0, dist1);
		dist1.assign(N2, INF32);
		int t = mostfar(g2, s, dist1);  // s-tが直径
		dia2 = dist1[t];
		mostfar(g2, t, dist2);
		for(i = 0; i < N2; i++) d2[i] = max(dist1[i], dist2[i]);
	}
	sort(d2.begin(), d2.end());
	vector<ll> d2sum(d2.size()+1); for(int kk = 0; kk < (int)d2.size(); kk++) {d2sum[kk+1] = d2sum[kk] + d2[kk];}

	ll ans = 0;
	ll mx = max(dia1, dia2);
	for(i = 0; i < N1; i++)
	{
		// mx-d1[i]-1 未満
		ll brd = mx-d1[i]-1;
		{
			auto [idx,num] = bi_idxnum_miman(d2, brd);
			ans += num*mx;
		}
		// mx-d1[i]-1 以上
		{
			auto [idx,num] = bi_idxnum_ijou(d2, brd);
			ans += d2sum[N2]-d2sum[idx];  // [idx,N) のd2[]総和
			ans += (d1[i]+1) * num;
		}
	}
	cout << ans << endl;

	return 0;
}
