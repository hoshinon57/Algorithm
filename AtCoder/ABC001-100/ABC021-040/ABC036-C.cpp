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

// 「分野別 初中級者が解くべき過去問精選 100問」の問題102より
// https://qiita.com/e869120/items/acba3dd8649d913102b5
// 
// https://atcoder.jp/contests/abc036/tasks/abc036_c

/*
 * 座標圧縮で解く。
 * 
 * (1)元データをソート＆重複を削除したもの
 * (2)元データ
 * の2種類を用意しておき、
 * (2)の各値について、(1)の何番目にあるかをlower_bound()で求めればよい。
 * 
 * 計算量はO(NlogN).
 */

int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<int> a(N);
	for(i = 0; i < N; i++) cin >> a[i];
	auto a_org = a;

	// 元データをソートし、重複を削除する
	sort(a.begin(), a.end());
	a.erase(unique(a.begin(), a.end()), a.end());

	for(auto &e : a_org)
	{
		int c;
		c = lower_bound(a.begin(), a.end(), e) - a.begin();
		cout << c << endl;
	}

	return 0;
}
