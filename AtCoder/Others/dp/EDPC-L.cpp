#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * 基本は公式解説の通り。
 *   https://kyopro-friends.hatenablog.com/entry/2019/01/12/231000
 *   https://drken1215.hatenablog.com/entry/2023/07/22/031100
 * 
 * 太郎君はX-Yを最大化したく、
 * 次郎君はX-Yを最小化したい。
 * 後者は -(X-Y) を最大化したい ⇒ Y-X を最大化したい と読み替えると、
 * 2者とも「自分のスコア - 相手のスコア」を最大化したいとなる。
 * 
 * dp[l][r]を以下のように定義する。
 *   区間[l,r]が残っている状態から始めたときの、「自分のスコア - 相手のスコア」の最大値
 * 
 * 今の手番が太郎君か次郎君かは関係なく、例えばa[l]を取るときは
 *   a[l] - dp[l+1][r]
 * で計算することができる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・最初は、dp[2][l][r]のように「今どちらのターンか」も保持していた。
 *   これでもACはするが、実装としては冗長だった。
 * ・2者ともに「自分のスコア - 相手のスコア」を最大化したいような問題では、
 *   dp値を「今の盤面から始めたときの、自分のスコア - 相手のスコア の最大値」とすることで、
 *   「今どちらのターンか」の情報は不要となる。
 */

int N;
vector<ll> a;
vector<vector<ll>> dp;  // dp[l][r]  [l,r]

ll func(int l, int r)
{
	ll &d = dp[l][r];
	if(d != -1) return d;
	if(l == r)  // ベースケース
	{
		return d = a[l];
	}

	ll d1 = a[l] - func(l+1, r);
	ll d2 = a[r] - func(l, r-1);
	d = max(d1, d2);

	return d;
}

int main(void)
{
	int i;
	cin >> N;
	a.resize(N);
	for(i = 0; i < N; i++) cin >> a[i];
	dp.resize(N, vector<ll>(N, -1));

	cout << func(0, N-1) << endl;

	return 0;
}
