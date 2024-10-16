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

// ABC375 https://atcoder.jp/contests/abc375

/*
 * コンテスト時、実装方針は分かったが解き切れなかった。
 * クエリ先読みとワーシャルフロイド法の気持ちで解く。
 * 
 * [典型]辺の削除は難しいので、クエリを逆方向に見る。
 * 
 * 基本は公式解説の通り。
 *   https://atcoder.jp/contests/abc375/editorial/11134
 * クエリを逆順に見ていって、道路の通行止めを道路追加に読み替えることができる。
 * 以下の手順で解ける。
 * ・まず、クエリを正順に最後まで見て、残っている道路についてdistを設定していく。
 * ・全頂点について一度ワーシャルフロイドを適用する。
 * ・クエリを逆読みしていって、
 *   (1)道路通行止めクエリ＝道路追加クエリ
 *     対象の道路となるdistが小さい方向に更新できるならば、更新する
 *     対象の道路の両端についてワーシャルフロイドを適用する。
 *   (2)質問クエリ
 *     現在のdistをansに設定
 * ・ansをreverseすると正順になるので、それを出力。
 * 
 * コンテスト時、"道路追加クエリ"にて無条件でdistを更新しており、ACできなかった。
 * 道路が復活したとしても、既存のルートの方が短ければ更新しないのが正しい。
 */

int main(void)
{
	// 0-indexed
	int i, k;
	ll N, M, Q; cin >> N >> M >> Q;
	vector<ll> a(M), b(M), c(M); for(i = 0; i < M; i++) {cin >> a[i] >> b[i] >> c[i]; a[i]--; b[i]--;}
	vector<vector<ll>> que(Q, vector<ll>(3, 0));  // que[Q][3]
	vector<bool> can(M, true);  // can[i]:道路iが使えるならtrue
	for(i = 0; i < Q; i++)  // クエリ先読み
	{
		int ki; cin >> ki;
		if(ki == 1)
		{
			cin >> que[i][1];
			que[i][1]--;
			can[que[i][1]] = false;
		}
		else
		{
			cin >> que[i][1] >> que[i][2];
			que[i][1]--;
			que[i][2]--;
		}
		que[i][0] = ki;
	}

	vector<vector<ll>> dist(N, vector<ll>(N, INF64));  // dist[N][N]
	for(i = 0; i < N; i++) dist[i][i] = 0;
	// まず、クエリを正順に最後まで見て、残っている道路についてdistを設定
	for(i = 0; i < M; i++)
	{
		if(can[i])
		{
			dist[a[i]][b[i]] = dist[b[i]][a[i]] = c[i];
		}
	}

	// 頂点noについてワーシャルフロイド
	auto wf = [&](int no) -> void
	{
		int ii, jj;
		for(ii = 0; ii < N; ii++)
		{
			for(jj = 0; jj < N; jj++)
			{
				if(dist[ii][no] != INF64 && dist[no][jj] != INF64)  // ii-no, no-jj間がどちらも通れる場合
				{
					dist[ii][jj] = min(dist[ii][jj], dist[ii][no]+dist[no][jj]);
				}
			}
		}
	};

	// まず全頂点についてワーシャルフロイド
	for(k = 0; k < N; k++) wf(k);

	// クエリ逆読み
	reverse(que.begin(), que.end());
	vector<ll> ans;
	for(i = 0; i < Q; i++)
	{
		if(que[i][0] == 1)  // 道路が復活、の扱い
		{
			int d = que[i][1];  // 道路d
			chmin(dist[a[d]][b[d]], c[d]);
			chmin(dist[b[d]][a[d]], c[d]);
			wf(a[d]);
			wf(b[d]);
		}
		else
		{
			ans.push_back(dist[que[i][1]][que[i][2]]);
		}
	}

	reverse(ans.begin(), ans.end());
	for(auto &e : ans)
	{
		if(e == INF64) cout << -1 << endl;
		else cout << e << endl;
	}

	return 0;
}
