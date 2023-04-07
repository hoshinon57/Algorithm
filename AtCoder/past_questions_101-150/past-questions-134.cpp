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

// 「上級者が解くべき過去問精選 100 + 50 問」の問題134
// https://qiita.com/e869120/items/acba3dd8649d913102b5
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_5_D&lang=ja

/*
 * Binary Indexed Treeと座標圧縮を用いて解く。
 * 
 * 配列aが与えられたとき、転倒数(反転数)は以下のアルゴリズムで求められる。
 * (1-indexedに注意)
 * for(i = 0; i < n; i++)
 * {
 *   answer += i - bit.Sum(a[i]);
 *   bit.Add(a[i], 1);
 * }
 * 例えばa[i]=3のとき、昇順に並んでいればa[0]～a[i-1]に3以下の値がi個あることになる。
 * つまり i-bit.Sum(3) にて、転倒数が求められる。
 * 
 * ただしこの場合、bitの要素数はa[i]の最大値が必要。
 * 本問題では10^9となり、メモリ制限を超過する。
 * 一方でa[i]の個数は最大で2*10^5なので、座標圧縮すればよい。
 * 
 * 座標圧縮は以下URLを参考に実装した。
 *   https://drken1215.hatenablog.com/entry/2021/08/09/235400
 *   https://algo-logic.info/coordinate-compress/
 */

// Binary Indexed Treeを用いて以下(1)(2)をO(logN)で計算する
//   (1)iが与えられたとき、累積和A1+A2+...+Aiを計算
//   (2)iとxが与えられたとき、Aiにxを加算する
// [注意]
//   1-indexed
//   Sum()は閉区間で処理する
//   要素数nは2のべき乗でなくても良い
// 以下URLをほぼそのまま持ってきている
// https://www.slideshare.net/hcpc_hokudai/binary-indexed-tree
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
};

int main(void)
{
	int i;
	int n;
	cin >> n;
	vector<int> a(n), a_comp(n);
	for(i = 0; i < n; i++)
	{
		cin >> a[i];
		a_comp[i] = a[i];
	}

	// 座標圧縮
	sort(a.begin(), a.end());
	a.erase(unique(a.begin(), a.end()), a.end());
	for(i = 0; i < n; i++)
	{
		a_comp[i] = lower_bound(a.begin(), a.end(), a_comp[i]) - a.begin();
		a_comp[i]++;  // 1-indexed
	}

	// 転倒数を求める
	ll answer = 0;
	BIT<int> bit(n);
	for(i = 0; i < n; i++)
	{
		answer += i - bit.Sum(a_comp[i]);
		bit.Add(a_comp[i], 1);
	}
	cout << answer << endl;

	return 0;
}
