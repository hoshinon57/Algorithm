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
 * まず、このコードではACにならない。
 * 結果自体は正しいが、コンテスト後に追加されたafter_contestのテストケースにてTLEとなる。
 * 参考：https://qiita.com/u2dayo/items/3f1ea15ec3b71d021b43
 *       https://scrapbox.io/procon-kirokuyou/ABC237_E_-_Skiing_(500)
 * 
 * この問題を解くにあたりダイクストラ法およびベルマンフォード法は
 * ほとんど理解していなかったが、以下のように考えた。
 * (どちらの手法でも無いはず)
 * 
 * 広場Nにいるときの、これまでの最大の楽しさをscore[]に持つ。
 * 現在の広場から移動する際、移動したらscore[]を更新できるか計算し、
 * 更新できるなら移動先をキューに積む。
 * 後はキューが空になるまで計算する。
 * 
 * 最後に、score[]の全体から最大値を探し、それが答になる。
 */
int main(void)
{
	// 0-indexedに変換する
	int i;
	int N, M;
	cin >> N >> M;
	vector<int> H(N);
	int h_min = INF32;
	for(i = 0; i < N; i++)
	{
		cin >> H[i];
		h_min = min(h_min, H[i]);
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

	vector<ll> score(N, -INF64);  // 広場Nにいるときの、これまでの最大の楽しさ

	/*
	for(i = 0; i < N; i++)
	{
		cout << i << ":";
		for(auto e : graph[i])
		{
			cout << e << " ";
		}
		cout << endl;
	}
	*/

	queue<int> que;   // 現在の広場
	// 広場1、楽しさ0で開始
	que.push(0);  
	score[0] = 0;

	while(!que.empty())
	{
		ll tmpScore;
		int nowSpace = que.front();
		que.pop();
		for(auto &e : graph[nowSpace])
		{
			// 移動する場合の楽しさ計算
			if(H[nowSpace] >= H[e])
			{
				tmpScore = score[nowSpace] + (H[nowSpace] - H[e]);
			}
			else
			{
				tmpScore = score[nowSpace] - 2*(H[e] - H[nowSpace]);
			}
			// 移動先の楽しさが更新できるか
			if(score[e] < tmpScore)
			{
				score[e] = tmpScore;
				que.push(e);
			}
		}
	}
	
//	cout << "---" << endl;
	ll maxScore = -INF64;
	for(auto &e : score)
	{
//		cout << e << endl;
		maxScore = max(maxScore, e);
	}

	cout << maxScore << endl;

	return 0;
}
