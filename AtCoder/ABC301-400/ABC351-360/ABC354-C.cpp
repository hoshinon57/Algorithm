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

// ABC354 https://atcoder.jp/contests/abc354

/*
 * カードを横軸A,縦軸Cの平面にプロットすると分かりやすい。
 * Aの大きい方から順にカード見ていって、
 * それまでのCminよりCが大きいカードがあれば、それを捨てることになる。
 * 
 * AおよびCは全て異なる値なので、同じ値のケアが不要なぶん、処理は楽にできる。
 * (コンテスト時はそこを読み落としていて、だいぶ面倒な実装にしてしまった)
 * 
 * [ACまでの思考の流れ] [どう思考すれば解法にたどり着けるか]
 * ・2要素の大小関係を考えるとき、2次元平面で考えるとイメージしやすい。
 * ・大小関係の条件が2つあるときは、片方でソートすると上手くいくことがある。(片方の条件を常に満たせるようになる)
 *   https://x.com/kyopro_friends/status/1793951189798916579
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<array<int,3>> c(N);  // {A, C, no.}
	for(i = 0; i < N; i++)
	{
		cin >> c[i][0] >> c[i][1];
		c[i][2] = i+1;
	}
	sort(c.begin(), c.end(), greater<array<int,3>>());  // Aで降順

	int cmin = INF32;
	vector<int> ans;
	for(auto [aa, cc, id] : c)
	{
		if(cc < cmin)  // 残す
		{
			cmin = cc;
			ans.push_back(id);
		}
		else {;}  // 捨てる
	}

	sort(ans.begin(), ans.end());
	int sz_ = (int)ans.size();
	cout << sz_ << endl;
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}
