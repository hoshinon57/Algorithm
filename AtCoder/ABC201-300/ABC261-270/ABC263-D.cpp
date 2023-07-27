#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC263 https://atcoder.jp/contests/abc263

/*
 * まず、置き換え操作において、Lで置き換える範囲とRで置き換える範囲は被らせる必要はない。
 * そのため、仮想的な区切りを用いて
 * ・区切りより前になるようxを選ぶ場合の、総和が最も小さくなるx
 * ・区切りより後になるようyを選ぶ場合の、総和が最も小さくなるy
 * を求める。
 * そして区切りについて全探索すればよい。
 * 
 * x_merit[i]:xにiを選んだとき、どれだけメリットがあるか（総和が小さくなるか）
 * x_merit_min[i]:x_merit[1]～x_merit[i]までの最小値
 * とし(yも同様に定義する)、事前に計算しておく。
 * A[i]とA[i+1]間に区切りを置いた場合、 x_merit_min[i] + y_merit_min[i+1] が最適なx,yの選び方となる。
 * 全ての区切り方のうち、最小値が答となる。
 */
int main(void)
{
	// 1-indexed
	ll i;
	ll N, L, R;
	cin >> N >> L >> R;
	vector<ll> a(N+1);
	for(i = 1; i <= N; i++)
	{
		cin >> a[i];
	}
	vector<ll> x_merit(N+1);  // x_merit[i] xにiを選んだとき、どれだけ改善されるか（小さくなるか）
	vector<ll> x_merit_min(N+1);  // x_merit_min[i]  x_merit[1]～x_merit[i]までの最小値
	ll a_sum = 0;
	for(i = 1; i <= N; i++)  // x=iのとき、総和がどれだけ小さくなるか
	{
		a_sum += a[i];
		x_merit[i] = i*L - a_sum;
		x_merit_min[i] = min(x_merit_min[i-1], x_merit[i]);
	}

	vector<ll> y_merit(N+2);  // 楽に計算するため[N+1]を使いたい よってサイズをN+2にしておく
	vector<ll> y_merit_min(N+2);
	a_sum = 0;
	for(i = N; i >= 1; i--)
	{
		a_sum += a[i];
		y_merit[i] = (N-i+1)*R - a_sum;
		y_merit_min[i] = min(y_merit_min[i+1], y_merit[i]);
	}

	ll min_tmp = INF64;
	for(i = 0; i <= N; i++)
	{
		ll tmp;
		tmp = x_merit_min[i] + y_merit_min[i+1];
		min_tmp = min(min_tmp, tmp);
	}
	cout << a_sum + min_tmp << endl;

	return 0;
}
