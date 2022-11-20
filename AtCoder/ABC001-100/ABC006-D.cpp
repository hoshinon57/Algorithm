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

// 「分野別 初中級者が解くべき過去問精選 100問」の問題54
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc006

/*
 * 動的計画法(最長増加部分列)で解く。
 * 
 * まず、初期状態のカードにおける最長増加部分列を求める。
 * 例えば {1,3,5,2,4,6} であれば {1,2,4,6}.
 * 操作としては、ここに3と5を挿入すればカード全体を昇順に並べることができ、これが最小手順となる。
 * よって (カードの枚数)-(最長増加部分列) が答となる。
 * 
 * 計算量はO(NlogN).
 */
int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<int> dp;
	for(i = 0; i < N; i++)
	{
		int c;
		cin >> c;
		// dp[]の中から、c以上となる最初の要素を探す
		auto itr = lower_bound(dp.begin(), dp.end(), c);
		if(itr != dp.end())  // 見つかった場合は値を置き換え
			*itr = c;
		else  // 見つからなかった場合は末尾に追加
			dp.push_back(c);
	}
	cout << N - dp.size() << endl;

	return 0;
}
