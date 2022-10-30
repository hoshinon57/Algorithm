#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC275 https://atcoder.jp/contests/abc275

/*
 * メモ化再帰で解けばよい。
 * 計算量はO(log^2(N))となる。
 */

// 再帰のメモ用 func(k)の答がメモ済みなら要素有りで、umap[k] = func(k)となる
unordered_map<ll,ll> umap;

// メモ化再帰を用いて、func(k)を返す
ll func(ll k)
{
	if(k == 0) return 1;
	if(umap.count(k) > 0) return umap[k];  // メモ済み

	umap[k] = func(k/2) + func(k/3);

	return umap[k];
}

int main(void)
{
	ll N;
	cin >> N;

	cout << func(N) << endl;

	return 0;
}
