#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <random>  // ランダムテスト用
using namespace std;

// Binary Indexed Tree(BIT)のメモ

/*
 * [ざっくり概要]
 * 以下のことがO(logN)できるデータ構造。
 *   (1)Sum : iが与えられたとき、累積和A1+A2+...+Aiを計算
 *            [i,j]の区間の累積和を求めるSumもある
 *   (2)Add : iとxが与えられたとき、Aiにxを加算する
 *   (3)Add_Range : l,rが与えられたとき、Al～Arにxを加算する (Range Add Query)
 *   (4)lower_bound : a1+a2+...+ax>=w となるような最小のxを返す (各aiが0以上であることが条件)
 * BIT構造体：(1),(2),(4)に対応
 * BIT_RAQ構造体：(1),(2),(3)に対応
 * 要素数Nに対し、サイズNの配列で実装可能。
 * 
 * 転倒数(反転数)の求め方：Inversion()に記載
 * x番目に小さい要素の求め方：main()に記載
 * 
 * [参考資料]
 * https://www.slideshare.net/hcpc_hokudai/binary-indexed-tree
 * https://algo-logic.info/binary-indexed-tree/
 * 
 * [関連する問題]
 * AOJ DSL_2_B https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_B&lang=ja
 * AOJ DSL_2_E https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_E&lang=ja  Range Add Query
 * AOJ ALDS1_5_D https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_5_D&lang=ja  反転数(転倒数)
 * AOJ 0365 https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=0365&lang=ja
 * ARC033-C
 * ABC244-D
 * ABC221-E 難しかった
 * ABC153-F
 * ABC190-F 転倒数
 * ABC261-F 転倒数
 * ABC296-F 転倒数
 * ABC396-F 転倒数(最初に1回求めるだけ)
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

// Binary Indexed Treeを用いて以下(1)～(3)をO(logN)で計算する
//   (1)Sum : iが与えられたとき、累積和A1+A2+...+Aiを計算
//            [i,j]の区間の累積和を求めるSumもある
//   (2)Add : iとxが与えられたとき、Aiにxを加算する
//   (3)Add_Range : l,rが与えられたとき、Al～Arにxを加算する (Range Add Query)
// [注意]
//   1-indexed
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

// ランダムテストにて、BITのlower_bound()の計算結果と、愚直に計算した結果が一致するかテストする
void BIT_random_test(void)
{
	int i, j;
	std::random_device rd;
	std::mt19937 rng(rd());

	cout << "(BIT)random test:start" << endl;
	for(i = 0; i < 100; i++)
	{
		int n = std::uniform_int_distribution<int>(1, 100000)(rng);
		BIT<long long> bit(n);
		vector<long long> a(n+1);
		for(j = 0; j < 200; j++)
		{
			int query = std::uniform_int_distribution<int>(0, 3)(rng);
			if(query == 0)  // Add
			{
				int idx = std::uniform_int_distribution<int>(1, n)(rng);
				int x = std::uniform_int_distribution<int>(0, 100)(rng);
				bit.Add(idx, x);
				a[idx] += x;
//				cout << "query==0, " << "idx=" << idx << ", x=" << x << endl;
			}
			else if(query == 1)  // [1,x]の累積和
			{
				long long s1 = 0, s2 = 0;
				int idx = std::uniform_int_distribution<int>(1, n)(rng);
				s1 = bit.Sum(idx);
				for(int k = 1; k <= idx; k++)
				{
					s2 += a[k];
				}
				assert(s1 == s2);
//				cout << "query==1, " << "idx=" << idx << endl;
			}
			else if(query == 2)  // 先頭からの累積和がxを超えるindexを求める
			{
				int idx1, idx2;
				long long sum = std::uniform_int_distribution<int>(1, 1000)(rng);
				idx1 = bit.lower_bound(sum);
				long long tmp = 0;
				for(idx2 = 1; idx2 <= n; idx2++)
				{
					tmp += a[idx2];
					if(tmp >= sum) break;
				}
				assert(idx1 == idx2);
//				cout << "query==2, " << "sum=" << sum << endl;
			}
		}
	}
	cout << "(BIT)random test:OK!" << endl;
}

// ランダムテストにて、BIT_RAQの計算結果と、愚直に計算した結果が一致するかテストする
void RAQ_random_test(void)
{
	int i, j;
	std::random_device rd;
	std::mt19937 rng(rd());

	cout << "(RAQ)random test:start" << endl;
	for(i = 0; i < 100; i++)
	{
		int n = std::uniform_int_distribution<int>(1, 100000)(rng);
		BIT_RAQ<long long> br(n);
		vector<long long> a(n+1);
		for(j = 0; j < 100; j++)
		{
			int query = std::uniform_int_distribution<int>(0, 3)(rng);
			if(query == 0)
			{
				int idx = std::uniform_int_distribution<int>(1, n)(rng);
				int x = std::uniform_int_distribution<int>(-100, 100)(rng);
				br.Add(idx, x);
				a[idx] += x;
//				cout << "query == 0  " << "idx=" << idx << ", x=" << x << endl;
			}
			else if(query == 1)
			{
				int l = std::uniform_int_distribution<int>(1, n)(rng);
				int r = std::uniform_int_distribution<int>(1, n)(rng);
				int x = std::uniform_int_distribution<int>(-100, 100)(rng);
				if(l > r) swap(l, r);
				br.Add_Range(l, r, x);
				for(int k = l; k <= r; k++)
				{
					a[k] += x;
				}
//				cout << "query == 1  " << "l=" << l << ", r=" << r << ", x=" << x << endl;
			}
			else if(query == 2)
			{
				long long s1 = 0, s2 = 0;
				int idx = std::uniform_int_distribution<int>(1, n)(rng);
				s1 = br.Sum(idx);
				for(int k = 1; k <= idx; k++)
				{
					s2 += a[k];
				}
//				cout << "query == 2  " << "s1=" << s1 << ", s2=" << s2 << endl;
				assert(s1 == s2);
			}
			else
			{
				long long s1 = 0, s2 = 0;
				int l = std::uniform_int_distribution<int>(1, n)(rng);
				int r = std::uniform_int_distribution<int>(1, n)(rng);
				if(l > r) swap(l, r);
				s1 = br.Sum(l, r);
				for(int k = l; k <= r; k++)
				{
					s2 += a[k];
				}
//				cout << "query == 3  " << "l=" << l << ", r=" << r << ", s1=" << s1 << ", s2=" << s2 << endl;
				assert(s1 == s2);
			}
		}
	}
	cout << "(RAQ)random test:OK!" << endl;
}

// 転倒数(反転数)の求め方メモ
void Inversion(void)
{
	/*
	転倒数とバブルソートの交換回数は一致する。
	要素(隣同士でなくてもよい)を交換すると、転倒数の偶奇が切り替わる。
	*/
	/*
	配列a[i]が与えられたとき、転倒数は以下のアルゴリズムで求められる。

	Bit<int> bit(M+1);  // M:要素a[]の最大値 +1は1-indexedのため
	for(i = 0; i < n; i++)  // n:a[]の要素数
	{
 		answer += i - bit.Sum(a[i]);
		bit.Add(a[i], 1);
	}

	例えばa[i]=3のとき、a[]が昇順に並んでいればa[0]～a[i-1]に3以下の値がi個あることになる。
	つまり i-bit.Sum(3) にて、転倒数が求められる。
	BITの各操作がO(logN)なので、計算量は全体でO(NlogN).

	ただしBITは1-indexedのため、a[i]の値も1以上にする必要がある。
	またa[i]の値が大きくなる場合、座標圧縮する必要がある。
	*/
}

