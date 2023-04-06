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

// 「上級者が解くべき過去問精選 100 + 50 問」の問題133
// https://qiita.com/e869120/items/acba3dd8649d913102b5
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_B&lang=ja

/*
 * 区間加算(RAQ:Range Add Query)に対応したBinary Indexed Treeを用いて解く。
 * 以下URLを参考に実装した。
 *   https://www.slideshare.net/hcpc_hokudai/binary-indexed-tree
 *   https://algo-logic.info/binary-indexed-tree/
 */

// Binary Indexed Treeを用いて以下(1)～(3)をO(logN)で計算する
//   (1)Sum : iが与えられたとき、累積和A1+A2+...+Aiを計算
//            [i,j]の区間の累積和を求めるSumもある
//   (2)Add : iとxが与えられたとき、Aiにxを加算する
//   (3)Add_Range : l,rが与えられたとき、Al～Arにxを加算する (Range Add Query)
// [注意]
//   1-indxed
//   Sum(),Add_Range()は閉区間で処理する
//   要素数nは2のべき乗でなくても良い
// 以下URLをほぼそのまま持ってきている
// https://www.slideshare.net/hcpc_hokudai/binary-indexed-tree
// https://algo-logic.info/binary-indexed-tree/
template <typename T>
struct BIT_RAQ
{
private:
	vector<T> array[2];
	const int n;  // 1-indexedでA1～Anまでの数列を扱う

	// array[idx]に対し、1番目からi番目までの累積和を求める
	T Sum_sub(int idx, int i)
	{
		T s = 0;
		while(i > 0)
		{
			s += array[idx][i];
			i -= i & (-i);  // LSBを減算
		}
		return s;
	}

	// array[idx]に対し、i番目の要素にxを加算
	void Add_sub(int idx, int i, T x)
	{
		while(i <= n)
		{
			array[idx][i] += x;
			i += i & (-i);  // LSBを加算
		}
	}

public:
	// 引数n_に対し、A1～A(n_)まで使えるよう初期化(1-indexed)
	BIT_RAQ(int n_) : n(n_)
	{
		array[0].assign(n+1, 0);  // 1-indxedのため配列の要素数は+1して確保
		array[1].assign(n+1, 0);
	}

	// 1番目からi番目までの累積和を求める
	T Sum(int i)
	{
		return Sum_sub(1, i)*i + Sum_sub(0, i);
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
		Add_sub(0, i, x);
	}

	// [l,r]の要素にxを加算
	void Add_Range(int l, int r, T x)
	{
		/*
		 * BIT0 の l 番目に -x(l−1)を加算
		 * BIT0 の r+1 番目に xr を加算
		 * BIT1 の l 番目に x を加算
		 * BIT1 の r+1 番目に -x を加算
		 */
		Add_sub(0, l	, -x * (l - 1));
		Add_sub(0, r+1	,  x * r);
		Add_sub(1, l	,  x);
		Add_sub(1, r+1	, -x);
	}
};

int main(void)
{
	int n, q;
	cin >> n >> q;
	BIT_RAQ<int> b(n);

	while(q > 0)
	{
		int query;
		cin >> query;
		if(query == 0)
		{
			int s, t, x;
			cin >> s >> t >> x;
			b.Add_Range(s, t, x);
		}
		else
		{
			int i;
			cin >> i;
			cout << b.Sum(i, i) << endl;
		}
		q--;
	}

	return 0;
}
