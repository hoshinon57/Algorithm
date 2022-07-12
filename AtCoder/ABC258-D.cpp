#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC258 https://atcoder.jp/contests/abc258

/*
 * 「同じステージを複数回クリアしたとしても、全てクリア回数に数えられる」という条件がある。
 * よって、
 * (1)ステージ1までクリアして、後は1～1ステージの中でゲーム部分が最小のものを繰り返す
 * (2)ステージ2までクリアして、後は1～2ステージの中でゲーム部分が最小のものを繰り返す
 * (3)ステージ3までクリアして、後は1～3ステージの中でゲーム部分が最小のものを繰り返す
 * …
 * (N)ステージNまでクリアして、後は1～Nステージの中でゲーム部分が最小のものを繰り返す
 * 上記(1)～(N)のうち最小のものが求める時間となる。
 * 
 * 「ステージxまでクリアして」の部分は、A/B入力時に累積和として計算しておく。
 */
int main(void)
{
	// 1-indexed
	ll i;
	ll N, X;
	cin >> N >> X;
	vector<ll> ab_sum(N+1);  // ab_sum[i]:1～iステージまでの、A,Bの累積和
	vector<ll> b_min(N+1);  // b_min[i]:1～iステージまでの、Bの最小値
	ll min_tmp = INF64;
	for(i = 1; i <= N; i++)
	{
		ll a, b;
		cin >> a >> b;
		ab_sum[i] = ab_sum[i-1] + a + b;
		min_tmp = min(min_tmp, b);
		b_min[i] = min_tmp;
	}

	ll answer = -1;
	for(i = 1; i <= N; i++)  // 「ステージiまでクリアして、後は1～iステージの中でゲーム部分が最小のものを繰り返す」を計算
	{
		if(X < i) break;  // そもそも、ステージiまでにクリアしてしまうため除外

		ll sum = 0;
		ll x_zan = X;  // あと何回クリアすればよいか

		// ステージiまでクリア
		sum += ab_sum[i];
		x_zan -= i;

		// 残りはゲーム部分が最小のステージを繰り返す
		sum += b_min[i] * x_zan;

		if(answer == -1)  // 初回
		{
			answer = sum;
		}
		else
		{
			answer = min(answer, sum);
		}
	}

	cout << answer << endl;

	return 0;
}
