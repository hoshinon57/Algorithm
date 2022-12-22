#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題32
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1166&lang=jp

/*
 * 幅優先探索で解く。
 * 
 * 入力を元に、各マスから行けるマスを迷路mpとして構築した後、
 * 幅優先探索でゴールまでの距離を求める。
 * 
 * 壁の持ち方に悩んだが、
 * mp[{y,x}] = マス{y,x}から行けるマスの一覧
 * とすればよいので、mp[{y,x}]の実現にはmapを、「一覧」の実現にはsetを採用した。
 * 
 * 計算量は、1つのデータセットあたり O(hw*log(hw)) かな…？
 * 頂点数および辺数がO(hw)で、setを使う部分でO(log(hw))かと考えるため。
 */

using pii = pair<int,int>;  // {y,x}
int w, h;

// 入力を元に迷路mpを構築する
void input(map<pii, set<pii>> &mp)
{
	for(int i = 0; i < h*2-1; i++)
	{
		int n;
		int x;
		int y = i/2;
		if(i%2 == 0)  // 横方向
		{
			for(x = 0; x < w-1; x++)  // x
			{
				// マス{y,x}と{y,x+1}が繋がっているかどうか
				cin >> n;
				if(n == 0)
				{
					mp[{y,x}].insert({y,x+1});
					mp[{y,x+1}].insert({y,x});
				}
			}
		}
		else  // 縦方向
		{
			for(x = 0; x < w; x++)  // x
			{
				// マス{y,x}と{y+1,x}が繋がっているかどうか
				cin >> n;
				if(n == 0)
				{
					mp[{y,x}].insert({y+1,x});
					mp[{y+1,x}].insert({y,x});
				}
			}
		}
	}
}

// 迷路mpに対して、幅優先探索で各マスへの距離を求める
int bfs(map<pii, set<pii>> &mp)
{
	vector<vector<int>> dist(h, vector<int>(w, INF32));  // dist[y][x]
	queue<pii> que;

	// 始点は左上のマス
	dist[0][0] = 1;
	que.push({0,0});

	while(!que.empty())
	{
		int y = que.front().first;
		int x = que.front().second;
		que.pop();

		for(auto &e : mp[{y,x}])  // マス{y,x}から行けるマスを列挙
		{
			int ty = e.first;
			int tx = e.second;
			if(dist[ty][tx] != INF32) continue;  // 探索済み

			// ty,txには範囲内の有効な値しか入らないため、範囲外チェックは不要
			dist[ty][tx] = dist[y][x] + 1;
			que.push({ty, tx});
		}
	}

	// 右下のマスがゴール
	int answer;
	if(dist[h-1][w-1] != INF32)
		answer = dist[h-1][w-1];
	else
		answer = 0;

	return answer;
}

int main(void)
{
	// 0-indexed
	while(true)
	{
		cin >> w >> h;
		if(w == 0 && h == 0) break;

		// 入力
		// マス{y,x}に対して、繋がっているマス{y', x'}の一覧をsetで管理
		map<pii, set<pii>> mp;
		input(mp);

		// 探索
		cout << bfs(mp) << endl;
	}

	return 0;
}
