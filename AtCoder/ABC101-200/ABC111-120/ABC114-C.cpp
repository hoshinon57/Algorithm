#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC114 https://atcoder.jp/contests/abc114

/*
 * 再帰を用いた全探索で解く。
 * func()にてnumが753数かを確認しつつ、
 * numの末尾に3 or 5 or 7を付与した値で再帰する。
 * 
 * 引数に7,5,3が存在するかをbitで持たせることで、
 * 753数の確認を簡易にする。
 */

ll N;
ll count_753 = 0;
// num:753数か確認したい値
// flag:ここまでの再帰で、7,5,3が存在するかをbitで持たせている
void func(ll num, int flag)
{
	if(num > N) return;

	// 753数か確認
	if(flag == 0b0111) count_753++;

	// 再帰
	func(num*10+3, flag | 0b0001);
	func(num*10+5, flag | 0b0010);
	func(num*10+7, flag | 0b0100);

	return;
}

int main(void)
{
	cin >> N;

	func(0, 0);

	cout << count_753 << endl;

	return 0;
}
