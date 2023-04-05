#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Binary Indexed Tree(BIT)のメモ

/*
 * [ざっくり概要]
 * 以下のことがO(logN)できるデータ構造。
 *   iが与えられたとき、累積和A1+A2+...+Aiを計算
 *   iとxが与えられたとき、Aiにxを加算する
 * 要素数Nに対し、サイズNの配列で実装可能。
 * 
 * [参考資料]
 * https://www.slideshare.net/hcpc_hokudai/binary-indexed-tree
 * https://algo-logic.info/binary-indexed-tree/
 * 
 * [関連する問題]
 * AOJ DSL_2_B https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_B&lang=ja
 */

// Binary Indexed Treeを用いて以下(1)(2)をO(logN)で計算する
//   (1)iが与えられたとき、累積和A1+A2+...+Aiを計算
//   (2)iとxが与えられたとき、Aiにxを加算する
// [注意]
//   1-indxed
//   Sum()は閉区間で処理する
//   要素数nは2のべき乗でなくても良い
// 以下URLをほぼそのまま持ってきている
// https://www.slideshare.net/hcpc_hokudai/binary-indexed-tree
template <typename T>
struct BIT
{
	const int n;  // 1-indexedでA1～Anまでの数列を扱う
	vector<T> array;  

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
};
