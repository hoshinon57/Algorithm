#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC170 https://atcoder.jp/contests/abc170

/*
 * chokudai氏の解法で実装してみた。
 *   https://blog.hamayanhamayan.com/entry/2020/06/15/010432
 *   https://x.com/kyopro_friends/status/1272178596165988354
 *   https://atcoder.jp/contests/abc170/submissions/14358430  chokudai氏の提出
 * 
 * "Kマス移動"も各頂点からforループでK回まわす、愚直なBFSを考える。O(HWK).
 * ここから枝刈りをすると十分に早くなってACとなる。
 * 今の頂点{y,x}から水かきしたときの回数は d_next=dist[y][x]+1 となる。
 * ・dist[ny][nx] < d_next なら、既に探索したときの方が確実に良いので、探索打ち切りできる。
 * ・dist[ny][nx] == d_next なら、このままKマス移動をループ回したときに
 *   その先のマスでdistを更新できるかもしれない。
 *   よってマス{ny,nx}はキューには積まないが、探索は続ける。
 *   ※うまく文章で書けないが、
 *     下から上方向へ探索したマスに対し、左から右方向へ探索したときに、
 *     「そういうケースがありうる」というのは分かる。
 * ・dist[ny][nx] > d_next なら、distを更新してキューに積む。
 *   これはBFSの仕組みより、{ny,nx}が未探索と同義になる。
 * 
 * 後は実装を参照。
 */

const int DIR = 4;
int dx[DIR] = {1, -1, 0, 0};
int dy[DIR] = {0, 0, 1, -1};

int main(void)
{
	int i;
	int H, W, K; cin >> H >> W >> K;
	int y1, x1, y2, x2; cin >> y1 >> x1 >> y2 >> x2;  // 問題文とx,yの意味を逆にする
	y1--; x1--; y2--; x2--;
	vector<string> mp(H);
	for(i = 0; i < H; i++) cin >> mp[i];

	vector<vector<int>> dist(H, vector<int>(W, INF32));
	queue<pair<int,int>> que;  // {y, x}
	dist[y1][x1] = 0;
	que.push({y1, x1});

	while(!que.empty())
	{
		auto [y,x] = que.front();
		que.pop();
		for(int d = 0; d < DIR; d++)
		{
			int ny = y, nx = x;
			for(i = 1; i <= K; i++)
			{
				ny += dy[d];
				nx += dx[d];
				if(ny < 0 || ny >= H || nx < 0 || nx >= W) break;
				if(mp[ny][nx] == '@') break;

				// 探索打ち切り
				// if(dist[ny][nx] <= dist[y][x]) break;
				if(dist[ny][nx] < dist[y][x] + 1) break;  // 各種解説では上記だったが、本質はこっちかなと思った

//				if(dist[ny][nx] > dist[y][x] + 1)
				if(dist[ny][nx] == INF32)  // 判定は上記でも良いが、本質はこちら。未探索マスかどうかで判断する
				{
					dist[ny][nx] = dist[y][x] + 1;
					que.push({ny, nx});
				}
				// キューに積まれる値は1ずつ増えていくことから、
				// dist[ny][nx]==dist[y][x]+1 のときは、探索打ち切りせず、一方でキューにも積まず、となる
			}
		}
	}

	int ans = dist[y2][x2];
	if(ans == INF32) ans = -1;
	cout << ans << endl;

	return 0;
}
