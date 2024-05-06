#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC026 https://atcoder.jp/contests/abc026

/*
 * 二分探索で解く。
 * 
 * f(t)は連続であるので、
 *   f(l)<100 であるl
 *   f(r)>100 であるr
 * があったときに、lとrの間にf(x)=100となるxが必ず存在する。
 * これを二分探索で範囲を狭めていけばよい。
 * 
 * l,rの初期値は、入力A,B,Cによらず f(l)<100, f(r)>100 を満たす必要がある。
 * lについて、f(0)=0なので、l=0にすればよい。
 * rについて、条件が最も厳しくなるのはA=1,B=100である(B*sin()の部分の最小値が-100になるので)。
 * ここからr>200のとき確実にf(r)>100である。
 * よってrは200より大きい適当な値にしておけばよい。
 * 
 * テストケース special_1.txt にてWAが解消できず苦戦した。
 * 出力の桁数を10桁から15桁に増やしたことでACとなった。
 * ->出力する値は t なのに対して、
 *   「誤差が10^(-6)までOK」なのは f(t) である点に注意。
 *   入力A,B,Cによっては、tが10^(-10)変化した程度で許容誤差をオーバーするケースが存在する模様。
 */

double a, b, c;

double func(double t)
{
	double ret;
	ret = a*t + b*sin(c*t*3.141592653589793);
	return ret;
}

int main(void)
{
	cin >> a >> b >> c;

	double l = 0;
	double r = 200+10;  // 200より大きい適当な数
	double mid;  // t
	double ret;  // f(t)
	while(true)
	{
		mid = (l+r)/2;
		ret = func(mid);
		if(abs(ret-100) < 0.0000001) // 10^(-7)
		{
			break;
		}
		else if(ret > 100) r = mid;
		else l = mid;
	}

	// 出力の桁数に注意。ここでハマった。
	// 出力する値は t なのに対して、
	// 「誤差が10^(-6)までOK」なのは f(t) である点に注意。
	// 入力A,B,Cによっては、tが10^(-10)変化した程度で許容誤差をオーバーするケースが存在する模様。
	cout << std::fixed << std::setprecision(15);  // setprecision(10)だとWAだった
	cout << mid << endl;

	return 0;
}
