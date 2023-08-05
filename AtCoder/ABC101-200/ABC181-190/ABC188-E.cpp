#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC188 https://atcoder.jp/contests/abc188

/*
 * 動的計画法で解く。
 * ※公式解説の解法1と同じ考え方だった。
 *   https://atcoder.jp/contests/abc188/editorial/477
 * 
 * dp[i]を以下のように定義する。
 *   町iに到着した時点で金を購入済みのとき、金額の最安値
 * 
 * 町iで売る場合、a[i]-dp[i]が利益となる。
 * この最大値が答となる。
 * 
 * X<Yの制約があるため、町1から順番に
 *   現在の町iから行ける町eそれぞれについて、
 *   dp[e] = min(dp[e], dp[i], a[i])
 * がDPの遷移式となる。
 * ※minは先頭から順に、町e, 町1～iの手前, 町iを指す
 */

using Graph = vector<vector<int>>;

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	for(i = 0; i < M; i++)
	{
		int x, y; cin >> x >> y;
		x--; y--;
		g[x].push_back(y);  // x->y
	}

	ll ans = -INF64;
	vector<ll> dp(N, INF64);
	for(i = 0; i < N; i++)
	{
		ans = max(ans, a[i] - dp[i]);  // 町iで売る
		for(auto &e : g[i])
		{
			dp[e] = min({dp[e], dp[i], a[i]});
		}
	}
	cout << ans << endl;

	return 0;
}
