#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC252 https://atcoder.jp/contests/abc252

/*
 * jを固定して全探索する。
 * ※コンテスト中はiを固定して全探索していたが、これだとjについてもループが必要となりTLEだった。
 * ※i<j<kのような3変数がある場合、真ん中を固定すると解きやすい。
 * 
 * まず、数列Aは昇順にソートしても問題ない。
 * jを固定して考えたときに、
 *   A(i)として有効なもの：A(j)よりも小さい数
 *   A(k)として有効なもの：A(k)よりも大きい数
 * となり、2つの積が条件を満たす組み合わせの数となる。
 * 後はjについて全探索すればよい。
 */
int main(void)
{
	// 0-indexed
	int i, j;
	int N;
	cin >> N;

	vector<int> a(N);
	for(i = 0; i < N; i++) cin >> a[i];
	sort(a.begin(), a.end());  // 昇順

	ll answer = 0;
	for(j = 1; j < N-1; j++)
	{
		ll num_i = lower_bound(a.begin(), a.end(), a[j]) - a.begin();
		ll num_k = a.end() - upper_bound(a.begin(), a.end(), a[j]);
		answer += num_i * num_k;
	}

	cout << answer << endl;

	return 0;
}
