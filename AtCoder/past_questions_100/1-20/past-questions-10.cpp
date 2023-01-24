#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題10
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_5_A&lang=ja

/*
 * ビット全探索で解く。
 * 
 * 事前計算として、Aから要素を選ぶ全ての選び方をビット全探索し、
 * それぞれの和をunordered_setで保持しておく。
 * あとは各クエリごとに有無を判定すればよい。
 * 
 * 計算量はO(2^n*n + q).
 * ビット全探索の部分がO(2^n*n), クエリ回答がO(q)のため。
 */
int main(void)
{
	int i, j;
	int n;
	cin >> n;
	vector<int> a(n);
	for(i = 0; i < n; i++) cin >> a[i];

	unordered_set<int> add_list;  // Aから要素をいくつか選んだときの和、の一覧
	// Aを元にbit全探索
	for(i = 0; i < (1<<n); i++)
	{
		int sum = 0;
		for(j = 0; j < n; j++)
		{
			if( ((i>>j)&1) == 1 )  // a[j]を選ぶ場合
			{
				sum += a[j];
			}
		}
		add_list.insert(sum);
	}

	int q;
	cin >> q;
	for(; q > 0; q--)
	{
		int m;
		cin >> m;
		// 一覧の中にmがあるか否か
		if(add_list.count(m) > 0) cout << "yes" << endl;
		else cout << "no" << endl;
	}

	return 0;
}
