#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC190 https://atcoder.jp/contests/abc190

/*
 * Binary Indexed Treeで解く。
 * 
 * まずk=0の場合は、BITを用いて転倒数を計算する。
 * k=1以降も同じ手法だと、計算量がO(N*NlogN)となりTLE.
 * k=1以降は前回の結果からの変化点に着目して計算する。
 * 
 * 操作ごとに転倒数はあまり変わらなくて、
 * 左端にあったa[i]が右端に移動する部分のみ影響している。
 * (1)a[i]より大きい値：
 *   a[i]が右端に移動することにより、これらは転倒数の組になる。
 *   つまり (N-1)-a[i] だけ転倒数が増えることになる。
 * (2)a[i]より小さい値：
 *   a[i]が右端に移動することにより、元は転倒していたものが非転倒になる。
 *   つまりa[i]だけ転倒数が減ることになる。
 * よって操作ごとの転倒数の変化は (N-1)-a[i] - a[i] = N-1-a[i]*2 となる。
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

int main(void)
{
	int i;
	int N; cin >> N;
	BIT<int> bit(N+1);
	vector<int> a(N);
	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		ans += i - bit.Sum(a[i]+1);  // 1-indexedに変換するため+1
		bit.Add(a[i]+1, 1);
	}
	cout << ans << endl;  // k=0の場合

	for(i = 0; i < N-1; i++)  // k=1～N-1の場合
	{
		ans += N-1 - a[i]*2;
		cout << ans << endl;
	}

	return 0;
}
