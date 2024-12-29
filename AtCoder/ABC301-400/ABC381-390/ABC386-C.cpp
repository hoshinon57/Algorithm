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

// ABC386 https://atcoder.jp/contests/abc386

/*
 * 以下の方針で解ける。
 * ・SとTの文字列長の差が2以上であれば、一致できないのでNo.
 * ・|S|<=|T|に限定して処理したいので、|S|>|T|なら交換する。
 *   (実装では引数を逆にして再呼び出ししている)
 * ※以下、|S|<=|T| かつ差は1以下
 * ・S,Tの見ている位置をそれぞれspos,tposとする。
 *   spos<|S|の間、処理する。
 *   ・s[spos]==t[tpos]ならS,Tともに次の文字へ。
 *   ・不一致の場合、ミスカウントが2になればNoで終了。
 *     tposを+1して先に進める。(|S|<=|T|のため)
 *     |S|=|T|の場合、sposも+1する。
 * sposが最後まで行けばYes.
 */

// 文字列S,Tについて、Sに1回だけ挿入、削除、変更操作を行ってTに一致できればtrue
// 参考：https://atcoder.jp/contests/abc324/editorial/7421
bool solve(string &s, string &t) {
	int sl = (int)s.size();
	int tl = (int)t.size();
	if(abs(sl-tl) >= 2) return false;
	if(sl > tl) return solve(t, s);

	// 以下、|S|<=|T| かつ差は1以下
	int spos=0, tpos=0;
	int miss = 0;
	while(spos < sl)
	{
		if(s[spos] == t[tpos])
		{
			spos++;
			tpos++;
		}
		else
		{
			miss++;
			if(miss >= 2) return false;  // 不一致は1回まで
			if(sl == tl) spos++;  // |S|=|T|の場合、操作は文字変更のためsの位置もずらす
			tpos++;
		}
	}
	return true;
}

int main(void)
{
	int K; cin >> K;
	string s, t; cin >> s >> t;
	YesNo(solve(s,t));

	return 0;
}
