#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題21
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc023

/*
 * 二分探索で解く。
 * 
 * 次の判定問題を考える。
 * 「スコアxを達成することは可能か？」
 * 
 * これは、以下の手順で判定できる。
 * (1)各風船において、高度x以下で風船を割るまでの猶予時間を求める
 * (2)(1)の時間を短い順にソートし、順に割っていったときに猶予時間を満たせるか判定する
 * ※ソート結果が{0,0,1,3,4}なら条件を満たせる、{0,2,2,3,4}なら満たせない
 * 
 * あとはxについて「達成できる」「達成できない」の境界を二分探索で範囲を狭めていけばよい。
 * 
 * 計算量は、check()での判定問題がO(NlogN),
 * 二分探索の範囲をM(=INF64)としたときにO(logM)なので、合わせてO(NlogNlogM)となる。
 * 
 * ※判定問題にてソートを使わない方法もある。累積和を使っている。この場合O(N).
 *   参考：
 *     https://scrapbox.io/longshoujin/ABC023_D_-_%E5%B0%84%E6%92%83%E7%8E%8B
 *     https://algo-logic.info/abc023d/
 */

int main(void)
{
	ll i;
	int N;
	cin >> N;
	vector<ll> H(N), S(N);
	for(i = 0; i < N; i++) cin >> H[i] >> S[i];

	// スコアxを達成できるならtrue
	auto check = [&](ll x) -> bool
	{
		vector<ll> limit(N);  // 風船iについて、高度x以下を満たす最大の時刻
		for(i = 0; i < N; i++)
		{
			// 初期高度がxより高い場合、条件を満たせない
			if(H[i] > x) return false;

			limit[i] = (x-H[i])/S[i];  // i秒目までに割れば条件を満たせる
		}
		sort(limit.begin(), limit.end());  // 割るまでの猶予が短い順にソート
		for(i = 0; i < N; i++)  // i秒目に、猶予がi番目に短い風船を割る
		{
			if(limit[i] < i) return false;  // 猶予時間内に風船を割れない
		}
		return true;
	};

	ll l = -1;  // スコアを達成できない側
	ll r = INF64;  // スコアを達成できる側
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		if(check(mid))
			r = mid;
		else
			l = mid;
	}
	cout << r << endl;

	return 0;
}
