#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題22より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/arc054/tasks/arc054_b

/*
 * 微分して二分探索で解く。
 * 
 * x:計算を始めるまでの時間
 * y:計算を終えるまでの時間
 * とすると、y=x+P*2^(-2x/3) となる。
 * 
 * 微分すると、y'=1-(2/3)P*2^(-2x/3)*log(2) となる。
 * これはどうも単調増加関数っぽくて、y'=0 となるようなxが極小値となる。
 * よってこれを二分探索で解く。
 * 
 * ただしy'=0となるxが0未満のとき、極小値も0未満となる。
 * このときは即座に計算を始めるのが最善。
 * このケースだけは事前にチェックして出力している。
 */

int main(void)
{
	double P;
	cin >> P;

	// x:計算を始めるまでの時間
	// 戻り値:f'(x)の値
	auto calc_bibun = [&](double x) -> double
	{
		return 1 - 2.0*P/3.0 * pow(2, -2.0*x/3.0) * log(2.0);
	};

	// m:計算を始めるまでの時間
	// 戻り値:f(x)の値 つまり計算を終えるまでの時間
	auto calc = [&](double m) -> double
	{
		return m + P * pow(2.0, -2.0*m/3.0);
	};

	// f'(0)の時点で0より大きい場合、y=f(x)の極小値はx<0に存在する。
	// よって即座に計算を始めるのが最善。
	if(calc_bibun(0.0) > 0)
	{
		cout << std::fixed << std::setprecision(10);
		cout << P << endl;
		return 0;
	}

	// f'(x)は単調増加関数のため、f'(x)=0となるようなxがf(x)の極小値となる。
	// 二分探索でそのようなxを求める。
	double l, r;
	l = 0.0;
	r = (double)INF64;
	while(true)
	{
		if(r-l < 0.000000001) break;  // 10^(-9)

		double mid = (r+l)/2;
		if(calc_bibun(mid) < 0) l = mid;
		else r = mid;
	}

	// 求めるのはl,rではなく、計算が終わるまでの時間
	cout << std::fixed << std::setprecision(10);
	cout << calc(l) << endl;

	return 0;
}
