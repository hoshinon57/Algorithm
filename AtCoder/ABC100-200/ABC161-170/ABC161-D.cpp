#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC161 https://atcoder.jp/contests/abc161

/*
 * 再帰を用いた深さ優先探索で解く。
 * 
 * ある数n(末尾の値がa)があったときに、
 *   n*10+(a-1)
 *   n*10
 *   n*10+(a+1)
 * をそれぞれ再帰で呼び出していく。
 * 
 * main()からの呼び出しは、func(1)～func(9)まで個別に呼び出してやればよい。
 */

vector<ll> lunlun;

void func(ll n)
{
	// K最大値での回答が3234566667より、打ち切り条件を設定（かなり緩め）
	if(n > 10000000000)  return;

	// nはルンルン数
	lunlun.push_back(n);
	ll a = n%10;
	// 末尾に数値を付与して再帰
	// 1減らす、1増やすときは条件注意
	if(a > 0) func(n*10+(a-1));
	func(n*10+a);
	if(a < 9) func(n*10+(a+1));

	return;
}

int main(void)
{
	int K;
	cin >> K;

	for(int i = 1; i <= 9; i++) func(i);

	std::sort(lunlun.begin(), lunlun.end());
	cout << lunlun[K-1] << endl;

	return 0;
}
