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

// ABC296 https://atcoder.jp/contests/abc296

/*
 * 自力で解けず、解説を見た。
 *   https://atcoder.jp/contests/abc296/editorial/6117
 *   https://x.com/kyopro_friends/status/1642162610832547841
 *   https://ikatakos.com/pot/programming_algorithm/contest_history/atcoder/2023/0401_abc296
 *   https://scrapbox.io/procon-kirokuyou/ABC296_F_-_Simultaneous_Swap_(500)
 * 
 * 以下の順に判定していく。
 * ・ソートしても不一致ならNo
 * ・同じ値の要素が1つでもあればYes
 * ・あとは、転倒数の偶奇が一致すればtrue, 不一致ならfalse
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・要素の交換は転倒数を考えてみる。要素を交換すると、転倒数の偶奇が切り替わる。
 *   ※偶奇が切り替わるのは隣同士の交換のときだと勘違いしていた。
 *     どの2要素を交換しても、転倒数の偶奇は切り替わる。反省。
 * ・操作ごとにA,Bそれぞれの転倒数偶奇が切り替わる。
 *   よって初期状態でA,Bの偶奇が異なっていればNoではなかろうか。
 * ・手作業で色々やってみると、A,Bの先頭から合わせていったときに、最後は
 *   A=(..., a, b)   or   A=(..., a, b)
 *   B=(..., b, a)        B=(..., a, b)
 *   となりそう。(そこまでは一致させられる)
 * ・同じ要素が含まれる場合、ラスト2個にそれを持っていけばA=Bにできるので、常にYesになりそう。
 */

// Binary Indexed Treeを用いて以下(1)(2)(4)をO(logN)で計算する
//   (1)Sum : iが与えられたとき、累積和A1+A2+...+Aiを計算
//   (2)Add : iとxが与えられたとき、Aiにxを加算する
//   (4)lower_bound : a1+a2+...+ax>=w となるような最小のxを返す (各aiが0以上であることが条件)
// [注意]
//   1-indexed
//   Sum()は閉区間で処理する
//   要素数nは2のべき乗でなくても良い
// 以下URLをほぼそのまま持ってきている
// https://www.slideshare.net/hcpc_hokudai/binary-indexed-tree
// https://algo-logic.info/binary-indexed-tree/
template <typename T>
struct BIT
{
	vector<T> array;  
	const int n;  // 1-indexedでA1～Anまでの数列を扱う

	BIT(int n_) : array(n_+1, 0), n(n_) {}  // 1-indxedのため配列の要素数は+1して確保
	
	// 1番目からi番目までの累積和を求める
	T Sum(int i)
	{
		T s = 0;
		while(i > 0)
		{
			s += array[i];
			i -= i & (-i);  // LSBを減算
		}
		return s;
	}

	// [i,j]の要素の累積和を求める
	T Sum(int i, int j)
	{
		T s1 = Sum(i-1);
		T s2 = Sum(j);
		return s2 - s1;
	}

	// i番目の要素にxを加算
	void Add(int i, T x)
	{
		while(i <= n)
		{
			array[i] += x;
			i += i & (-i);  // LSBを加算
		}
	}

	// a1 + a2 + ... + ax >= w となるような最小のxを返す
	// 二分探索を用いるので、
	//   a1
	//   a1+a2
	//   a1+a2+a3 ...
	// が狭義単調増加になっている必要がある
	// ⇒各aiが0以上であること、が条件
	int lower_bound(T w)
	{
		// a[i]>=0 の条件があるので、wが0以下なら"a1"と打ち切ってよい
		if(w <= 0) return 1;

		int l = 0, r = 1;
		while(r <= n) r <<= 1;  // n以下の、最小の2のべき乗
		for(int len = r; len > 0; len >>= 1)  // len:長さ 1段下るごとに1/2になっていく
		{
			// w以上にならないギリギリで探索していく
			if(l+len <= n && array[l+len] < w)  // [l+1, len]を採用するとき
			{
				w -= array[l+len];
				l += len;  // 右側の子に行くイメージ
			}
		}
		return l+1;  // 「w以上にならないギリギリ」なので、+1したものが答
	}
};

bool solve(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<int> b(N); for(i = 0; i < N; i++) {cin >> b[i];}
	auto as = a, bs = b;
	sort(as.begin(), as.end());
	sort(bs.begin(), bs.end());
	if(as != bs) return false;  // ソートして不一致ならfalse
	for(i = 1; i < N; i++)  // 同じ値が含まれれば必ずtrue
	{
		if(as[i] == as[i-1]) return true;
	}

	// あとは、転倒数の偶奇とtrue/falseが紐づく
	BIT<int> bit_a(N+5), bit_b(N+5);
	ll ainv = 0, binv = 0;
	for(i = 0; i < N; i++)
	{
		ainv += i - bit_a.Sum(a[i]);
		bit_a.Add(a[i], 1);
		binv += i - bit_b.Sum(b[i]);
		bit_b.Add(b[i], 1);
	}
	ainv %= 2;
	binv %= 2;
	return (ainv == binv);
}

int main(void)
{
	YesNo(solve());

	return 0;
}
