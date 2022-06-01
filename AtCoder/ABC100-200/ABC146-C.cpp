#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC146 https://atcoder.jp/contests/abc146

/*
 * 二分探索で解く。
 * 
 * 整数Nを買うための金額をfunc(N)としたときに、func(N) < func(N+1)が成り立つ。
 * よって「整数Xは買えるが、X+1は買えない」という境界値が存在する。
 * これを二分探索で解けばよい。
 */

ll A, B;

int digit(ll N)
{
	int d = 0;
	if(N == 0) return 1;
	while(N > 0)
	{
		d++;
		N /= 10;
	}
	return d;
}

// 整数Nを買うための金額を返す
ll cost(ll N)
{
	return A*N + B*digit(N);
}

int main(void)
{
	ll X;
	cin >> A >> B >> X;

	ll left, right;
	left = 1;
	right = 1000000000;  // 10^9
	// 1つも買えない場合、および最大値の10^9まで買える場合を先に処理
	// とはいえ、初期値をleft=0, right=(10^9)+1にするとかで、処理不要にできたかも
	if(X < cost(left))
	{
		cout << "0" << endl;
		return 0;
	}
	if(X >= cost(right))
	{
		cout << right << endl;
		return 0;
	}

	while(right - left > 1)
	{
		ll mid = (left+right)/2;
		if(X >= cost(mid))
		{
			// 買える
			left = mid;
		}
		else
		{
			// 買えない
			right = mid;
		}
	}
	cout << left << endl;

	return 0;
}
