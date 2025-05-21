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

// ABC403 https://atcoder.jp/contests/abc403

/*
 * Dごとに、動的計画法で解く。各種解説の通り。
 *   https://atcoder.jp/contests/abc403/editorial/12823
 *   https://x.com/kyopro_friends/status/1916488596862935249  
 *   https://zenn.dev/testcampus/articles/7688e87a0ae22e
 * 
 * まず、D=0だけは別処理。実装参照で割愛。(1WAした…)
 * 以下、D>=1について記載する。
 * 
 * Aの値に興味があるので、登場回数ごとにカウントする。
 * カウントしたものをDごとに取り出したとき、1以上が隣り合ってはダメ。
 * 隣り合わないように要素を取り除くが、取り除く回数を最小にしたい。
 * 
 * 以下のDPを考える。
 *   dp[i]:
 *   そこまでの要素を見たときに、
 *   i=0:その要素を取り除く, i=1:取り除かない
 *   ときの、取り除く回数の最小値
 * 「取り除かない」⇒「取り除かない」への遷移は存在しない。
 * これでカウントが元々0のところもうまく処理できる。
 *   
 * [ACまでの思考の流れ]
 * ・Dごとが重要なので、D単位(mod D)で取り出してみる
 */

int main(void)
{
	ll i, j;
	ll N, D; cin >> N >> D;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	const ll MXA = 1000000;
	vector<ll> cnt(MXA+5); for(auto &e : a) {cnt[e]++;}

	ll ans = 0;
	if(D == 0)
	{
		for(auto &e : cnt)
		{
			if(e == 0) continue;
			ans += e-1;
		}
		cout << ans << endl;
		return 0;
	}

	auto cal = [&](vector<ll> &li) -> void
	{
		vector<ll> dp(2);
		for(auto &e : li)
		{
			vector<ll> ndp(2);
			// 取り除かないケース
			ndp[0] = dp[1];
			// 取り除くケース
			ndp[1] = min(dp[0], dp[1])+e;
			swap(dp, ndp);
		}
		ans += *min_element(dp.begin(), dp.end());
	};
	for(i = 0; i < D; i++)  // 数値iスタートで、D単位で取り出す
	{
		vector<ll> li;
		for(j = i; j <= MXA; j+=D)
		{
			li.push_back(cnt[j]);
		}
		cal(li);
	}
	cout << ans << endl;

	return 0;
}
