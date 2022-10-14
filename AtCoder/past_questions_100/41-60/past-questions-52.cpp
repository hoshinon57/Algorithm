#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題52
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2017yo/tasks/joi2017yo_d

/*
 * bitDPと累積和で解く。
 * bitDPの基本的な実装は past-questions-49.cpp も参照のこと。
 * 
 * dp[S]を以下のように定義する。
 *   Sの各ビットをそれぞれのぬいぐるみに割り当てて、
 *   1になっているビットに対して好きな順に左から並べようとしたときに(種類ごとに連続して置く)、
 *   交換するために取り出したぬいぐるみの最小数と、そこまでのぬいぐるみの総数(pair<int,int>)
 * 
 * たとえば S=b'1011 であれば、
 *   {0,1,3}, {0,3,1}, {1,0,3}, {1,3,0}, {3,0,1}, {3,1,0}   (数値は種類)
 * の順にぬいぐるみを並べる方法がある。
 * 巡回セールスマン問題と違い、最後にどの種類のぬいぐるみを選んだかの情報は不要。
 * 
 * i番目のぬいぐるみを左から並べる場合に、初期状態から何個取り出すかの計算が必要になる。
 *   toy_num[i]:i番目の種類のぬいぐるみの数
 *   toy_org_sum[i][j]:初期状態にて、i番目の種類のぬいぐるみが、左からj個の中で何個あるか(累積和)
 * を用いて計算する。
 * 最初は累積和を使わずにループで計算したところ、TLEとなった。
 */

const int MAX_M = 20;  // ぬいぐるみの種類の最大
int toy_num[MAX_M];  // toy_num[i]:i番目の種類のぬいぐるみの数

// toy_org_sum[i][j]:初期状態にて、i番目の種類のぬいぐるみが、左からj個の中で何個あるか(累積和)
vector<vector<int>> toy_org_sum;

// dp[S]:
//   Sの各ビットをそれぞれのぬいぐるみに割り当てて、
//   1になっているビットに対して好きな順に左から並べようとしたときに(種類ごとに連続して置く)、
//   交換するために取り出したぬいぐるみの最小数と、そこまでのぬいぐるみの総数(pair<int,int>)
//   未探索であれば-1
pair<int,int> dp[1<<MAX_M];
int N, M;

pair<int,int> func(int S)
{
	int i;

	// メモ化再帰
	if(dp[S].first != -1) return dp[S];

	// 1になっているビットが1つ、つまり1種類目のぬいぐるみを割り当てる場合
	if(__builtin_popcount(S) == 1)
	{
		int pos;
		for(i = 0; i < M; i++)
		{
			if(S == (1<<i))
			{
				pos = i;  // i=pos番目のぬいぐるみ
				break;
			}
		}
		// 取り出すぬいぐるみの数を計算
		// 左からtoy_num[pos]個のぬいぐるみのうち、種類pos以外の個数を計算する
		// (当初、ここをループで実装しておりTLEになった)
		dp[S].first = toy_num[pos] - toy_org_sum[pos][toy_num[pos]];
		dp[S].second = toy_num[pos];
		return dp[S];
	}

	int ret = INF32;
	for(i = 0; i < M; i++)
	{
		if((S & (1<<i)) == 0) continue;  // 1になっていないビットはスルー

		int S_prev = S & (~(1<<i));  // iに相当するビットのみ落とす
		auto e = func(S_prev);  // 再帰で dp[S-{i}] を計算

		dp[S].second = e.second+toy_num[i];  // いつ計算しても同じ

		// 種類iについて、取り出すぬいぐるみの数を計算
		int count;
		// 左からe.second番目(=これまでに確定した個数)からtoy_num[i]個のぬいぐるみのうち、
		// 種類i以外の個数を計算する
		// (当初、ここをループで実装しておりTLEになった)
		count = toy_num[i] - (toy_org_sum[i][e.second+toy_num[i]] - toy_org_sum[i][e.second]);
		ret = min(ret, e.first + count);
	}
	dp[S].first = ret;

	return dp[S];
}

int main(void)
{
	// 0-indexed
	int i, j;
	cin >> N >> M;  // N個のぬいぐるみ、M種類

	toy_org_sum.assign(M, vector<int>(N+1, 0));  // toy_org_sum[x][0] = 0;
	for(i = 0; i < N; i++)
	{
		int tmp;
		cin >> tmp;
		tmp--;  // 0-indexedに変換
		toy_num[tmp]++;
		// 累積和の計算
		// まず前回のものをコピーし、今回のぬいぐるみの種類についてインクリメント
		for(j = 0; j < M; j++)
		{
			toy_org_sum[j][i+1] = toy_org_sum[j][i];
		}
		toy_org_sum[tmp][i+1]++;
	}
	// dpテーブル初期化
	for(i = 0; i < (1<<M); i++)
	{
		dp[i] = {-1,0};  // 未探索
	}

	auto e = func((1<<M)-1);
	cout << e.first << endl;

	return 0;
}
