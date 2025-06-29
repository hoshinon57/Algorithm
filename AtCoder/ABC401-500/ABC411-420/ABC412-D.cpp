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

// ABC412 https://atcoder.jp/contests/abc412

/*
 * コンテスト時に解けなかった、大反省。
 * 
 * 「すべての頂点の次数が2」ということは、"連結成分ごと"にサイクルになっているということ。
 * N頂点で1つのサイクルなだけでなく、2つ以上のサイクルになっていても良い。
 * ただしサイクルになるためには頂点数が3つ以上必要なことから(※)、
 * 制約のN<=8より、連結成分は2つまでである。
 * (※)頂点数が1つで次数が2だと、自己ループになってしまう。
 *   また頂点数が2つだと多重辺になってしまう。
 * 
 * 順列全探索でサイクル上の頂点の並び順を全て探索する。
 * サイクルが2つの場合も、順列にて「どこでいったん区切るか」を全探索すればよい。
 * 詳細は実装を参照。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・順列全探索はすぐ見えたが、サイクルが1つとは限らないというのがしばらく見えなかった。
 *   しかもそれに気づいた後、「サイクルは2つまで」が見えず、bitDPか？となってうまく実装できず終わってしまった。大反省。
 * ・単純無向グラフにて全ての頂点の次数が2ということは、"連結成分ごと"にサイクルになっていることと同値。
 * ・単純無向グラフにてサイクルを作るには、頂点が3つ以上必要。
**/

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	vector<vector<int>> ed(N, vector<int>(N, 0));
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		ed[a][b] = 1;
		ed[b][a] = 1;
	}
	vector<int> a(N);
	iota(a.begin(), a.end(), 0);

	int ans = INF32;
	do  // v(a[0]) <-> v(a[1]) <-> v(a[2]) <-> ... <-> v(a[N-1]) <-> v(a[0]) とつなぐ
	{
		// サイクル1つ
		{
			vector<vector<int>> mk(N, vector<int>(N, 0));
			for(i = 0; i < N; i++)
			{
				mk[a[i]][a[(i+1)%N]] = 1;
				mk[a[(i+1)%N]][a[i]] = 1;
			}
			int cnt = 0;
			for(i = 0; i < N; i++)
			{
				for(j = 0; j < N; j++)
				{
					if(ed[i][j] != mk[i][j]) cnt++;
				}
			}
			cnt /= 2;
			chmin(ans, cnt);
		}

		// サイクル2つ
		{
			// [0,bd) [bd,N) に分ける
			// サイクルにできるのは頂点3つ以上
			for(int bd = 3; bd <= N-3; bd++)
			{
				vector<vector<int>> mk(N, vector<int>(N, 0));
				for(i = 0; i < bd; i++)
				{
					mk[a[i]][a[(i+1)%bd]] = 1;
					mk[a[(i+1)%bd]][a[i]] = 1;
				}
				for(i = bd; i < N; i++)
				{
					int u = i;
					int v = u+1;
					if(v == N) v = bd;
					mk[a[u]][a[v]] = mk[a[v]][a[u]] = 1;
				}
				int cnt = 0;
				for(i = 0; i < N; i++)
				{
					for(j = 0; j < N; j++)
					{
						if(ed[i][j] != mk[i][j]) cnt++;
					}
				}
				cnt /= 2;
				chmin(ans, cnt);
			}
		}
	} while (next_permutation(a.begin(), a.end()));
	cout << ans << endl;

	return 0;
}
