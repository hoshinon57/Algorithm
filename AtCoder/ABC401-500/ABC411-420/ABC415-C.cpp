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

// ABC415 https://atcoder.jp/contests/abc415

/*
 * ちょっと問題文の理解に時間がかかったやつ。
 * 
 * 問題文にて定義された"状態"をグラフの頂点として考える。
 * 状態X->Yの遷移を考えると、bitが0->1へ1つだけ変化したものに限定される。
 * かつ状態Yが安全であるもののみ。
 * 
 * 以上を元にまずグラフを構築する。
 * これは状態数が2^Nで、各状態からの遷移数がN.
 * 
 * そうしたらBFSにて頂点0⇒頂点(2^N-1)への遷移が可能かを判定する。
 * 制約の |S|の総和 に上限があるので、十分間に合う。
**/

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる

using Graph = vector<vector<int>>;

// 頂点sを起点にBFS.
// distは未探索部分をINF32で埋めていること。
// 探索結果として、distに頂点sを起点(=0)とした距離を格納する。
void bfs_graph(Graph &G, int s, vector<int> &dist)
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
	return;
}

bool solve(void)
{
	ll i, j;
	ll N; cin >> N;
	string s; cin >> s;
	Graph g(1LL<<N);  // 2^N頂点
	
	// 状態stが安全か
	auto issafe = [&](ll st) -> bool
	{
		if(st == 0) return true;
		if(s[st-1] == '0') return true;
		return false;
	};

	for(i = 0; i < (1LL<<N); i++)  // 頂点i
	{
		if(!issafe(i)) continue;  // 念のため
		for(j = 0; j < N; j++)  // bit j
		{
			// bitを1つだけ0->1にするのを考える
			if(isbiton(i, j)) continue;
			ll to = i;
			setbit(to, j);
			if(issafe(to))  // 遷移先が安全
			{
				g[i].push_back(to);  // i->to
			}
		}
	}

	vector<int> dist(1LL<<N, INF32);
	bfs_graph(g, 0, dist);

	return (dist[(1<<N)-1] != INF32);
}

int main(void)
{
	int T; cin >> T;
	while(T > 0)
	{
		T--;
		YesNo(solve());
	}

	return 0;
}
