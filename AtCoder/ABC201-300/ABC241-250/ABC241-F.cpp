#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <set>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC241 https://atcoder.jp/contests/abc241

/*
 * BFSで解く。解説と同じ考え方(それ以外の解法はほとんど無さそうだけど)。
 *   https://atcoder.jp/contests/abc241/editorial/3451
 *   https://x.com/kyopro_friends/status/1500064218594570244
 *   https://scrapbox.io/procon-kirokuyou/ABC241_F_-_Skate_(500)
 * 
 * 盤面は縦横10^9と大きいので、そのまま管理はできない。
 * 一方で停止できるマスはスタートマスと、障害物の上下左右4マスと少なく、高々4N+1である。
 * よってこの4N+1マスでmap,setを使ってグラフを構築し、スタートマスからBFSする。
 * ※distは未探索マスはINFではなく、count()==0 となる点に注意。
 * 詳しくは実装を参照。
 * 
 * 丁寧にGraphを構築してからBFSしたが、Graph構築は不要だったかな。
 * BFSのキューごとに、4方向を見て止まれるマスを探索してそこへ移動する、という実装で十分だったかも。
 * 
 * [ACまでの思考の流れ]
 * ・パッと見で01BFSか？ いや盤面が広すぎるから違うか。
 * ・盤面そのままでは管理できない。
 *   止まれるマスは実は4Nぐらいしかないので、それを管理すれば良さそう。
 */

using pii = pair<int,int>;
const int DIR = 4;
int dx[DIR] = {1, -1, 0, 0};
int dy[DIR] = {0, 0, 1, -1};

int main(void)
{
	// 0-indexed
	// 入力とX,Yを逆にしている
	int i;
	int H, W, N; cin >> H >> W >> N;
	int sy, sx; cin >> sy >> sx;
	sy--; sx--;
	int gy, gx; cin >> gy >> gx;
	gy--; gx--;
	map<int, set<int>> gyou, retu;  // gyou[y]={x1,x2,...} y行目にある障害物のx座標
	vector<int> y(N), x(N);
	for(i = 0; i < N; i++)
	{
		cin >> y[i] >> x[i];
		y[i]--; x[i]--;
		gyou[y[i]].insert(x[i]);
		retu[x[i]].insert(y[i]);
	}
	map<pii, vector<pii>> g;  // グラフ g[{y,x}}]={{y1,x1},{y2,x2},...}  マス{y,x}から行ける(止まれる)マスの一覧

	// g[{yy,xx}]からの行き先を作る
	auto make_graph = [&](int yy, int xx) -> void
	{
		// 右方向
		auto itr = gyou[yy].lower_bound(xx);
		if(itr != gyou[yy].end())
		{
			g[{yy,xx}].push_back({yy,*itr-1});
		}
		// 左
		if(itr != gyou[yy].begin())
		{
			itr--;
			g[{yy,xx}].push_back({yy,*itr+1});
		}
		// 下
		itr = retu[xx].lower_bound(yy);
		if(itr != retu[xx].end())
		{
			g[{yy,xx}].push_back({*itr-1, xx});
		}
		// 上
		if(itr != retu[xx].begin())
		{
			itr--;
			g[{yy,xx}].push_back({*itr+1, xx});
		}
	};

	// 障害物の上下左右4マスを基点に、止まれるマスを探してグラフ構築
	for(i = 0; i < N; i++)
	{
		for(int d = 0; d < DIR; d++)
		{
			int ny = y[i] + dy[d];
			int nx = x[i] + dx[d];
			if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
			make_graph(ny, nx);
		}
	}
	make_graph(sy, sx);  // スタート地点を基点

	queue<pii> que;  // {y,x}
	map<pii, int> dist;  // dist[{y,x}]
	que.push({sy, sx});
	dist[{sy, sx}] = 0;
	while(!que.empty())
	{
		auto[yy,xx] = que.front();
		que.pop();
		for(auto [ny,nx] : g[{yy,xx}])  // {yy,xx} -> {ny,nx}
		{
			if(dist.count({ny,nx})) continue;  // 探索済み
			dist[{ny,nx}] = dist[{yy,xx}] + 1;
			que.push({ny,nx});
		}
	}

	int ans = -1;
	if(dist[{gy,gx}]) ans = dist[{gy,gx}];
	cout << ans << endl;

	return 0;
}
