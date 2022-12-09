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
 * 三分探索で解く。
 * 
 * x:計算を始めるまでの時間
 * y:計算を終えるまでの時間
 * とすると、y=x+P*2^(-2x/3) となる。
 * これを https://www.geogebra.org/graphing?lang=ja などで調べると、下に凸の関数となる。
 * 
 * よってL=0, R=INF64を初期値として、三分探索で最小値を求める。
 * グラフ全体の極小値が0未満の部分にあるようなPも存在する(つまりx=0が答)が、
 * その場合もL=R=0.0にて三分探索が完了するため、問題はない。
 */

int main(void)
{
	double P;
	cin >> P;
	double l, r;
	l = 0.0;
	r = (double)INF64;

	// m:計算を始めるまでの時間
	// 戻り値:計算が終わるまでの時間
	auto calc = [&](double m) -> double
	{
		return m + P * pow(2.0, -2.0*m/3.0);
	};

	while(true)
	{
		if(r-l < 0.0000000001) break;  // 10^(-9)

		double mid1, mid2;
		mid1 = (l*2+r)/3;  // 三分探索 左側
		mid2 = (l+r*2)/3;  // 三分探索 右側
		if(calc(mid1) > calc(mid2))
		{
			l = mid1;
		}
		else
		{
			r = mid2;
		}
//		cout << std::fixed << std::setprecision(10);
//		cout << "l=" << l << " r=" << r << endl;
	}

	// l, rは「計算を始めるまでの時間」であり、
	// 求めたいのは「計算が終わるまでの時間」である点に注意
	cout << std::fixed << std::setprecision(10);
	cout << calc(l) << endl;

	return 0;
}
