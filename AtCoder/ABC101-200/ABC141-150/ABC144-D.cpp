#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題91より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc144

/*
 * 水量が容器の半分以下かどうか、で処理を分ける。
 * 
 * (1)容器の半分以下の場合
 * 水によってできる形は三角柱となる。
 * 三角形部分の斜辺を除く2辺の長さは、片方がbで、もう片方が水量に依存し、zとする。
 * 三角柱の体積は (bz/2)*a となり、これがxに等しいことから、 z=2x/ab となる。
 * tanΘ=b/z となるΘが答であり、atan(b/z)を求めればよい。
 * 今回はatan2()を使用した。
 * 
 * (2)容器の半分より多い場合
 * こちらは「水が無い部分でできる形」を考える。同じく三角柱になる。
 * 同じように計算していくと、
 * tanΘ=(2(a*a*b-x)/a*a*a) となるΘが答であり、atan(2(a*a*b-x)/a*a*a)を求めればよい。
 * 
 * 計算量はO(1).
 */

int main(void)
{
	int a, b, x;
	cin >> a >> b >> x;

	double answer;
	if(x*2 <= a*a*b)  // 水量が半分以下の場合
	{
		answer = atan2((double)a*b*b, (double)x*2)*180/M_PI;
	}
	else  // 水量が半分以上の場合
	{
		answer = atan2((double)2*(a*a*b-x), (double)a*a*a)*180/M_PI;
	}
	cout << std::fixed << std::setprecision(10);
	cout << answer << endl;

	return 0;
}
