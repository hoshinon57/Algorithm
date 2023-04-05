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

// 「上級者が解くべき過去問精選 100 + 50 問」の問題132
// https://qiita.com/e869120/items/acba3dd8649d913102b5
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_B&lang=ja

/*
 * Binary Indexed Treeにて
 *   特定の要素Aiにxを加算
 *   先頭の要素A1からAiまでの累積和を計算
 * するライブラリを作成して解く。
 * どちらも計算量は、要素数をNとしてO(logN).
 * 
 * [i,j]の要素の累積和を求める場合は、
 * A1～Ajまでの累積和から、A1～A(i-1)までの累積和を減算する
 * ことで実現する。
 */

// 以下URLをほぼそのまま持ってきている
// https://www.slideshare.net/hcpc_hokudai/binary-indexed-tree
template <typename T>
struct BIT
{
	vector<T> array;  
	const int n;  // 配列の要素数(数列の要素数+1)

	BIT(int n_) : array(n_+1, 0), n(n_) {}
	
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
	int n, q;
	cin >> n >> q;
	BIT<int> b(n);

	while(q > 0)
	{
		int c, x, y;
		cin >> c >> x >> y;
		if(c == 0)
		{
			b.Add(x, y);
		}
		else
		{
			cout << b.Sum(x, y) << endl;
		}
		q--;
	}

	return 0;
}
