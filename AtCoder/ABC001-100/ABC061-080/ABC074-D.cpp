#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題63より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc074

/*
 * ワーシャルフロイド法(的な手法)で解く。
 * 
 * まず入力データをAijを、都市i,j間を直結する道路の長さとする。
 * 以下、これをdist[i][j]とする。
 * 
 * 各i,j(ただし無向グラフなのでi<jとする)の組み合わせにおいて、
 *   (1)dist[i][j]  つまりi,j直結の長さ
 *   (2)dist[i][k]+dist[k][j]  つまり他都市kを経由した場合の長さ
 * を比較する。
 * 
 * (1)==(2)となるようなkが存在する場合、
 * 「i,j直結の道路は不要」とみなせる。
 * 長さの和の最小を求めるのが目的で、k経由とすればよいため。
 * 
 * 一方、(1)>(2)となるようなkが存在する場合、
 * これは「i,j直結よりも迂回した方が短いルートがある」ということであり、入力Aに矛盾する。
 * 条件を満たす道路が存在しないので、-1を出力して終了する。
 * 
 * 全てのkに対して(1)<(2)であれば、i,j直結の道路は必要であり、答に加算する。
 * 
 * 計算量はO(N^3).
 * 
 * [補足]
 * -1判定は、入力データに対してワーシャルフロイド法を適用し、
 * その結果を入力データと比較する方法でもよい。
 * 不一致の場合は、条件を満たす道路が存在しないと判断できる。
 */

int main(void)
{
	// 0-indexed
	int i, j, k;
	int N;
	cin >> N;
	vector<vector<int>> dist(N, vector<int>(N));
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			cin >> dist[i][j];
		}
	}

	// 都市i,j間の距離 dist[i][j] が、i,j直結の道路が必要なのか、他都市を経由しても良いのかを調べる。
	// dist[i][j] == dist[i][k]+dist[k][j] となるkがあれば、直結の道路は不要。
	ll answer = 0;
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)
		{
			bool need = true;  // i,j直結の道路が必要か
			for(k = 0; k < N; k++)
			{
				if(k == i || k == j) continue;
				if(dist[i][j] == dist[i][k] + dist[k][j])
				{
					// i,j直結の道路と、k経由の道路とで長さが同一となる。
					// 長さが最小のものを求めるので、この場合、直結の道路は不要にできる。
					need = false;
					break;
				}
				else if(dist[i][j] > dist[i][k] + dist[k][j])
				{
					// i->j直結よりも迂回した方が短いルートがある場合、そもそもの条件を満たさない。
					// -1を出力して終了。
					cout << -1 << endl;
					return 0;
				}
			}
			if(need) answer += dist[i][j];
		}
	}
	cout << answer << endl;

	return 0;
}