int main(void)
{
	BIT<int> b(7);  // 要素数は2のべき乗でなくても良い
	b.Add(1, 3);  // 1-indxed
	b.Add(3, 10);
	assert(b.Sum(1, 3) == 13);
	assert(b.Sum(1, 7) == 13);
	assert(b.lower_bound(3) == 1);
	assert(b.lower_bound(4) == 3);
	b.Add(7, 10);  // b(7)で定義したので、A7まで使える
	b.Add(7, 10);
	assert(b.Sum(2, 7) == 30);
	assert(b.lower_bound(0) == 1);  // lower_bound()を使う場合、各要素は0以上の前提
	assert(b.lower_bound(14) == 7);
	assert(b.lower_bound(33) == 7);
	assert(b.lower_bound(34) == 8);  // 末尾の次

	BIT_random_test();

	BIT_RAQ<int> br(13);
	br.Add(1, 2);
	br.Add(13, -2);
	assert(br.Sum(13) == 0);
	assert(br.Sum(5) == 2);
	assert(br.Sum(10, 13) == -2);
	br.Add_Range(2, 8, 1);
	br.Add_Range(7, 13, 2);
	assert(br.Sum(1, 6) == 7);
	assert(br.Sum(10, 13) == 6);
	assert(br.Sum(1, 13) == 21);
	assert(br.Sum(1) == 2);
	assert(br.Sum(5) == 6);

	RAQ_random_test();

	/*
	ある集合を考えたときに、x番目に小さい要素は以下の手順で求めることができる。

	集合に含まれる値の最大値をNとして、BIT<int> b(N); で定義する。
	  b.Add(a, 1);	// 集合にaを追加
	  b.Add(a, -1);	// 集合からaを削除
	  b.Sum(a);		// aが何番目に小さいか
	  b.lower_bound(x);	// x番目に小さい要素は何か
	集合に含まれる値に1を立てていくイメージ。
	*/

	return 0;
}
