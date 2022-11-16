#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const int MOD = 10000;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題53
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_1_D&lang=ja

/*
 * 動的計画法(最長増加部分列)で解く。
 * 初見では解けず、以下URLの記事を見て理解してから実装した。
 *   https://qiita.com/python_walker/items/d1e2be789f6e7a0851e5
 * 
 * 計算量はO(NlogN).
 */
int main(void)
{
	int i;
	int n;
	cin >> n;
	vector<int> a(n);
	for(i = 0; i < n; i++) cin >> a[i];
	vector<int> dp;

	for(i = 0; i < n; i++)
	{
		// dp[]の中から、a[i]以上となる最初の要素を探す
		auto itr = lower_bound(dp.begin(), dp.end(), a[i]);
		if(itr != dp.end())  // 見つかった場合は値を置き換え
			*itr = a[i];
		else  // 見つからなかった場合は、末尾に追加
			dp.push_back(a[i]);
	}
	cout << dp.size() << endl;  // dpサイズが最長増加部分列の長さになる

	return 0;
}
