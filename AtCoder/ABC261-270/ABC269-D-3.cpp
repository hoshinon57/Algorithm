#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC269 https://atcoder.jp/contests/abc269

/*
 * 幅優先探索で解く。
 * データの持ち方はABC269-D.cppとほぼ同じ。
 * 
 * 入力のN個のマスそれぞれを起点にして、bfs()にて隣接マスをたどっていく。
 * たどったマスはseen[]をtrueに更新していく。
 * 
 * 起点からbfs()を呼び出した回数が、求める連結個数となる。
 */

int N;
vector<pair<int,int>> xy;  // xy[i]:入力におけるi番目のマスのx,y(0-indexed)
int black[2010][2010] = {0};  // (x,y)が黒なら、black[x][y]=1
int grid_idx[2010][2010] = {0};  // (x,y)が黒のとき、それが何番目のマスかをgrid_idx[x][y]に保持 (0～N-1)
vector<bool> seen;  // seen[i]:i番目のマスを探索済みならtrue

// no番目のマスを起点に幅優先探索を行う
// たどったマスはseen[]をtrueに更新していく
void bfs(int no)
{
	queue<int> que;
	que.push(no);  // no番目のマスを起点とする
	seen[no] = true;  // 起点は探索済み

	int dx[6] = {-1, -1,  0, 0, 1, 1};  // 隣接のマス数は6種類
	int dy[6] = {-1,  0, -1, 1, 0, 1};
	while(!que.empty())
	{
		int v = que.front();
		que.pop();

		int x = xy[v].first;
		int y = xy[v].second;
		for(int i = 0; i < 6; i++)
		{
			if(black[x+dx[i]][y+dy[i]] == 1)  // 隣のマスが黒か
			{
				// 隣のマスが何番目のマスかを、grid_idxを使って取得
				int idx = grid_idx[x+dx[i]][y+dy[i]];
				if(seen[idx]) continue;  // 探索済みならスルー

				// 探索済みにしてキューに積む
				seen[idx] = true;
				que.push(idx);
			}
		}
	}
}

int main(void)
{
	// 0-indexed
	int i;
	cin >> N;
	for(i = 0; i < N; i++)
	{
		int x, y;
		cin >> x >> y;
		x += 1005;  // 配列の添字が負にならないよう、+1005のゲタをはかせる
		y += 1005;
		xy.push_back({x,y});
		black[x][y] = 1;
		grid_idx[x][y] = i;
	}
	seen.resize(N, false);

	int answer = 0;
	for(i = 0; i < N; i++)
	{
		if(seen[i]) continue;  // 探索済みならスルー
		bfs(i);
		answer++;  // 各頂点を起点としてBFSを実施できた回数が、求める連結個数となる
	}
	cout << answer << endl;

	return 0;
}
