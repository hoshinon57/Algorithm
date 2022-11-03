#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題96より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc139/tasks/abc139_d

/*
 * 例えばN=4のとき、（いったん数列Pが存在するかは無視した上で、）
 * どういったMが最大になるかと考えると、 M={0,1,2,3} が理論上の最大になる。
 * 
 * ではこうなるような数列Pが存在するかを考えると、 P{4,1,2,3} が条件を満たす。
 * 
 * ということで、 {1,2,3,...,N} を1つ右にずらした数列が最適なPとなる。
 * Mの総和は 0+1+2+...+(N-1) の等差数列となるので、 (N-1)*(N-1+1)/2 が答となる。
 */
int main(void)
{
	ll N;
	cin >> N;

	// 0, 1, 2, ..., N-1 までの和
	cout << (N-1) * (N-1+1) / 2 << endl;  // Nは最大で10^9であるため、N*Nの時点でオーバーフローはしない

	return 0;
}
