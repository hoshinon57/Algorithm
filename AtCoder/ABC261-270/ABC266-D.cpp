#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC266 https://atcoder.jp/contests/abc266

/*
 * 動的計画法で解く。
 * 
 * dp[j][i]を以下のように定義する。
 *    i:時刻iまで終わったときに  (i:0～100000)
 *    j:高橋君が座標jにいるときの  (j:0～4)
 *    スコアの最大値
 * ただし、その時刻までに到達できない箇所は-1とする(スコア0とは別扱い)(後述※)
 * 
 * DPの遷移式は以下になる。
 *   dp[j][i] = max(dp[j-1][i-1], dp[j][i-1], dp[j+1][i-1])
 * 遷移を行った上で、Snuke君を捕まえる処理を行えばよい。
 * 
 * (※)
 * ただし、例えば時刻1に座標4に出てくるSnuke君は捕まえることができないので、
 * これは捕まえる処理から外す必要がある。
 * dp[]にて「その時刻までに到達できない座標」を-1にすることで実現する。
 */

int main(void)
{
	ll i, j;
	ll N;
	cin >> N;
	// x[i]:時刻iに出てくるSnuke君の座標
	//      「座標0で出る」と「出てこない」の区別がつかないが、後者は大きさを0とすれば正しく計算できる
	// a[i]:時刻iに出てくるSnuke君の大きさ
	//      出てこない時刻では0とする
	vector<ll> x(100000+1), a(100000+1);
	for(i = 0; i < N; i++)
	{
		ll t;
		cin >> t;
		cin >> x[t] >> a[t];
	}
	// ここまで入力

	// dp[j][i]
	//   i:時刻iまで終わったときに  (i:0～100000)
	//   j:高橋君が座標jにいるときの  (j:0～4 ただし判定を楽にするために0～5を確保しておく)
	//   スコアの最大値
	// その時刻までに到達できない箇所は-1とする(スコア0とは別扱い)
	vector<vector<ll>> dp(5+1, vector<ll>(100000+1, -1));
	dp[0][0] = 0;  // 初期値 時刻0に座標0にいる

	for(i = 1; i <= 100000; i++)  // 時刻
	{
		for(j = 0; j <= 4; j++)
		{
			if(j==0)
			{
				dp[0][i] = max(dp[0][i-1], dp[1][i-1]);
			}
			else
			{
				dp[j][i] = max(max(dp[j-1][i-1], dp[j][i-1]), dp[j+1][i-1]);
			}
		}
		// Snuke君を捕まえる
		// ただし、その時刻までに到達できない座標に出るなら捕まえられない
		if(dp[x[i]][i] != -1)
		{
			dp[x[i]][i] += a[i];
		}
	}

	// dp[0][100000],[1][100000],...,[4][100000]の最大値が答
	ll answer = 0;
	for(i = 0; i <= 4; i++)
	{
		answer = max(answer, dp[i][100000]);
	}
	cout << answer << endl;

	return 0;
}
