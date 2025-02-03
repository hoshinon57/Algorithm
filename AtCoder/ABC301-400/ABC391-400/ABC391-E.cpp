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

// ABC391 https://atcoder.jp/contests/abc391

/*
 * DPおよび再帰で解く。基本は公式解説の通り。
 *   https://atcoder.jp/contests/abc391/editorial/12103
 * 
 * dp[i][j]を
 *   操作i回目を行った後の列で、  (i=0～N)
 *   左からj番目の値について、  (j=0～)
 *   {val:操作後の値, cnt:それを0/1ひっくり返すための回数}
 * と定義する。
 * dp[N][0].second が答。
 * これを再帰で解いていく。
 * 
 * まずベースケースとしてdp[0][*].firstは入力Aから求められ、また.secondは明らかに1.
 * dp[x][j]は、dp[x-1][j*3], dp[x-1][j*3+1], dp[x-1][j*3+2] から求まる。（三分木を描くとイメージしやすい）
 * これら3つのvalの最頻値が、dp[x][j]のvalとなる。
 * dp[x][j]のcntについて、dp[x][j]とvalが一致するものが3つあれば、2つをひっくり返す必要があり、それはcntのうち小さい方2つの和となる。
 * 一致するものが2つであれば、1つをひっくり返せばよく、cntのうち小さい方となる。
 * この辺り、実装を見た方が早い。
 */

int main(void)
{
	int N; cin >> N;
	string a; cin >> a;

	// 操作ope回目、左からidx番目
	auto f = [&](auto self, ll ope, ll idx) -> pair<ll,ll>
	{
		if(ope == 0)  // ベースケース
		{
			return {a[idx]-'0', 1};  // ひっくり返すための回数は1回
		}

		auto [val1, cnt1] = self(self, ope-1, idx*3);
		auto [val2, cnt2] = self(self, ope-1, idx*3+1);
		auto [val3, cnt3] = self(self, ope-1, idx*3+2);
		ll v;
		{
			vector<ll> tmp = {val1, val2, val3};  // 最頻値を求める
			sort(tmp.begin(), tmp.end());
			v = tmp[1];
		}
		vector<ll> cc;
		// ifは2回 or 3回ヒットする
		// v=val1=val2=val3なら操作回数の小さい方2つの加算が答、
		// vと一致が2つなら操作回数の小さい方が答。
		if(v == val1) cc.push_back(cnt1);
		if(v == val2) cc.push_back(cnt2);
		if(v == val3) cc.push_back(cnt3);
		sort(cc.begin(), cc.end());
		ll c = 0;
		if(cc.size() == 3) c = cc[0]+cc[1];
		else c = cc[0];  // size=2
		return {v, c};
	};
	cout << f(f, N, 0).second << endl;

	return 0;
}
