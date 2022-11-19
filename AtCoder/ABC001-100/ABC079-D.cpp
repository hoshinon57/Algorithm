#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC079 https://atcoder.jp/contests/abc079

/*
 * ワーシャルフロイド法で解く。
 * 
 * dist[i][j]をi->jへの最小魔力、とする。
 * まず入力c(i,j)の通りにdist[]を初期化する。
 * これはグラフにおいて頂点i,jを直接つないだ場合の距離とみなせる。
 * 
 * 次にワーシャルフロイド法で距離を更新する。
 * これは頂点i,jについて他頂点を経由する場合も含めた最短距離を求めることになる。
 * 
 * 最後にA(i,j)の入力aに対して、頂点a->1への最短距離を加算していく。
 * これはdist[a][1]を使えば良い。
 * 
 * 計算量は、N=10としてワーシャルフロイド法の部分にO(N^3),
 * 入力A(i,j)に対する処理にO(HW)となるので、合わせてO(N^3+HW)になるのかな…と思う。
 */
int main(void)
{
	int i, j, k;
	int H, W;
	cin >> H >> W;
	vector<vector<int>> dist(10, vector<int>(10));  // dist[i][j]:i->jへの最小魔力
	for(i = 0; i <= 9; i++)
	{
		for(j = 0; j <= 9; j++)
		{
			cin >> dist[i][j];
		}
	}

	// ワーシャルフロイド法で最短距離を求める
	// 今回はどの頂点どうしも接続されていると考えることができるので、INFを用いた判定は不要。
	for(k = 0; k <= 9; k++)
	{
		for(i = 0; i <= 9; i++)
		{
			for(j = 0; j <= 9; j++)
			{
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}

	ll answer = 0;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			// 数値a->1への最短距離を加算する
			int a;
			cin >> a;
			if(a == -1) continue;
			answer += dist[a][1];
		}
	}
	cout << answer << endl;

	return 0;
}
