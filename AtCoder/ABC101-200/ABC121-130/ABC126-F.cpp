#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC126 https://atcoder.jp/contests/abc126

/*
 * 構築問題。自力で解けず、各種解説を見た。
 *   https://betrue12.hateblo.jp/entry/2019/05/20/213302
 *   https://img.atcoder.jp/abc126/editorial.pdf
 * 
 * 上記解説の通りで、
 * ・K>=2^M なら作れない
 * ・後述のコーナーケースを除き、それ以外なら構築できる。
 *   Kをまず置いて、左右に ... 4 5 6 7 K 7 6 5 4 ... 0 K と置けば条件を満たす。
 *   z xor z = 0 であることと、
 *   0～(2^M)-1 までの各値をxorすると0になること、の性質を活用する。
 * 
 * M=0,1はコーナーケース扱い、注意。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・構築問題は実際にいくつか作ってみて、どういう値なら作れそうか、作れなさそうかを考えてみる。
 * ・0～(2^M)-1 までの各値をxorすると0になる、という性質。
 */

int main(void)
{
	ll i;
	ll M, K; cin >> M >> K;
	ll pow2M = (ll)1<<M;  // 2^M
	if(K >= pow2M)
	{
		cout << -1 << endl;
		return 0;
	}
	if(M == 0)
	{
		cout << "0 0" << endl;
		return 0;
	}
	else if(M == 1)
	{
		if(K == 0)
		{
			cout << "0 0 1 1" << endl;
			return 0;
		}
		else  // K=1
		{
			cout << -1 << endl;
			return 0;
		}
	}
	// ここまでコーナーケース

	// 例えばM=4として、以下を構築する
	// 0 1 2 ... 14 15 K 15 14 ... 0 K
	vector<ll> ans;
	for(i = 0; i < pow2M; i++)
	{
		if(i == K) continue;
		ans.push_back(i);
	}
	ans.push_back(K);
	for(i = pow2M-1; i >= 0; i--)
	{
		if(i == K) continue;
		ans.push_back(i);
	}
	ans.push_back(K);

	for(i = 0; i < (int)ans.size(); i++)
	{
		if(i != 0) cout << " ";
		cout << ans[i];
	}
	cout << endl;

	return 0;
}
