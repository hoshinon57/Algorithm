#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <array>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC349 https://atcoder.jp/contests/abc349

/*
 * 再帰を使い、全探索で解く。
 * 
 * 状態数は9!=3.6*10^5.
 * 各状態からの遷移数は多くても9.
 * よって初期状態から全探索しても十分に間に合う。
 * 
 * func(mp,t)を以下のように定義する。
 *   現在の盤面がmp, 次の手番をt(0 or 1)としたときに、どちらが勝つか(0 or 1)
 * 再帰関数にて、以下方針で実装すればよい。
 * ・3マスそろったなら終了。
 * ・全て埋まったなら、スコア判定して終了。
 * ・各空きマスそれぞれについて、自分の色で塗って相手ターンへ遷移(再帰)。
 *   その結果に1つでも自分が勝つケースがあれば、自分の勝ち。1つも無ければ相手の勝ち。
 */

vector<vector<int>> bingo = 
{
	{0,1,2},{3,4,5},{6,7,8},
	{0,3,6},{1,4,7},{2,5,8},
	{0,4,8},{2,4,6}
};
vector<ll> a_;

int func(array<int,9> &mp, int t)
{
	int i, j;
	// そろった判定
	for(i = 0; i < (int)bingo.size(); i++)
	{
		for(j = 0; j < 2; j++)
		{
			if(mp[bingo[i][0]] == j && mp[bingo[i][1]] == j && mp[bingo[i][2]] == j)
			{
				return j;
			}
		}
	}

	bool end = true;
	for(i = 0; i < 9; i++)
	{
		if(mp[i] == -1) end = false;
	}
	if(end)  // 全マス埋まったか
	{
		ll fi = 0, se = 0;
		for(i = 0; i < 9; i++)
		{
			if(mp[i] == 0) fi += a_[i];
			else se += a_[i];
		}
		if(fi > se) return 0;
		else return 1;
	}

	for(i = 0; i < 9; i++)
	{
		if(mp[i] != -1) continue;  // 既に置かれている
		mp[i] = t;
		int ret = func(mp, 1-t);
		mp[i] = -1;

		// 遷移先で自分が勝つケースが1つでもあればよい
		if(ret == t)
		{
			return t;
		}
	}

	// どの遷移先も勝てなかったケース 相手の勝ち
	return 1-t;
}

int main(void)
{
	int i;
	a_.resize(9);
	for(i = 0; i < 9; i++) cin >> a_[i];

	array<int,9> mp;
	mp.fill(-1);

	string ans = "Aoki";
	if(func(mp, 0) == 0) ans = "Takahashi";
	cout << ans << endl;

	return 0;
}
