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

// ABC260 https://atcoder.jp/contests/abc260

/*
 * 自力で解けず、解説のHint4まで見た。
 * 鳩の巣原理を用いて解く。
 * 
 * 基本は各種解説の通り。
 *   https://atcoder.jp/contests/abc260/editorial/4437
 *   https://scrapbox.io/ganariya-competitive/AtCoderBeginnerContest260_F%E5%95%8F%E9%A1%8C500%E7%82%B9_%E3%80%8CFind_4-cycle%E3%80%8D
 *   https://geam1113.hatenablog.com/entry/2022/07/23/064220
 * 
 * S,Tが独立集合という条件から、4-cycleの条件は
 *   T側のある2頂点a,bについて、
 *   それがS側のある頂点と辺で結ばれている、というのが2つある
 * となる。
 * 
 * edge[a][b]を
 *   T側の2頂点a,bについて、ともに繋がるS側の頂点があれば、その番号
 * として定義する。
 * そしてSの頂点ごとに辺2つの組み合わせを全列挙し、
 *   edge[a][b]が登録済なら、条件を満たすS側の2頂点が見つかったので出力して終了。
 *   未登録なら頂点Sの番号を登録する。
 * とする。
 * これは(T^2+1)回目のループまでで必ず終了する。T=3000より、十分に間に合う。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・「条件を満たすものが2つ見つかればよい」「任意の2つでよい」といった場合、鳩の巣原理を考えてみる。
 *   本問ではS側の頂点で条件を満たすものが2つ見つかれば良い。
 */

using Graph = vector<vector<int>>;

int main(void)
{
	int i, j, k;
	int S, T, M; cin >> S >> T >> M;
	Graph g(S+T);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}

	// S->Tの辺を見ていく
	// edge[a][b]:T側の2頂点(a+S),(b+S)ともに繋がるS側の頂点があれば、その番号
	vector<vector<int>> edge(T, vector<int>(T, -1));
	for(i = 0; i < S; i++)  // 頂点i
	{
		int gs = (int)g[i].size();
		for(j = 0; j < gs; j++)
		{
			for(k = j+1; k < gs; k++)  // g[i][j]とg[i][k]
			{
				int u = g[i][j] - S;
				int v = g[i][k] - S;
				if(edge[u][v] != -1)
				{
					cout << i+1 << " " << edge[u][v]+1 << " " << u+S+1 << " " << v+S+1 << endl;
					return 0;
				}
				else
				{
					edge[u][v] = edge[v][u] = i;
				}
			}
		}
	}
	cout << -1 << endl;

	return 0;
}
