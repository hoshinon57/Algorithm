#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題79より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc106

/*
 * 累積和で解く。
 * ABC106-D.cppと異なり、1次元の累積和。
 * 
 * 公式解説を参考にした。
 *   https://img.atcoder.jp/abc106/editorial.pdf
 * 
 * train[l][r]を、l～rを走る列車の数とする。
 * 例えばクエリがp=2, q=4のとき、
 * train[2][2], [2][3], [2][4], [3][2], [3][3], [3][4], [4][2], [4][3], [4][4]が該当する列車となる。
 * つまりtrain[p][p], [p][p+1], ..., [p][q], [p+1][p], ..., [q][q]の和。
 * ただしこれを二重ループで求めると、O(Q*N^2)となりTLE.
 * 
 * そこでtrain[p][q]のpについて固定し、qの部分について累積和を構築する。
 * するとクエリがp=2, q=4であればtrain_sum[2], sum[3], sum[4]の3つの累積和を使えば良い。
 * 
 * 計算量はO(N^2+QN).
 */

using vvi = vector<vector<int>>;

int main(void)
{
	int i, j;
	int N, M, Q;
	cin >> N >> M >> Q;
	vvi train(N+5, vector<int>(N+5));  // train[l][r]:l～rを走る列車の数
	vvi train_sum(N+5, vector<int>(N+5));  // train_sum[l][r]:train[l][*]について、[*]:[0]～[r-1]のr個の累積和
	for(i = 0; i < M; i++)
	{
		int l, r;
		cin >> l >> r;
		train[l][r]++;
	}
	// 累積和の構築
	for(i = 0; i <= N; i++)
	{
		for(j = 0; j <= N; j++)
		{
			train_sum[i][j+1] = train_sum[i][j] + train[i][j];
		}
	}

	for(i = 0; i < Q; i++)
	{
		int p, q;
		cin >> p >> q;
		// 例えばp=2, q=4のとき、
		// [l,r]=train[2][2], [2][3], [2][4], [3][2], [3][3], [3][4], [4][2], [4][3], [4][4]が該当する。
		// つまりtrain[p][p], [p][p+1], ..., [p][q], [p+1][p], ..., [q][q]の和。
		int sum = 0;
		for(j = p; j <= q; j++)  // 左端について固定し、train[j][p]～train[j][q]の和を求める
		{
			// train[j][*]について、[p,q]の和がほしい -> 半開区間の[p,q+1)で考えて、sum[q+1]-sum[p]となる
			sum += train_sum[j][q+1] - train_sum[j][p];
		}
		cout << sum << endl;
	}

	return 0;
}
