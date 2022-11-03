#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題95より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc149/tasks/abc149_b

/*
 * 解いた後、以下の解説記事の実装の方がずっと分かりやすく、実装しなおした。
 * https://blog.hamayanhamayan.com/entry/2020/01/01/024113
 * 
 * 高橋君のクッキーについて、何枚食べるかは min(A, K) となる。
 * AとKともに、この枚数ぶん引く。
 * 青木君のクッキーについても同様。
 * 
 * 最後にA,Bを出力すればよい。
 */

int main(void)
{
	ll A, B, K;
	cin >> A >> B >> K;

	ll tmp;  // 食べる枚数
	tmp = min(A, K);
	A -= tmp;
	K -= tmp;

	tmp = min(B, K);
	B -= tmp;
	K -= tmp;

	cout << A << " " << B << endl;

	return 0;
}
