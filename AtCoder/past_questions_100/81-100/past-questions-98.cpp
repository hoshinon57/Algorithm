#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1000000007;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題98
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/sumitrust2019/tasks/sumitb2019_e

/*
 * 赤青緑それぞれの帽子の数を、hats[3]に保持する。
 * 例えば{赤2,青2,緑1}なら{2,2,1}と表す。
 * 初期状態では{0,0,0}となる。
 * 
 * {3,3,1}のときAi=3であれば、2通り。
 * {0,0,0}のとき(初期状態)Ai=0であれば、3通り。
 * 各人に対してこれを掛け算していけばよい。
 * 
 * {3,3,1}のときAi=3であれば、次は{4,3,1}もしくは{3,4,1}にする必要がある。
 * どちらでも良いので、最初の方をインクリメントする実装とした。
 */

int main(void)
{
	// 0-indexed
	int i;
	int N;
	cin >> N;
	int hats[3] = {0,0,0};

	ll answer = 1;
	for(i = 0; i < N; i++)
	{
		int a;
		cin >> a;

		int count = 0;
		for(auto &e : hats)  // 入力と同じ色である帽子が、何種類あるか調べる
		{
			if(e == a)
			{
				count++;
				if(count == 1) e++;  // 先頭のhats[]を増やしておく
			}
		}
		answer = (answer*count)%MOD;
	}

	cout << answer << endl;

	return 0;
}
