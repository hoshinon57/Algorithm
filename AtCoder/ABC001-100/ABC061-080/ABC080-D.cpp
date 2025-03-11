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

// ABC080 https://atcoder.jp/contests/abc080

/*
 * 制約よりO(NC)解法が間に合う。
 * なので「区間をsetで管理」「いもす法やイベントソート」といった方針も考えたが、もっと単純実装とした。
 * ※ちなみにいもす法だと同じチャンネルで続く番組での管理に悩んだが、
 *   チャンネルごとにいもす法をやり、いもす値が2であれば1に補正して、
 *   最後に各チャンネルごとのいもす値を合算、とすれば上手くいく。
 * 
 * rec[i][j]を
 *   チャンネルiの時刻jから1秒間、録画必要ならtrue
 * と定義する。
 * 入力s,tについてj=[s,t]となるrec[i][j]をtrueにしてあげると、
 * 各時刻tについてrec[*][t]=trueとなる個数の最大値が答。
 * 
 * [ACまでの思考の流れ]
 * ・入力[s,t)で[1,2),[2,3)とあったとき、同じチャンネルか異なるチャンネルかで必要な録画機の個数が異なる。
 *   ならばチャンネルごとに変数を持ったり処理したりすればよい。
**/

int main(void)
{
	ll i, j;
	ll N, C; cin >> N >> C;
	const ll TMX = 100000;  // 時刻最大値
	vector<vector<bool>> rec(C, vector<bool>(TMX+1, 0));
	for(i = 0; i < N; i++)
	{
		ll s, t, c; cin >> s >> t >> c;
		c--;
		for(j = s; j <= t; j++) rec[c][j] = true;
	}

	int ans = 0;
	for(i = 0; i <= TMX; i++)
	{
		int cnt = 0;
		for(j = 0; j < C; j++)
		{
			if(rec[j][i]) cnt++;
		}
		chmax(ans, cnt);
	}
	cout << ans << endl;

	return 0;
}
