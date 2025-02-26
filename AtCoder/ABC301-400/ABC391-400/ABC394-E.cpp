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

// ABC394 https://atcoder.jp/contests/abc394

/*
 * 自力で解けず、解説を見た。BFSで解く。
 * 基本は公式解説の通り。
 *   https://atcoder.jp/contests/abc394/editorial/12279
 * 
 * 距離0ないし1の2点から、両端に伸ばしていく。
 * dist[i][i]に0を、i->jへの辺があるdist[i][j]に1を初期値として設定＆キューに積んでおく。
 * キューから取り出した2頂点について、その前後に同じラベルの辺があれば、distを+2にしてさらにキューに積む。
 * 
 * 計算量は、状態数がO(N^2), 各状態からの遷移がO(N^2)なので、全体でO(N^4).
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・最初、「回文は前後から見ると上手くいくケースがある」という典型に固執し、TLEとなっていた。
 *   (この方針でも上手くやればいけたのかもしれないが…いや無理か？)
 * ・回文の中央から両端に伸ばすという方針も覚えておく。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<string> c(N);
	for(i = 0; i < N; i++) cin >> c[i];

	using pii = pair<int,int>;
	queue<pii> que;
	vector<vector<int>> dist(N, vector<int>(N, INF32));
	for(i = 0; i < N; i++)
	{
		dist[i][i] = 0;
		que.push({i, i});
	}
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)  // i->j
		{
			if(i == j) continue;
			if(c[i][j] != '-')
			{
				dist[i][j] = 1;
				que.push({i, j});
			}
		}
	}

	while(!que.empty())
	{
		auto [u, v] = que.front();
		que.pop();
		int pu, nv;  // pu->u, v->nv
		for(pu = 0; pu < N; pu++)
		{
			if(c[pu][u] == '-') continue;
			for(nv = 0; nv < N; nv++)
			{
				if(c[v][nv] == '-') continue;
				if(dist[pu][nv] != INF32) continue;
				if(c[pu][u] == c[v][nv])
				{
					dist[pu][nv] = dist[u][v] + 2;
					que.push({pu, nv});
				}
			}
		}
	}

	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			if(dist[i][j] != INF32)
			{
				cout << dist[i][j];
			}
			else
			{
				cout << -1;
			}
			if(j != N-1) cout << " ";
		}
		cout << endl;
	}

	return 0;
}
