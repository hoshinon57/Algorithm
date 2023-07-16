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

// ABC234 https://atcoder.jp/contests/abc234

/*
 * 以下の関数check(start, diff)を実装する。
 *   最上位の桁の値がstart, 桁ごとの差がdiffのときに、X以上の等差数を探索する
 * 最初の桁をstartとし、以降は+diffごと変えながら桁を足していけばよい。(適切に終了判定は必要)
 * 
 * 後はこの関数を
 *   start:1～9
 *   diff:-9～0～+9
 * の組み合わせで呼び出して全探索すればよい。
 * check()は最大でも17回ほどループすれば終わるので、十分間に合う。
 */

ll X;
ll answer;

// 最上位の桁の値がstart, 桁ごとの差がdiffのときに、
// X以上の等差数を探索する
void check(ll start, ll diff)
{
	ll num = 0;
	while(num < X)
	{
		num = num*10 + start;
		start += diff;
		if(start < 0 || start > 9) break;  // 桁が0～9の範囲外になれば終了
	}
	
	if(num >= X)  // X以上にならずにwhileが終わるケースもあるため、判定は必要
	{
		answer = min(answer, num);
	}
}

int main(void)
{
	int i, j;
	cin >> X;
	answer = INF64;
	for(i = 1; i <= 9; i++)  // スタート 1～9
	{
		for(j = -9; j <= 9; j++)  // 差 -9～0～+9
		{
			check(i, j);
		}
	}
	cout << answer << endl;

	return 0;
}
