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

// ABC296 https://atcoder.jp/contests/abc296

/*
 * 問題文と入力をぐっとにらんでみると、
 *   i->Aiへ辺が張られたグラフにおいて、サイクル内の頂点の個数は？
 * を答えれば良いと分かる。
 *   https://x.com/kyopro_friends/status/1642161902506872832
 * 
 * 最初に頂点の入次数を最初に求めておいて、
 * 入次数が0のものを消し、それにより次の頂点も入次数が0になればまた消し…というのを
 * キューを用いて実装すればよい。
 * 
 * [ACまでの思考の流れ]
 * ・Functional Graphにおいて、サイクル上にある頂点は入次数を0のものから順に消していくことで求められる。
 *   Functional Graphでない、次数が2以上のグラフの場合は…強連結成分分解(SCC)かな？
 */

using Graph = vector<vector<int>>;

int main(void)
{
	int i;
	int N; cin >> N;
	Graph g(N);
	vector<int> in(N);  // 入次数
	for(i = 0; i < N; i++)  // i->Ai
	{
		int a; cin >> a;
		a--;
		g[i].push_back(a);
		in[a]++;
	}

	queue<int> que;
	for(i = 0; i < N; i++)
	{
		if(in[i] == 0) que.push(i);
	}

	int ans = N;
	while(!que.empty())
	{
		int v = que.front();
		que.pop();
		ans--;
		for(auto &e : g[v])  // 頂点vを消す
		{
			in[e]--;
			if(in[e] == 0)
			{
				que.push(e);
			}
		}
	}
	cout << ans << endl;

	return 0;
}
