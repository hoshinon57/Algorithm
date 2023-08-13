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

// ABC153 https://atcoder.jp/contests/abc153

/*
 * メモ化再帰で解く。(通常のDPで解けるが、練習としてこちらを選んだ)
 * 
 * func(h)を
 *   モンスターの体力がhのとき、0以下にするまでに必要な魔力の最小値を返す
 * として定義する。
 * func(H)がそのまま答。
 * 
 * func()の中身は、
 *   h<=0なら0を返す (もう撃破済みなので)
 *   メモ化済みならそれを返す
 *   N種類の魔法それぞれについて、 func(h-a[i])+b[i] の最小値が答(※)
 * となる。
 * (※)体力が(h-a[i])のときに必要な魔力にb[i]を加えたもの、という考え方
 * 
 * 計算量はO(NH).
 */

int N;
vector<int> a, b;
vector<int> memo;

int func(int h)
{
	if(h <= 0) return 0;
	if(memo[h] != -1) return memo[h];

	int i;
	int m = INF32;
	for(i = 0; i < N; i++)
	{
		m = min(m, func(h-a[i]) + b[i]);
	}
	return (memo[h] = m);
}

int main(void)
{
	int i;
	int H; cin >> H >> N;
	a.resize(N);
	b.resize(N);
	memo.resize(H+1, -1);  // -1はメモ未
	for(i = 0; i < N; i++)
	{
		cin >> a[i] >> b[i];
	}
	cout << func(H) << endl;

	return 0;
}
