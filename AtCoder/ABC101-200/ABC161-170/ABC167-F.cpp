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

// ABC167 https://atcoder.jp/contests/abc167

/*
 * 自力で解ききれず、解説を見た。
 *   https://kmjp.hatenablog.jp/entry/2020/05/10/0930
 *   https://ikatakos.com/pot/programming_algorithm/contest_history/atcoder/2020/0510_abc167
 *   https://x.com/kyopro_friends/status/1260562834741657600
 *   https://betrue12.hateblo.jp/entry/2020/05/10/224429
 * 
 * カッコ列の条件として、
 * (1) (と)の数が一致すること
 * (2) 左から順に見ていき、(をスコア+1, )をスコア-1として加算していったときに、スコアが常に0以上であること
 * を満たす必要がある。[典型]
 * 
 * まず、S全体で(1)を満たしていなければNoで終了。
 * 以下、(1)を満たすとする。
 * 
 * 各Sごとに {min,last} = {途中のスコアの最小値, 末尾でのスコア} を計算する。
 * (2)より、last>=0のSを先に並べるのが良さそう、となる。
 * またこのうち、minが大きいものから並べるのが良さそう、となる。
 * この順に並べていき、スコアが常に0以上であれば、まずlast>=0のグループについてはOK.
 * 
 * 問題はlast<0のグループ。これが解けなかった。
 * 反対側から見ると同じように判定できる。
 * {min,last}を{min-last, -last}と置き換えればよい。
 * 
 * last>=0とlast<0の両グループについて配置できたのであれば、全体として(2)を満たすので、OK.
 */

bool solve(void)
{
	int i;
	int N; cin >> N;
	int ltot = 0, rtot = 0;
	using pii = pair<int,int>;
	vector<pii> s1, s2;  // {(lの数-rの数)のmin, (lの数-rの数)のlast}
	for(i = 0; i < N; i++)
	{
		string s; cin >> s;
		int c = 0, minc = 0;
		for(auto &e : s)
		{
			if(e == '(') {c++; ltot++;}
			else {c--; rtot++;}
			chmin(minc, c);
		}
		// 末尾でのスコアが正負で格納先を変える
		if(c >= 0) s1.push_back({minc, c});
		else s2.push_back({minc-c, -c});  // 反対方向から見るので、反転
	}
	if(ltot != rtot) return false;

	auto check = [&](vector<pii> &arr) -> bool
	{
		int now = 0;  // それまでのスコア
		sort(arr.begin(), arr.end(), greater<pii>());  // minが大きい順にソート
		for(auto [m,lst] : arr)
		{
			if(now+m < 0) return false;  // それまでのスコア+次の要素の最小値 が負ならNG
			now += lst;
		}
		return true;
	};

	if(check(s1) && check(s2)) return true;
	else return false;
}

int main(void)
{
	YesNo(solve());

	return 0;
}
