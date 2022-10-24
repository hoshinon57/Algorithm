#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題47
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2015ho/tasks/joi2015ho_b

/*
 * 区間DPで解く。
 * 別解法での練習を目的として、再帰を使わず実装した。
 * 
 * dp[i][l][r]の定義は同じく以下。
 *   次が i=0ならJOIくんの番、i=1ならIOIちゃんの番であって、 (i=0,1)
 *   ケーキのピースが[l,r)だけ残っているときに、 (a[l],a[l+1],...,a[r-1]), (l,r=0～N)
 *   JOIくんが取れるピースの大きさの最大値。
 * ケーキは円状であるため、0番目のケーキに戻るのを考えるのが面倒(r>lになる)。
 * よってケーキを2周して考えることで、常にl<rを満たすようにする。
 * 
 * DPの遷移も同様(省略)。
 * 再帰を使わないので、dp[i][l][r]について、r-l(=ケーキの残りピース数)が小さいものから順に計算していく。
 * ケーキを2周して計算しているので、
 *   [0,N), [1,N+1), [2,N+2), ..., [N-1, 2N-1)
 * における最大値が答となる。
 * 
 * 計算量は変わらずO(N^2).
 */

const int N_MAX = 2000;
// dp[i][l][r]
//   次が i=0ならJOIくんの番、i=1ならIOIちゃんの番であって、 (i=0,1)
//   ケーキのピースが[l,r)だけ残っているときに、 (a[l],a[l+1],...,a[r-1]), (l,r=0～N)
//   JOIくんが取れるピースの大きさの最大値。
// ケーキは円状であるため、0番目のケーキに戻るのを考えるのが面倒(r>lになる)。
// よってケーキを2周して考えることで、常にl<rを満たすようにする。
ll dp[2][N_MAX*2+1][N_MAX*2+1] = {0};  // 厳密には N_MAX*2-1 で良いはず

int main(void)
{
	// 0-indexed
	int i;
	int N;
	cin >> N;
	vector<ll> a(N*2);  // ケーキのサイズ
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		a[i+N] = a[i];  // 2周して考える
	}
	// DPテーブル初期化は不要(初期値0のため)

	// 残りピース数について、小さい方からDP計算をしていく
	for(int piece = 1; piece <= N; piece++)
	{
		for(int l = 0; l < N*2; l++)
		{
			int r = l+piece;  // l番目からpiece個残っている状態、つまり[l,l+piece)を計算する
			if(r > N*2) break;

			// 残りピースが1個の場合
			if(piece == 1)
			{
				dp[0][l][r] = a[l];  // 自分のターンなら取れる
				dp[1][l][r] = 0;  // 相手のターンなら取れない
			}
			// 以下、残りピースが2個以上

			// dp[0][][]を求める
			ll tmp1 = a[l]   + dp[1][l+1][r];
			ll tmp2 = a[r-1] + dp[1][l][r-1];
			dp[0][l][r] = max(tmp1, tmp2);

			// dp[1][][]を求める
			if(a[l] > a[r-1])
			{
				dp[1][l][r] = dp[0][l+1][r];
			}
			else
			{
				dp[1][l][r] = dp[0][l][r-1];
			}
		}
	}

	// ケーキを2周して計算しているので、
	// [0,N), [1,N+1), [2,N+2), ..., [N-1, 2N-1)
	// における最大値が答となる
	ll answer = 0;
	for(i = 0; i < N; i++)
	{
		answer = max(answer, dp[0][i][i+N]);
	}
	cout << answer << endl;
	
	return 0;
}
