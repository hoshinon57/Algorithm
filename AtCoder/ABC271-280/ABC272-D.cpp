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

// ABC272 https://atcoder.jp/contests/abc272

/*
 * 幅優先探索で解く。
 * 
 * 駒の移動量が√Mということは、
 * x方向の移動量をa, y方向の移動量をbとすると
 * M=a^2+b^2 が成立する。
 * よって、このような{a,b}の組み合わせをまず全て求める。
 * a,bの最大値は(N-1)であるので、ここの計算量はO(N^2).
 * 
 * 例えばM=5であれば 1^2+2^2 であるので、
 * ある方向に1マス、もう片方の方向に2マス進める。
 * 
 * 全てリストアップできたら、後は始点から幅優先探索すればよい。
 */
int main(void)
{
	// 0-indexed
	int i, j;
	int N, M;
	cin >> N >> M;
	vector<pair<int,int>> move_yx;  // 駒が何マス移動できるか、のリスト {y方向の移動量, x方向の移動量}
	// M=a^2+b^2 となるようなa,bを探す
	for(i = 0; i <= N; i++)
	{
		for(j = i; j <= N; j++)
		{
			if(i*i +j*j == M)
			{
				// 例えば{1,2}のとき、
				//   -1や-2など負側のケース
				//   {2,1}のようにxy逆のケース
				// があるのでそれらも列挙する
				// x=yや x(or y)=0の場合などで重複するが、結果に問題は無い
				move_yx.push_back({ i,  j});
				move_yx.push_back({ i, -j});
				move_yx.push_back({-i,  j});
				move_yx.push_back({-i, -j});
				move_yx.push_back({ j,  i});
				move_yx.push_back({ j, -i});
				move_yx.push_back({-j,  i});
				move_yx.push_back({-j, -i});
			}
		}
	}

	queue<pair<int,int>> que;  // {y,x}
	vector<vector<int>> dist(N, vector<int>(N, INF32));  // dist[y][x]
	que.push({0,0});  // {1,1}を起点として探索
	dist[0][0] = 0;
	while(!que.empty())
	{
		int x = que.front().second;
		int y = que.front().first;
		que.pop();

		for(auto e : move_yx)
		{
			int dx = e.second;
			int dy = e.first;
			if(x+dx < 0 || x+dx >= N || y+dy < 0 || y+dy >= N) continue;

			if(dist[y+dy][x+dx] > dist[y][x]+1)  // 距離を更新できるなら
			{
				dist[y+dy][x+dx] = dist[y][x]+1;
				que.push({y+dy, x+dx});
			}
		}
	}

	for(i = 0; i < N; i++)  // y
	{
		for(j = 0; j < N; j++)  // x
		{
			if(dist[i][j] != INF32)
			{
				cout << dist[i][j];				
			}
			else
			{
				cout << -1;
			}
			if(j != N-1) cout << " ";
		}
		cout << endl;
	}

	return 0;
}
