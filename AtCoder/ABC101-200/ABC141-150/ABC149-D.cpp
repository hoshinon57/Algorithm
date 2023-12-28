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

// ABC149 https://atcoder.jp/contests/abc149

/*
 * 貪欲法で解いた。
 * 
 * まず、「勝たない」と決めた場合はあいこか負けかを選べるので、そのK回後には影響しない。
 * また今回勝てる場合、K回後に同じ手を出せないとしてもトータルでは損をしないので、
 * 「勝てるときに勝つ」という戦略が最適になる。
 * 
 * よって以下の貪欲法で解く。
 *   win[i]に、i回目に勝ったかどうかを保持する。
 *   K回前に勝っており、かつそれが今回と同じ出ならば、今回は勝てない。それ以外では勝つ。
 * 
 * ※解説より、K単位で分けてグループ分けしておき、そのグループごとに独立して考えるという手法もある。
 *   この場合は前回の手のみを考えればよくなるので、DPで解ける。
 *   ＞mod Kで分けることで独立なK個のゲームに分割できる
 *   https://twitter.com/kyopro_friends/status/1211267343822667776
 */

int main(void)
{
	int i;
	int N, K; cin >> N >> K;
	int R, S, P; cin >> R >> S >> P;
	string t; cin >> t;
	vector<bool> win(N, false);  // win[i]:i回目に勝つならtrue
	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		if(i < K) win[i] = true;  // K回目までは勝てる
		else
		{
			if(win[i-K] && t[i-K] == t[i]) {;}  // K回前に勝っていて、それが今回と同じ手ならば、今回は勝てない
			else win[i] = true;
		}
		if(win[i])
		{
			if(t[i] == 'r') ans += P;
			else if(t[i] == 's') ans += R;
			else ans += S;
		}
	}
	cout << ans << endl;

	return 0;
}
