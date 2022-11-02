#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題57
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2008yo/tasks/joi2008yo_f

/*
 * ワーシャルフロイド法で解いてみた。
 * 
 * 新たに運行を開始する船舶の入力ごとに、
 * ワーシャルフロイド法を用いて全島間の最小運賃を計算しなおす。
 * 
 * ※最初は、ワーシャルフロイド法にて入力c,dのみ中継地点として更新すればよいと考えたが、
 *   更新のforループは0から行うことより、c,dの値によっては正しく更新できないケースが存在する。
 *   よって、毎度3重ループによる更新が必要。
 * 
 * 計算量はO(k*n^3)となる。
 * (1回あたりのワーシャルフロイド法にO(n^3), 入力が最大でk回)
 * 最大でn=100, k=5000のため10^9以上となるが、本問題のテストケースでは1秒程度で通せた。
 * ただし、入力の大半を「新たに運行を開始する船舶」にしてみたところ、1分以上かかった。
 */

int main(void)
{
	// 0-indexed
	int i, j, h;
	int n, k;
	cin >> n >> k;
	vector<vector<ll>> cost(n, vector<ll>(n, INF64));  // cost[i][j]:島i,j間の最安料金
	for(i = 0; i < n; i++) cost[i][i] = 0;

	for(; k > 0; k--)
	{
		int in;
		cin >> in;
		if(in == 0)
		{
			int a, b;
			cin >> a >> b;
			a--;  // 0-indexedに変換
			b--;
			if(cost[a][b] != INF64)
				cout << cost[a][b] << endl;
			else
				cout << -1 << endl;
		}
		else  // in == 1
		{
			int c, d;
			ll e;
			cin >> c >> d >> e;
			c--;  // 0-indexedに変換
			d--;

			// 既により安い運賃で旅行できている可能性があるため、チェック必要
			cost[c][d] = min(cost[c][d], e);
			cost[d][c] = min(cost[d][c], e);

			// ワーシャルフロイド法で全更新
			for(h = 0; h < n; h++)
			{
				for(i = 0; i < n; i++)
				{
					for(j = 0; j < n; j++)
					{
						if(cost[i][h] == INF64 || cost[h][j] == INF64) continue;
						cost[i][j] = min(cost[i][j], cost[i][h] + cost[h][j]);
					}
				}
			}
		}
	}

	return 0;
}
