#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1000000007;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題76
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/nikkei2019-final/tasks/nikkei2019_final_a

/*
 * 公式解説を参考にした実装。
 * https://img.atcoder.jp/nikkei2019-final/editorial.pdf
 * 尺取り法みたいな感じ？
 * 
 * k_sum[i]:k=iにおける答(総和の最大値) と定義する。
 * 
 * iを0～N-1の間で1つ固定し、
 * それに対してjをi～N-1まで1つずつ動かしながら、総和を計算していく。
 * すると各(i,j)について、k_sum[j-i+1]の値を上記の総和で更新していけばよい。
 * 
 * 計算量はO(N^2).
 */

int main(void)
{
	// 0-indexed
	int i, j;
	int N;
	cin >> N;
	vector<ll> a(N);
	for(i = 0; i < N; i++) cin >> a[i];
	vector<ll> k_sum(N+1, 0);  // k_sum[i]:k=iにおける答(総和の最大値)

	for(i = 0; i < N; i++)
	{
		ll sum = 0;
		for(j = i; j < N; j++)
		{
			// 区間[l,r]の総和を求める
			// [l,r]の区間の長さはr-l+1.
			sum += a[j];
			k_sum[j-i+1] = max(k_sum[j-i+1], sum);  // 最大値を更新
		}
	}

	for(i = 1; i <= N; i++)
	{
		cout << k_sum[i] << endl;
	}

	return 0;
}
