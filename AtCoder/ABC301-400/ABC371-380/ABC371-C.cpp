#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC371 https://atcoder.jp/contests/abc371

/*
 * 順列全探索で解く。
 * 
 * グラフG,Hが同型ということは、Hの頂点番号を組み替えるとGと一致する、ということ。
 * N=8なので、組み替え方を N! の全通り試しても十分に間に合う。
 * 
 * g[i][j]を、グラフGにて頂点i,j間に辺があればtrueと定義する。
 * h[i][j]も同様に。
 * 
 * グラフHの頂点(i,j)と、組み替えて対応するGの頂点(ii,jj)について、
 * i-j間とii-jj間の辺の有無が不一致であれば、辺を追加するか削除していく。
 * 組み替えパターンごとのコストを求め、その最小値が答。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<vector<bool>> g(N, vector<bool>(N, false));
	vector<vector<bool>> h(N, vector<bool>(N, false));
	int mg; cin >> mg;
	for(i = 0; i < mg; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u][v] = g[v][u] = true;
	}
	int mh; cin >> mh;
	for(i = 0; i < mh; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		h[u][v] = h[v][u] = true;
	}
	vector<vector<int>> a(N, vector<int>(N, 0));  // a[N][N]
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)
		{
			cin >> a[i][j];
			a[j][i] = a[i][j];  // 逆方向も設定しておく
		}
	}

	vector<int> idx(N);  // idx[i]:Hの頂点iをGの頂点idx[i]に割り当てる
	iota(idx.begin(), idx.end(), 0);
	ll ans = INF64;
	do
	{
		ll sum = 0;
		for(i = 0; i < N; i++)
		{
			for(j = i+1; j < N; j++)
			{
				int ii = idx[i], jj = idx[j];  // G[ii][jj], H[i][j]
				if(g[ii][jj] != h[i][j])
				{
					sum += a[i][j];
				}
			}
		}
		chmin(ans, sum);
	} while (next_permutation(idx.begin(), idx.end()));
	cout << ans << endl;
	return 0;
}
