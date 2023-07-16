#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC237 https://atcoder.jp/contests/abc237

/*
 * ダイクストラ法で解くが、そのままでは負のコストが存在するため解けない。
 * そのためコストの決め方を区数する(ポテンシャル、というらしい)
 * 参考：
 * https://qiita.com/u2dayo/items/3f1ea15ec3b71d021b43
 * https://scrapbox.io/procon-kirokuyou/ABC237_E_-_Skiing_(500)
 * https://zenn.dev/akiku/articles/akiku-abc237
 * https://kyo-pro.hatenablog.com/entry/ABC237E
 * 
 * 以下のようにコストを定義する。
 *   広場X→Yへ移動するとき、
 *   XがYより高い(下る)場合、移動コストを0とする(標高が楽しさに変換された、という考え)
 *   XよりYが高い(登る)場合、移動コストを|H(X)-H(Y)|とする
 * これにより負のコストが無くなり、ダイクストラ法で解ける。
 * 
 * 各広場iについて最小コストを計算したら、
 *   (H[0]-H[i])-cost[i]
 * の最大値が求める「楽しさの最大値」となる。
 * ここでH[0]-H[i]の部分は、標高が楽しさに変換された部分。
 * cost[i]が、高い場所への移動で失われた楽しさの部分。
 */

int main(void)
{
	// 0-indexedに変換する
	int i;
	int N, M;
	cin >> N >> M;
	vector<int> H(N);
	for(i = 0; i < N; i++)
	{
		cin >> H[i];
	}
	vector<vector<int>> graph(N);
	for(i = 0; i < M; i++)
	{
		int u, v;
		cin >> u >> v;
		u--;
		v--;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	/*
	 * 広場X→Yへ移動するとき、
	 * XがYより高い(下る)場合、移動コストを0とする
	 * XよりYが高い(登る)場合、移動コストを|H(X)-H(Y)|とする
	 */
	vector<ll> cost(N, INF64);  // 広場1から広場Nへ到達したときの最小コスト

	// [pair]first:キュー積み時のコスト second:広場
	priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> que;
	// 広場1、楽しさ0で開始
	pair<int, int> tmp;
	tmp.first = tmp.second = 0;
	que.push(tmp);
	cost[0] = 0;

	while(!que.empty())
	{
		// 最小コストであるコスト/広場のペアをキューから取得
		tmp = que.top();
		int nowSpace = tmp.second;
		que.pop();

		// キューに同じ場所が積まれている可能性がある
		// キューに積まれていたコストを見て、それより小さなコストで処理済みなら飛ばす	
		if(tmp.first > cost[nowSpace]) continue;

		for(auto &e : graph[nowSpace])   // e:移動先 nowSpace:現在の広場
		{
			ll moveCost;
			// 移動コスト計算
			if(H[nowSpace] >= H[e])
			{
				// 下るならコストは0とみなす
				moveCost = 0;
			}
			else
			{
				// 登る場合、コストは|Hx-Hy|とみなす
				moveCost = H[e] - H[nowSpace];
			}
			// 移動先のコストを更新できるか
			if(cost[e] > cost[nowSpace] + moveCost)
			{
				cost[e] = cost[nowSpace] + moveCost;
				tmp.first = cost[nowSpace] + moveCost;
				tmp.second = e;
				que.push(tmp);
			}
		}
	}
	
	ll answer = -INF64;
	// H[0]-H[i]：標高が楽しさに変換された部分
	// となるため、各広場H[i]について H[0]-H[i] からコストを引き、その最大値が回答の「楽しさ」となる。
	for(i = 0; i < N; i++)
	{
		answer = max(answer, H[0] - H[i] - cost[i]);
	}
	cout << answer << endl;

	return 0;
}
