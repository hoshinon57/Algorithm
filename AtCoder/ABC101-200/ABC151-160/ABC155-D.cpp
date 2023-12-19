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

// ABC155 https://atcoder.jp/contests/abc155

/*
 * 二分探索で解く。
 * [ACまでの思考の流れ]
 * ・「小さい順に並べたとき、K番目の値を求めよ」は二分探索で解けるケースがある。
 * ・「X以下の値がK個以上あるか？」という判定問題を考えると、
 *   false,false,...,false,true,true,...
 *   とどこかでfalse/trueが切り替わるので、そこを二分探索で求める。
 * [ACへのポイント]
 * ・「2つを選んでペアにする(重複はNG)」は、以下の方針だと実装が楽そう。
 *   ・同じものを選んでOKとし、同じ{Ai,Ai}が条件を満たすなら-1する。
 *   ・{Ai,Aj}と{Aj,Ai}がダブるので、最後に2で割る
 * 
 * 入力Aをソートしておき、上記判定問題を解けばよい…のだが、判定問題の実装に苦労した。
 * まず、「2つを選んでペアにする」は
 *   同じものを選んでOKとし、同じ{Ai,Ai}が条件を満たすなら-1する。
 *   {Ai,Aj}と{Aj,Ai}がダブるので、最後に2で割る
 * とした方が実装が簡単。
 * ※snuke氏の実装も同じ方針みたい。またけんちょん氏の解説も参考。
 *   https://atcoder.jp/contests/abc155/submissions/10109007
 *   https://drken1215.hatenablog.com/entry/2020/04/23/142200
 * 
 * Aについて1つ固定し、A*B<=Xを考えると、
 *   A>0のとき：B<=(X/A の切り捨て)
 *   A<0のとき：B>=(X/A の切り上げ)
 *   A=0のとき：X>=0なら全てのAについて条件を満たす
 * という場合分けが必要になる。
 * また(C++では)Xの正負によってX/Aが切り捨て扱いか切り上げ扱いかが変わるので、その場合分けも必要。
 * ここに苦戦した…
 */

int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	sort(a.begin(), a.end());

	// ペアの積がx以下になるのが、K個以上ならtrue
	auto check = [&](ll x) -> bool
	{
		ll cnt = 0;
		for(auto &e : a)
		{
			// 場合分けはソース先頭のコメントを参照
			if(e > 0)
			{
				if(x >= 0)
				{
					cnt += upper_bound(a.begin(), a.end(), x/e) - a.begin();
				}
				else
				{
					cnt += upper_bound(a.begin(), a.end(), (x-e+1)/e) - a.begin();
				}
			}
			else if(e < 0)
			{
				if(x >= 0)
				{
					cnt += a.end() - lower_bound(a.begin(), a.end(), x/e);
				}
				else
				{
					cnt += a.end() - lower_bound(a.begin(), a.end(), (x+e+1)/e);
				}
			}
			else
			{
				if(x >= 0) cnt += a.size();
			}
			if(e*e <= x) cnt--;  // {Ai,Ai}が条件を満たすなら除外
		}
		cnt /= 2;  // {Ai,Aj}と{Aj,Ai}がダブるので2で割る
		return (cnt >= K);
	};
	ll l = -INF64, r = INF64;
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		if(check(mid)) r = mid;
		else l = mid;
	}
	cout << r << endl;

	return 0;
}
