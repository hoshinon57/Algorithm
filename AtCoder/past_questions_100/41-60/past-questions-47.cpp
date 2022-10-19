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
 * 
 * dp[i][l][r]を以下のように定義する。
 *   次が i=0ならJOIくんの番、i=1ならIOIちゃんの番であって、 (i=0,1)
 *   ケーキのピースが[l,r)だけ残っているときに、 (a[l],a[l+1],...,a[r-1]), (l,r=0～N)
 *   JOIくんが取れるピースの大きさの最大値。
 * ケーキは円状であるため、0番目のケーキに戻るのを考えるのが面倒(r>lになる)。
 * よってケーキを2周して考えることで、常にl<rを満たすようにする。
 * 2周という考え方の参考：
 *   https://atcoder.jp/contests/abc268/editorial/4777
 * 
 * 遷移式は以下のようになる。
 * (1)ケーキが残り1個のとき
 *   自分(JOI)のターンならa[l]. ケーキを取れる。
 *   相手(IOI)のターンなら0. ケーキを取れない。
 * (2)ケーキが残り2個以上のとき
 *   自分のターンなら、左側を取るケースと右側を取るケースのうち、最終的に大きく取れる方を選ぶ。
 *   相手のターンなら、左側と右側で大きい方を選ぶ。
 * 以上をメモ化再帰にて実装した。
 * 
 * 最初だけは好きなところから取れるので、
 *   0個目を取るケース、1個目を取るケース、...、(N-1)個目を取るケース
 * のそれぞれを計算する。
 * 
 * 計算量は、
 *   (1)最初にJOIくんが取るケーキのパターンがN通り
 *   (2)dpの状態がN*N通り
 *   よってO(N^3).
 * …ではなく、(1)の各パターンでメモしたデータは残っているので、
 * (2)のみが影響することになり、O(N^2).
 */

const int N_MAX = 2000;
// dp[i][l][r]
//   次が i=0ならJOIくんの番、i=1ならIOIちゃんの番であって、 (i=0,1)
//   ケーキのピースが[l,r)だけ残っているときに、 (a[l],a[l+1],...,a[r-1]), (l,r=0～N)
//   JOIくんが取れるピースの大きさの最大値。
// ケーキは円状であるため、0番目のケーキに戻るのを考えるのが面倒(r>lになる)。
// よってケーキを2周して考えることで、常にl<rを満たすようにする。
ll dp[2][N_MAX*2+1][N_MAX*2+1];  // 厳密には N_MAX*2-1 で良いはず

vector<ll> a;  // ケーキのサイズ

// 再帰を用いてdp[turn][l][r]を返す
ll func(int turn, int l, int r)
{
	if(dp[turn][l][r] != -1) return dp[turn][l][r];  // メモ化再帰

	if(r - l == 1)  // 残り1個
	{
		// 自分(JOI)のターンならケーキを取れて、相手(IOI)のターンなら取れない
		dp[turn][l][r] = (turn == 0) ? a[l] : 0;
		return dp[turn][l][r];
	}

	// 残り2個以上
	if(turn == 0)  // 自分(JOI)のターン
	{
		// 左側を取るケースと、右側を取るケース、のうち最終的に大きく取れる方
		ll tmp1 = a[l]   + func(1, l+1, r);
		ll tmp2 = a[r-1] + func(1, l, r-1);
		dp[turn][l][r] = max(tmp1, tmp2);
	}
	else  // 相手(IOI)のターン
	{
		// 相手は大きい方を取る
		if(a[l] > a[r-1])
		{
			dp[turn][l][r] = func(0, l+1, r);
		}
		else
		{
			dp[turn][l][r] = func(0, l, r-1);
		}
	}

	return dp[turn][l][r];
}

int main(void)
{
	// 0-indexed
	int i, j;
	int N;
	cin >> N;
	a.resize(N*2);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		a[i+N] = a[i];  // 2周して考える
	}
	// DPテーブル初期化
	for(i = 0; i <= N_MAX*2; i++)
	{
		for(j = 0; j <= N_MAX*2; j++)
		{
			dp[0][i][j] = dp[1][i][j] = -1;
		}
	}

	ll answer = -1;
	for(i = 0; i < N; i++)
	{
		// 最初にi個目のケーキを取る
		ll tmp = a[i] + func(1, i+1, (i+1)+(N-1));  // (i+1)番目から(N-1)個残っている
		answer = max(answer, tmp);
	}
	cout << answer << endl;
	
	return 0;
}
