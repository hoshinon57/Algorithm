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

// ABC113 https://atcoder.jp/contests/abc113

/*
 * 座標圧縮に近い考え方で解く。
 * (座標圧縮の勉強時、WEBサイトでの関連問題として載っていた)
 * 
 * 県ごとに属する市を管理する配列 id[N][*] を用意する。
 * これをid[i]ごとにソートしておく。
 * 
 * すると各Pi,Yiについて、認識番号の下6桁は
 *   ソートされたid[Pi]にて、何番目にYiが登場するか(の+1)
 * となる。これをlower_bound()で求めればよい。
 */

int main(void)
{
	// 0-indexed
	int i;
	int N, M;
	cin >> N >> M;
	vector<int> p(M), y(M);
	vector<vector<int>> id(N, vector<int>());  // id[N][]
	for(i = 0; i < M; i++)
	{
		cin >> p[i] >> y[i];
		p[i]--;
		id[p[i]].push_back(y[i]);
	}

	for(i = 0; i < N; i++)
	{
		sort(id[i].begin(), id[i].end());
	}
	// 重複は無いためerase()は不要

	for(i = 0; i < M; i++)
	{
		// 1-indexedに戻すので+1
		cout << std::setfill('0') << std::setw(6);  // 6桁の0埋め
		cout << p[i]+1;
		cout << std::setfill('0') << std::setw(6);
		auto x = lower_bound(id[p[i]].begin(), id[p[i]].end(), y[i]) - id[p[i]].begin();
		cout << x+1 << endl;
	}

	return 0;
}
