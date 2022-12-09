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
 * x:計算を始めるまでの時間
 * f(x):計算を終えるまでの時間
 * とすると、f(x) = x + P*2^(-2x/3) となる。
 * これを微分すると、
 * f'(x) = 1 - (2/3)*P*2^(-2x/3)*log(2) となり、
 * f'(x) = 0 となるようなxは、
 * x = 3/2 * log2(2/3*P*log(2))
 * となる。
 * 
 * ARC054-B-2.cppより、f'(x)=0となるxが極小値となり、
 * 上記の式変換によりそのようなxをO(1)で解くことができる。
 */

int main(void)
{
	double P;
	cin >> P;

	double x;
	x = 3.0/2.0 * log2(2.0*P/3.0 * log(2.0));

	// 極小値がx<0に存在する場合は、即座に計算を始めるのが最善。
	// x=0に置き換える。
	if(x < 0) x = 0;

	cout << std::fixed << std::setprecision(10);
	cout << x + P * pow(2, -2.0*x/3.0) << endl;

	return 0;
}
