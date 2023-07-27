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
 * 再帰関数で解く。(赤と青を両方とも計算するパターン)
 * 
 * 赤い宝石用の関数func_Red(n)を、
 *   レベル(n)の赤い宝石1個から始めたときに、
 *   最終的に得られるレベル1の赤い宝石の数と、レベル1の青い宝石の数を返す
 * と定義する。
 * func_Blue(n)も同様。
 * 
 * あとはpair<ll, ll>をそれぞれ赤い宝石、青い宝石と定めて計算すればよい。
 * ABC260-C.cppも参照。
 */

pair<ll, ll> func_Blue(int n);
int N, X, Y;

// レベル(n)の赤い宝石1個から始めたときに、
// 最終的に得られるレベル1の赤い宝石の数と、レベル1の青い宝石の数を返す
pair<ll, ll> func_Red(int n)
{
	// ベースケース
	if(n == 1)
	{
		return make_pair(1, 0);
	}

	pair<ll, ll> tmp1, tmp2;
	tmp1 = func_Red(n-1);
	tmp2 = func_Blue(n);
	// 青い宝石はX個に変換できる
	tmp2.first *= X;
	tmp2.second *= X;

	return make_pair(tmp1.first+tmp2.first, tmp1.second+tmp2.second);
}

pair<ll, ll> func_Blue(int n)
{
	// ベースケース
	if(n == 1)
	{
		return make_pair(0, 1);
	}

	pair<ll, ll> tmp1, tmp2;
	tmp1 = func_Red(n-1);
	tmp2 = func_Blue(n-1);
	// 青い宝石はY個に変換できる
	tmp2.first *= Y;
	tmp2.second *= Y;

	return make_pair(tmp1.first+tmp2.first, tmp1.second+tmp2.second);
}

int main(void)
{
	// 1-indexed
	int i;
	cin >> N >> X >> Y;

	pair<ll, ll> jewel = func_Red(N);
//	cout << jewel.first << " " << jewel.second << endl;
	cout << jewel.second << endl;

	return 0;
}
