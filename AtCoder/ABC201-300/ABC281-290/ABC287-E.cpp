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

// ABC287 https://atcoder.jp/contests/abc287

/*
 * コンテスト中に解けず、解説を見た。
 *   https://atcoder.jp/contests/abc287/editorial/5645
 *   https://programming-hiroba.com/abc287-e/
 * 
 * 各iについての答の候補は、文字列Siを辞書順にソートしたときの前後の要素となる。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<pair<string,int>> s_idx(N);
	for(i = 0; i < N; i++)
	{
		cin >> s_idx[i].first;
		s_idx[i].second = i;
	}
	sort(s_idx.begin(), s_idx.end());

	vector<int> ans(N);  // ans[i]:Siの答
	for(i = 0; i < N; i++)
	{
		int cnt = 0;
		// 後側と比較
		if(i != N-1)
		{
			for(j = 0; j < (int)min(s_idx[i].first.size(), s_idx[i+1].first.size()); j++)  // 先頭から一致文字を探索
			{
				if(s_idx[i].first[j] != s_idx[i+1].first[j]) break;
			}
			cnt = max(cnt, j);
		}
		// 前側と比較
		if(i != 0)
		{
			for(j = 0; j < (int)min(s_idx[i].first.size(), s_idx[i-1].first.size()); j++)
			{
				if(s_idx[i].first[j] != s_idx[i-1].first[j]) break;
			}
			cnt = max(cnt, j);
		}
		ans[s_idx[i].second] = cnt;
	}

	for(auto &e : ans)
	{
		cout << e << endl;
	}

	return 0;
}
