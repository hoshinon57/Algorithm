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
 * 動的計画法で解く。
 * 
 * red[i] :レベルiの赤い宝石1個からスタートして、最終的に得られるレベル1の青い宝石の数
 * blue[i]:レベルiの青い宝石1個からスタートして、最終的に得られるレベル1の青い宝石の数
 * と定義する。
 * red[1]=0, blue[1]=1となる。
 * また漸化式は以下となる。
 * red[n]  = red[n-1] + blue[n]*X
 * blue[n] = red[n-1] + blue[n-1]*Y
 * 
 * 後は上記をレベルが低い方から順に計算していけばよい。
 * ※red[n],blue[n]ではblue[n]を先に計算必要。red[n]の算出にblue[n]が必要のため。
 */
int main(void)
{
	// 1-indexed
	int i;
	int N, X, Y;
	cin >> N >> X >> Y;

	// red[i] :レベルiの赤い宝石1個からスタートして、最終的に得られるレベル1の青い宝石の数
	// blue[i]:レベルiの青い宝石1個からスタートして、最終的に得られるレベル1の青い宝石の数
	ll red[11] = {0}, blue[11] = {0};
	red[1] = 0;
	blue[1] = 1;

	for(i = 2; i <= N; i++)
	{
		blue[i] = red[i-1] + blue[i-1] * Y;
		red[i] = red[i-1] + blue[i]*X;
	}
	cout << red[N] << endl;

	return 0;
}
