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
 * コンテスト時に解けず。むずい、何だこれ。
 * 二分探索、physics0523氏の解法で解く。
 *   https://atcoder.jp/contests/abc381/editorial/11424
 * 
 * クエリ[L,R]に含まれる'/'を列挙し、そのうちの1つを考えてみる。この位置をposとすると、
 *   (1)posより手前にある1の個数 -> [L,pos)の1の個数
 *   (2)posより後ろにある2の個数 -> [pos+1,R+1)の2の個数
 * のうち小さい方から"11/22文字列"の長さが分かる。
 * これは事前に1,2の登場数の累積和を求めておくことで、O(1)で求められる。
 * 
 * ただし'/'が登場する全てについて計算すると、クエリごとにO(N)となり、全体でO(NQ)で間に合わない。
 * '/'の位置を1つ右に動かすと、上記(1)は増えて(2)は減る。よって
 *   L,R,posが与えられたとき、(1)<(2)ならtrue
 * という判定問題を考えると、posを増やしたときに true,true,...,true,false,false,... とt/fが1点で切り替わることから二分探索で解ける。
 * 
 * 二分探索が終わったときにl側とr側のどちらが最適かは、やってみないと分からない。
 * そのため最後に両方をチェックする必要がある…と思う。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・クエリ問題でクエリ先読みが使えそうにない場合、（当たり前だが）クエリごとの処理になる。
 *   愚直にやるとO(NQ)で間に合わない場合、logをつけることを考える。
 *   ⇒二分探索とかセグメント木とか。
 * ・二分探索を行った後にL側,R側それぞれで再チェックして最適解を求める場合、
 *   L,Rの初期値を判定関数に入れても大丈夫なよう、合法な値にしておくと良いかも。
 */

int main(void)
{
	int i;
	int N, Q; cin >> N >> Q;
	string s; cin >> s;
	vector<int> s1(N+1), s2(N+1);  // それぞれ1,2の累積和
	vector<int> possl;  // '/'の登場位置
	for(i = 0; i < N; i++)
	{
		s1[i+1] = s1[i];
		if(s[i] == '1') s1[i+1]++;
		s2[i+1] = s2[i];
		if(s[i] == '2') s2[i+1]++;
		if(s[i] == '/') possl.push_back(i);
	}

	while(Q > 0)
	{
		Q--;
		int L, R; cin >> L >> R;
		L--; R--;  // [L,R]
		int li = lower_bound(possl.begin(), possl.end(), L) - possl.begin();  // L以降にある'/'の最初の位置
		int ri = upper_bound(possl.begin(), possl.end(), R) - possl.begin() - 1;  // R以前にある'/'の最初（最右）の位置
		if(li > ri)  // [L,R]に'/'が無ければ0
		{
			cout << 0 << endl;
			continue;
		}

		// [possl[li], possl[ri]]の範囲で調べる
		int ans = 0;
		// possl[pi]の'/'を使ったとき、[L,possl[pi])にある1と[possl[pi]+1, R+1)にある2の個数の大小を調べる
		// "1の個数"<"2の個数"ならtrue
		// ansの更新も行っておく
		auto judge = [&](int pi) -> bool
		{
			int tmp1 = s1[possl[pi]] - s1[L];
			int tmp2 = s2[R+1] - s2[possl[pi]+1];
			chmax(ans, min(tmp1,tmp2)*2+1);  // "1の個数","2の個数"のうち小さい方をベースに、"11/22文字列"の長さを求める
			return tmp1<tmp2;
		};
		while(ri - li > 1)
		{
			int mid = (li+ri)/2;
			if(judge(mid)) li = mid;
			else ri = mid;
		}
		// l,r側のどちらかが最適解なので、両方試す
		judge(li);
		judge(ri);
		cout << ans << endl;
	}

	return 0;
}
