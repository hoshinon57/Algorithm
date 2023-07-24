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

// ABC131 https://atcoder.jp/contests/abc131

/*
 * まず、最短距離が2となる頂点対の最大最小を考えてみると、
 *   最小：全頂点どうしに辺が張られているグラフ (完全グラフ)
 *   最大：N-1本の辺で、頂点1から各頂点への辺のみが張られているグラフ
 * かと思われる。
 * 最小は0, 最大はCombinationの (N-1)C(2) になる。
 * 
 * では最大最小の間のものは作れるかを考えると、
 *   最大となるグラフから、最短距離が2となる頂点間に1本辺を追加すると、距離2の頂点対は1つ減る
 * となりそう。
 * 
 * よって実装方針としては、
 * (1)最大となる頂点対よりKの方が大きいならば、-1で終了。
 * (2)まず頂点1から他頂点への辺を張り、
 *    (N-1)C(2)-K だけ辺を追加する。
 * 
 * ※方針や実装、hamayanhamayan氏のものと同一だった。
 *   https://blog.hamayanhamayan.com/entry/2019/06/22/225637
 */

int main(void)
{
	int i, j;
	int N, K; cin >> N >> K;
	int ans_max = (N-1)*(N-2)/2;

	if(ans_max < K)
	{
		cout << -1 << endl;
		return 0;
	}

	vector<pair<int,int>> ans;
	for(i = 1; i < N; i++)  // (0,i)間の辺を張る
	{
		ans.push_back({0, i});
	}
	int zan = ans_max - K;  // この本数だけ追加で辺を張る
	for(i = 1; i < N; i++)
	{
		for(j = i+1; j < N; j++)
		{
			if(zan == 0) break;
			ans.push_back({i, j});  // (i,j)間の辺を張る
			zan--;
		}
		if(zan == 0) break;
	}

	cout << ans.size() << endl;
	for(auto &e : ans)
	{
		cout << e.first+1 << " " << e.second+1 << endl;
	}

	return 0;
}
