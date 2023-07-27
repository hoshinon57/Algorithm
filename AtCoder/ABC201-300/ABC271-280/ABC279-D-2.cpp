#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC279 https://atcoder.jp/contests/abc279

/*
 * 微分して解く。
 * 
 * 操作回数をcとしたときの、かかる時間の式は以下となる。
 *   f(c) = A/(√(c+1)) + B*c
 * これを微分すると以下になる。
 *   f'(c) = -A/(2(c+1)√(c+1)) + B
 * 下に凸の関数であり、極値が求める最小値となる。
 * 
 * f'(X)=0となるようなXを求めると、X=(A/2B)^(2/3) -1 となる。
 * Xは小数のため、これを切り捨て、切り上げした値をそれぞれx1,x2とすると、
 * x1,x2のどちらかが最小値となる。
 * ※Xの地点からグラフは上がり始めるが、f(x1)とf(x2)のどちらが大きいかは計算しないと分からないため。
 * 
 * 公式解説では「数値誤差等の問題もあるため、余裕を持って前後 ±5 程度の範囲を調べるとよい」とのこと。
 * 今回は周囲を探索せずともACとなったが、ノウハウとして覚えておいた方が良さそう。
 */

int main(void)
{
	ll A, B;
	cin >> A >> B;

	double x;  // 操作回数に小数を含めて考えたときの、時間が最小値（極値）となる回数
	x = pow(A/(B*2), 2.0/3);
	ll x1, x2;
	x1 = (ll)x;  // xの切り捨て
	x2 = x1+1;  // xの切り上げ

	auto calc = [&](ll c) -> double
	{
		double ret;
		ret = (double)A / sqrt(c+1) + (double)c * B;
		return ret;
	};

	// x1,x2のうち小さい方が答となる
	cout << std::fixed << std::setprecision(10);
	cout << min(calc(x1), calc(x2)) << endl;

	return 0;
}
