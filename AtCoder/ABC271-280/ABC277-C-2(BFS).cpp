#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC277 https://atcoder.jp/contests/abc277

/*
 * 幅優先探索で解く。
 * 連想配列を使っている以外は、いわゆる「教科書通りの実装」となる。
 * 
 * 計算量は同じくO(NlogN).
 * はしごがN本であり、mapの操作が1回あたりlogNのため。
 */
int main(void)
{
	int i;
	int N;
	cin >> N;
	map<int, vector<int>> mp;  // mp[i]:i階からはしごで繋がっている先の階の一覧
	map<int, bool> seen;  // seen[i]:i階を探索済みならtrue
	for(i = 0; i < N; i++)
	{
		int a, b;
		cin >> a >> b;
		mp[a].push_back(b);
		mp[b].push_back(a);
		seen[a] = false;  // 未探索
		seen[b] = false;
	}

	int max_floor = 0;  // これまでに辿った最高の階
	queue<int> que;
	que.push(1);  // 1階スタート
	seen[1] = true;

	while(!que.empty())
	{
		int floor = que.front();
		que.pop();

		max_floor = max(max_floor, floor);

		for(auto &e : mp[floor])  // e:floor階からはしごで繋がっている先の階
		{
			if(seen[e]) continue;  // 探索済みのためスルー

			seen[e] = true;
			que.push(e);
		}
	}

	cout << max_floor << endl;

	return 0;
}
