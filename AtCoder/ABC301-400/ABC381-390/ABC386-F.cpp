#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <unordered_map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC386 https://atcoder.jp/contests/abc386

/*
 * 解説をチラ見した。
 * レーベンシュタイン距離とか編集距離と呼ぶらしい。
 * EDPC-F(LCS)が基本問題。 https://atcoder.jp/contests/dp/tasks/dp_f
 * 
 * 上記基本問題を参考にO(|S||T|)なら簡単に実装できるが、もちろんTLE.
 * K<=20という制約に着目する。
 * 例えばK=2のときS="ab",T="abcde"はNo回答で、なぜかと言うとK文字以上差があるから。
 * もう少し考えると、基本問題で考えるdp[i][j]について |i-j|>K のとき、そこには絶対にたどり着けない。
 * つまり各iについて、jはi-K～i+Kの範囲で探索すれば十分。
 * 
 * 添え字処理が面倒なのでvector<map>で実装したらTLE(&WA).
 * dp/ndpの方式で、かつmapをunordered_mapに変えたらTLEは解消してWAが残る。
 * 自分の実装だと、|T|-|S|>K のようなときにミスってたので、
 * 出力時に dp.count(|T|)>0 という判定を追加してAC.
 * 
 * ※各種解説ではmapではなく配列で添字計算を丁寧にやる方針だったが、自分はmapで手を抜いた。
 *   自分のunordered_mapだと1000msだったが、配列だと200ms未満で収まるみたい。
 *   https://atcoder.jp/contests/abc386/submissions/61646523
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・文字の挿入、削除、変更などは編集距離を考えてみる。
 * ・制約より、2つのパラメータについて全探索ではなく探索範囲を狭めることができるケースがある。
**/

int main(void)
{
	ll i, j;
	ll K; cin >> K;
	string s, t; cin >> s >> t;
	ll ssz = (ll)s.size();
	ll tsz = (ll)t.size();
	unordered_map<ll,ll> dp;
	for(i = 0; i <= ssz; i++)
	{
		unordered_map<ll,ll> ndp;
		for(j = i-K-1; j <= i+K+1; j++)
		{
			if(j < 0) continue;
			if(j > tsz) continue;
			if(i == 0 && j == 0)
			{
				ndp[j] = 0;
				continue;
			}
			ll tmp= INF64;
			if(i-1 >= 0 && dp.count(j) > 0)
			{
				chmin(tmp, dp[j]+1);
			}
			if(j-1 >= 0 && ndp.count(j-1) > 0)
			{
				chmin(tmp, ndp[j-1]+1);
			}
			if(i-1 >= 0 && j-1 >= 0 && dp.count(j-1) > 0)
			{
				if(s[i-1] == t[j-1]) chmin(tmp, dp[j-1]);
				else chmin(tmp, dp[j-1]+1);
			}
			ndp[j] = tmp;
			// cout << "i=" << i << ",j=" << j << ": " << ndp[j] << endl;
		}
		swap(dp, ndp);
	}
	YesNo(dp.count(tsz) > 0 && dp[tsz] <= K);

	return 0;
}
