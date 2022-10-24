#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC274 https://atcoder.jp/contests/abc274

/*
 * 動的計画法(っぽい方法)で解く。
 * 
 * p2=(A1, 0)であること、線分のなす角が90度であることから、
 * 原点p1をスタートとして ヨコ→タテ→ヨコ→タテ→… と進んでいく。
 * よって
 *   横方向の移動の総和をxにできるか
 *   縦方向の移動の総和をyにできるか
 * という問題となる。
 * 
 * 数列Aを1つおきに見ていって、
 * A[i+2]:A[i]が取りうる値それぞれについて、±A[i+2]したものの集合
 * を計算していけばよい。
 * これをsetを用いて実装した。
 * 
 * 計算量は公式解説のO(N^2*Amax)に加え、set操作がO(logN)のため
 * O(N^2*Amax*logN)になるのかな…と思う。
 * 最大でN=10^3, A=10のためTLEになりそうだが、setの要素数がそれほど多くないからか、
 * 140ms程度でかなり余裕があった。
 * ※unordered_setに置き換えた場合、190ms程度で逆に遅くなった。
 */
int main(void)
{
	// 0-indexed
	int i;
	int N, x, y;
	cin >> N >> x >> y;
	vector<int> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
	}

	// ±a[0]±a[2]±a[4]±...±a[N or N-1] の集合
	// 0-indexedにしているので、入力xがこの集合に含まれるかを判定する
	set<int> s_even;
	s_even.insert(a[0]);  // 最初は必ず右方向
	for(i = 2; i < N; i+=2)  // a[0]を事前に挿入したため、i=2スタート
	{
		set<int>new_s_even;
		for(auto &e : s_even)
		{
			new_s_even.insert(e+a[i]);
			new_s_even.insert(e-a[i]);
		}
		swap(s_even, new_s_even);
	}

	// ±a[1]±a[3]±a[5]±...±a[N or N-1] の集合
	set<int> s_odd;
	s_odd.insert(0);
	for(i = 1; i < N; i+=2)
	{
		set<int> new_s_odd;
		for(auto &e : s_odd)
		{
			new_s_odd.insert(e+a[i]);
			new_s_odd.insert(e-a[i]);
		}
		swap(s_odd, new_s_odd);
	}

	if( (s_even.count(x) > 0) && (s_odd.count(y) > 0) )
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
