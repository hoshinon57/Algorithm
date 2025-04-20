#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <unordered_map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC082 https://atcoder.jp/contests/abc082

/*
 * x,y独立に考える。動的計画法(next DP)で解く。
 *   https://blog.hamayanhamayan.com/entry/2017/12/16/231529
 * 
 * 前処理として、Tで囲まれたFの個数を抜き出す。
 * 偶数番目の個数はx方向に、奇数番目はy方向に移動する。
 * 
 * x,y独立で考えられるので、例としてx側、Fの個数をdxに登録していくと、
 * 例えばdx={2,4,0,1,...}のとき 2±4±0±1±... が入力xと一致させることができるか、となる。
 * ※先頭のFのみ、xのプラス方向にしか進めない(マイナス方向には進めない)ことに注意。
 *   実装ではcal()にてxflagで処理している。
 * これはDP(next DP)にて、dp[i]:座標iにいることができればtrue として解ける。
 * 座標は負の値も取りうるので、
 *   ゲタをはかせる
 *   mapで楽する
 * などあるが、今回は後者とした。(一応、unordered_mapを使ったが、286ms VS 298ms で大差なかった)
 */

int main(void)
{
	int i;
	string s; cin >> s;
	s += 'T';  // 番兵
	int x, y; cin >> x >> y;
	vector<int> dx, dy;

	int cur = 0;  // 0:x, 1:y
	while(i < (int)s.size())
	{
		// F連続
		int cnt = 0;
		while(s[i] == 'F')
		{
			cnt++;
			i++;
		}
		// s[i]='T'
		if(cur == 0) dx.push_back(cnt);
		else dy.push_back(cnt);
		cur = 1-cur;
		i++;
	}

	auto cal = [&](vector<int> &d, bool xflag) -> unordered_map<int,bool>
	{
		unordered_map<int,bool> dp;
		dp[0] = true;  // DP初期値
		for(auto &e : d)
		{
			unordered_map<int,bool> ndp;
			for(auto [key, val] : dp)
			{
				ndp[key+e] = true;
				if(!xflag) ndp[key-e] = true;  // xの初回のみ、プラス方向にしか進めない
			}
			swap(dp, ndp);
			xflag = false;
		}
		return dp;
	};
	auto dpx = cal(dx, true);
	auto dpy = cal(dy, false);

	YesNo(dpx[x] && dpy[y]);

	return 0;
}
