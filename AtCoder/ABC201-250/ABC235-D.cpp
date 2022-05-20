#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC235 https://atcoder.jp/contests/abc235

/*
 * 再帰を用いた深さ優先探索で解く。
 * 
 * 現在の値に対して
 *   a倍するパターン
 *   末尾の数値を移動させるパターン
 * それぞれを再帰で呼び出していく。
 * 
 * 操作で桁数が減ることは無いため、Nより桁数が大きくなったら探索打ち切り。
 * また各値の最小操作回数をminCount[]に保持しておき、探索打ち切りに使う。
 */

ll a, N;
// minCount[i]:数値iにするための操作の最小回数
vector<ll> minCount;

// nの桁数を返す
int digit(ll n)
{
	if(n==0) return 1;
	int count = 0;
	while(n > 0)
	{
		count++;
		n /= 10;
	}

	return count;
}

// nowNum:現在の数値
// nowCount:ここまで何回操作をしたか
void func(ll nowNum, ll nowCount)
{
	// 操作で桁を減らすことはできない
	if(digit(nowNum) > digit(N)) return;

	// もっと少ない操作回数の手順があれば、探索打ち切り
	if(nowCount >= minCount[nowNum])  return;
	minCount[nowNum] = nowCount;  // 最小操作回数の更新

	// 操作成功
	if(nowNum == N)  return;

	// a倍するパターン
	func(nowNum*a, nowCount+1);
	// 末尾の数値を移動させるパターン
	if(nowNum >= 10 && (nowNum%10) != 0)  // 操作が許される場合のみ
	{
		ll tmp;
		// 329 -> 932 のように変化させる
		tmp = nowNum/10 + (nowNum%10)*pow(10, digit(nowNum)-1);
		func(tmp, nowCount+1);
	}

	return;
}

int main(void)
{
	cin >> a >> N;
	// 十分大きい値で初期化
	// 1000000:Nの最大 より少しだけ大きくしておく
	minCount.assign(1000010, INF32);

	func(1, 0);

	if(minCount[N] != INF32)
	{
		cout << minCount[N] << endl;
	}
	else
	{
		cout << -1 << endl;
	}

	return 0;
}
