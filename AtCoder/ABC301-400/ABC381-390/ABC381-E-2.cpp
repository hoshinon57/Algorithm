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

// ABC381 https://atcoder.jp/contests/abc381

/*
 * 公式解説の方針。
 *   https://atcoder.jp/contests/abc381/editorial/11415
 *   https://atcoder.jp/contests/abc381/submissions/60096513  snuke氏の実装
 *   https://x.com/kyopro_friends/status/1859957835121176650
 * 
 * クエリ[L,R]について、
 *   1をx個、/を1個、2をx個使った11/22文字列を作れるか？
 * という判定問題を考える。
 * これは二分探索でtrue/falseの境界を求めることができる。
 * 
 * 判定問題は
 *   L以降で1がx個目に出てくる位置、の次の位置 -> pos
 *   pos以降で/が1個目に出てくる位置、の次の位置 -> pos
 *   pos以降で2がx個目に出てくる位置、の次の位置 -> pos
 *   pos-1 <= R ならtrue
 * として求められる。
 * x=0のケースが面倒で、「出てくる位置、の次の位置」という処理となった。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・クエリ問題でクエリ先読みが使えそうにない場合、（当たり前だが）クエリごとの処理になる。
 *   愚直にやるとO(NQ)で間に合わない場合、logをつけることを考える。
 *   二分探索とかセグメント木とか。
 * ・長さ(k-1)の11/22文字列が作れるなら、長さkのも作れる。
 *   長さkのが作れないなら、(k+1)のも作れない。
 *   ここから「1および2の長さがxである11/22文字列を作れるか？」を思いつけるようにする。
 *   登場位置を列挙しておく手法により、「あるposからx個目の1の登場位置は？」が求められる。
 */

int main(void)
{
	int i;
	int N, Q; cin >> N >> Q;
	string s; cin >> s;
	vector<int> pos1, pos2, possl;  // "1が登場する位置"の列挙 他も同様
	for(i = 0; i < N; i++)
	{
		if(s[i] == '1') pos1.push_back(i);
		else if(s[i] == '2') pos2.push_back(i);
		else possl.push_back(i);
	}

	while(Q > 0)
	{
		Q--;
		int L, R; cin >> L >> R;
		L--; R--;  // [L,R]

		// 登場位置を列挙しているposについて、位置st以降でnum個目の登場位置の"次の位置"を返す
		// ⇒よって戻り値は"次に見るべき位置"となる
		// num個なければINFを返す
		auto getNext = [&](vector<int> &pos, int st, int num) -> int
		{
			if(num == 0) return st;
			auto idx = lower_bound(pos.begin(), pos.end(), st) - pos.begin();
			idx = idx + num - 1;  // num個目の登場位置
			if(idx >= (int)pos.size()) return INF32;
			return pos[idx]+1;
		};
		
		auto check = [&](int x) -> bool
		{
			// Lから1をx個、そこから/を1個、そこから2をx個
			int pos = getNext(pos1, L, x);  // 戻り値posは次に見るべき位置となる
			pos = getNext(possl, pos, 1);
			pos = getNext(pos2, pos, x);
			return (pos-1 <= R);
		};

		int ok = -1, ng = INF32;
		while(ng - ok > 1)
		{
			int mid = (ok+ng)/2;
			if(check(mid)) ok = mid;
			else ng = mid;
		}
		int ans;
		if(ok == -1) ans = 0;
		else ans = ok*2+1;
		cout << ans << endl;
	}

	return 0;
}
