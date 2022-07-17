#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC260 https://atcoder.jp/contests/abc260

/*
 * 再帰関数で解く。
 * 
 * 赤い宝石用の関数func_Red(level)を、
 *   レベル"level"の赤い宝石1個からスタートして、最終的に得られるレベル1の青い宝石の数を返す
 * と定義する。
 * func_Blue(level)も同様。
 * ※「1個からスタート」「レベル1の青い宝石の数を返す」がポイント。
 * 　ここの定義を上手く整理できず、コンテスト中に相当苦戦してしまった。
 * 
 * func_Red(1)=0, func_Blue(1)=1となる。
 * また
 * func_Red(2) =func_Red(1)+func_Blue(2)*X
 * func_Blue(2)=func_Red(1)+func_Blue(1)*Y
 * func_Red(n) =func_Red(n-1)+func_Blue(n)*X
 * func_Blue(n)=func_Red(n-1)+func_Blue(n-1)*Y
 * となる。
 * 後はこれを実装してやればよい。
 */

ll func_Red(int level);
ll func_Blue(int level);

int N, X, Y;

// レベル"level"の赤い宝石1個からスタートして、最終的に得られるレベル1の青い宝石の数を返す
ll func_Red(int level)
{
	// レベル1の赤い宝石1個はもう変換できない 
	// 戻り値は青い宝石の数なので、0を返す
	if(level == 1) return 0;

#if 0
	// [参考]ベースケースを2とすることも可能。この場合、main()側でN=1なら0を出力する処理が必要。
	if(level == 2) return X*Y;
#endif

	return func_Red(level-1) + func_Blue(level) * X;
}

// レベル"level"の青い宝石1個からスタートして、最終的に得られるレベル1の青い宝石の数を返す
ll func_Blue(int level)
{
	// レベル1の青い宝石1個はもう変換できない
	// 戻り値は青い宝石の数なので、1を返す
	if(level == 1) return 1;

#if 0
	// [参考]ベースケースを2とすることも可能。この場合、main()側でN=1なら0を出力する処理が必要。
	if(level == 2) return Y;
#endif

	return func_Red(level-1) + func_Blue(level-1) * Y;
}

int main(void)
{
	cin >> N >> X >> Y;
	cout << func_Red(N) << endl;

	return 0;
}
