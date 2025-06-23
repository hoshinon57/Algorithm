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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;

// ABC411 https://atcoder.jp/contests/abc411

/*
 * ドチャクソ難しかった… よくこれコンテスト中に解けたな…
 * 主客転倒の類にになるらしい。
 * 
 * 期待値の線形性だったかな、
 *   (最大値がn1になる確率)*n1
 *   (最大値がn2になる確率)*n2
 *   …
 *   (最大値がnkになる確率)*nk
 * の和が求める答えになる。
 * 
 * 最大値がxになる確率は、本問でいうと
 *   (出目が全てx以下になる確率) - (出目が全て(x-1)以下になる確率)
 * となる。[典型]
 * 減算部分が無いと、例えばx=4のときに「全て3以下」のときもカウントしてしまう(このとき最大値は4ではないので)。
 * 
 * 出目の小さい方から求めていく。
 * ここでまた実装にアレコレ必要だが、おおよそ公式解説と同じ方針だった。(コンテスト時はmapを用いたが)
 *   https://atcoder.jp/contests/abc411/editorial/13361
 * ざっと書くと以下となる。
 * ・入力aの一覧をソート＆重複削除して、どんな目が出るのかを事前計算。 -> num
 * ・no[k]:小さい方からk番目の値があるサイコロのNo.
 *   zero_cnt:まだx以下の値が出ていないサイコロの数
 *   cnt[i]:サイコロiにてx以下の値が何回出たか
 *   などを用意する。
 * ・小さい方からi番目の出目について、(値はnum[i])
 *   no[]とcnt[]から確率の分子部分を更新する。
 *   zero_cnt!=0, つまりnum[i]以下の出目が存在しないサイコロが残っていれば、最大値がnum[i]になる確率は0になる。
 *   そうでなければ、前回値(pre)を引いて 6^N で割り、num[i]をかけたものを加算する。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・確率や期待値の問題にて「最大値=xとなる確率」は
 *   (最大値がx以下になる確率) - (最大値がx-1以下になる確率)
 *   で求められる。
**/

int main(void)
{
	ll i, j;
	ll N; cin >> N;
	vector<vector<ll>> a(N, vector<ll>(6, 0));  // a[N][6]
	vector<ll> num;  // a[][]に含まれるサイコロの値 重複を取り除いて小さい順に
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < 6; j++)
		{
			cin >> a[i][j];
			num.push_back(a[i][j]);
		}
	}
	sort(num.begin(), num.end());
	num.erase(unique(num.begin(), num.end()), num.end());
	ll sz = (ll)num.size();
	vector<vector<ll>> no(sz);  // no[k]:小さい方からk番目の値があるサイコロのNo.
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < 6; j++)
		{
			auto idx = lower_bound(num.begin(), num.end(), a[i][j]) - num.begin();
			no[idx].push_back(i);
		}
	}

	mint ans = 0;
	mint pow6 = ((mint)6).pow(N);  // 6^N
	mint pre = 0, now = 1;
	ll zero_cnt = N;  // まだx以下の値が出ていないサイコロの数
	vector<ll> cnt(N);  // cnt[i]:サイコロiにてx以下の値が何回出たか
	for(i = 0; i < sz; i++)
	{
		for(auto &e : no[i])  // e:サイコロのNo.
		{
			if(cnt[e] == 0)
				zero_cnt--;
			else
				now = now / cnt[e] * (cnt[e]+1);
			cnt[e]++;
		}
		if(zero_cnt != 0)
		{
			;  // "x以下"を持たないサイコロがある つまり確率0
			// pre=0のまま
		}
		else
		{
			ans += (now - pre) / pow6 * num[i];
			pre = now;
		}
	}
	cout << ans.val() << endl;

	return 0;
}
