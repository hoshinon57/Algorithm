#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC320 https://atcoder.jp/contests/abc320

/*
 * シミュレーションで解く。
 * 
 * 「そうめんを流す」「人が列に戻ってくる」の2つをイベントとして管理する。
 * 優先度付きキューにて以下のデータを管理する。
 *   {time, kind, ex}
 *   kind=1のとき：そうめんを流すイベント ex:そうめんの番号
 *   kind=0のとき：人が列に戻ってくるイベント ex:戻ってくる人の番号
 *   問題文の条件より、そうめんよりも人が列に戻るイベントを優先するため、kindは上記の値とする
 * 
 * 後は実装を参考。
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	vector<ll> w(M), s(M);
	priority_queue<vector<ll>, vector<vector<ll>>, greater<vector<ll>>> que;
	for(i = 0; i < M; i++)
	{
		int t;
		cin >> t >> w[i] >> s[i];
		que.push({t, 1, i});
	}
	set<int> per;  // 並んでいる人
	vector<ll> ans(N);
	for(i = 0; i < N; i++) per.insert(i);

	while(!que.empty())
	{
		auto tmp = que.top();
		que.pop();		
		int t = tmp[0];
		int kind = tmp[1];
		int ex = tmp[2];
		if(kind == 1)
		{
			if(!per.empty())
			{
				// 先頭の人がそうめんをゲットし、S後に列に戻るイベントを追加
				int n = *per.begin();
				per.erase(n);
				ans[n] += w[ex];
				que.push({t+s[ex], 0, n});
			}
		}
		else
		{
			// ex番目の人が列に戻る
			per.insert(ex);
		}
	}
	for(auto &e : ans) cout << e << endl;

	return 0;
}
