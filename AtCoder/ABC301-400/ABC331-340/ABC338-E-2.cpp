#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <array>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC338 https://atcoder.jp/contests/abc338

/*
 * コンテスト時、および公式解説の方法。
 * スタックを用いて解く。
 *   https://atcoder.jp/contests/abc338/editorial/9172
 *   https://x.com/kyopro_friends/status/1751240830646698040
 * 
 * ※典型90-17と違い、Ax,Bxが全て相異なるのがポイント。
 *   この制約により、簡単に実装できる。
 * 
 * まず、A<Bになるようにしておく。
 * 各線分iについて、
 *   {A,i,0} 0:線分の始まり
 *   {B,i,1} 1:線分の終わり
 * となるようデータに積み、ソートする。
 * 
 * データの先頭から見ていき、
 *   線分の始まり：iをスタックに積む
 *   線分の終わり：スタックの末尾とiが不一致ならば、その線分は他と公差している
 * となる。
 */

bool solve(void)
{
	int i;
	int N; cin >> N;
	vector<array<int,3>> d;  // {pos, idx, (in:0, out:1)}
	for(i = 0; i < N; i++)
	{
		int a, b; cin >> a >> b;
		if(a>b) swap(a,b);
		d.push_back({a, i, 0});
		d.push_back({b, i, 1});
	}
	sort(d.begin(), d.end());

	vector<int> st;  // スタック的な
	for(auto [pos, idx, k] : d)
	{
		if(k == 0)  // 線分の始まり
		{
			st.push_back(idx);
		}
		else  // 線分の終わり
		{
			if(st.back() != idx) return true;  // スタックの末尾が自分以外のものだったら、公差する
			st.pop_back();
		}
	}
	return false;
}

int main(void)
{
	YesNo(solve());
	return 0;
}
