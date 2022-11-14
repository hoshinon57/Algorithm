#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC277 https://atcoder.jp/contests/abc277

/*
 * 深さ優先探索で解く。 
 * 
 * vector<vector<int>> graph; としてはしごで繋がっている階を管理しようとすると、
 * 階が10^9まであるためメモリが足りない。
 * そのため連想配列にてデータを持たせる。
 * 
 * それ以外はいわゆる「教科書通りの実装」でDFSを実装すればよい。
 * 探索ごとに階の最大を保持しておき、全て探索した後にそれが答となる。
 * 
 * 計算量はO(NlogN).
 * はしごがN本であり、mapの操作が1回あたりlogNのため。
 */

map<int, vector<int>> mp;  // mp[i]:i階からはしごで繋がっている先の階の一覧
map<int, bool> seen;  // seen[i]:i階を探索済みならtrue
int max_answer = 0;

// now階から深さ優先探索で探索していく
void dfs(int now)
{
	if(seen[now]) return;  // 探索済み
	seen[now] = true;

	max_answer = max(max_answer, now);

	// now -> 行ける階
	for(auto &e : mp[now])  // e：行ける階
	{
		dfs(e);
	}
}

int main(void)
{
	// 1-indexed
	int i;
	int N;
	cin >> N;
	for(i = 0; i < N; i++)
	{
		int a, b;
		cin >> a >> b;
		mp[a].push_back(b);
		mp[b].push_back(a);
		seen[a] = false;  // 未探索
		seen[b] = false;
	}

	dfs(1);  // 1階からスタート
	cout << max_answer << endl;

	return 0;
}
