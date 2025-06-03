#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC408 https://atcoder.jp/contests/abc408

/*
 * DFSで解く。ABC196-D(Hanjo)が近い。
 * 左上のマスから順に、ドミノを縦に置く/横に置く/置かない を決めながらDFS.
 * 
 * 途中経過のxor値を持たない/持つ、で2通りの実装がある。
 * 1)持たない
 *   dfs1()にて。コンテスト時はこれ。
 *   末尾までドミノの配置を決めてから、スコアを計算。
 *   O(HW*2^(HW)) とか。
 * 2)持つ
 *   dfs2()にて。公式解説とかで学んだ。
 *   現在のxor値を引数に持ちながらDFS. main()では全xorを計算して引数に与えることに注意。
 *   O(2^(HW)) とかのはず。
 * 
 * (1)は9ms, (2)は3msとかだった。
 * まあコンテスト時は実装の複雑さをイメージして決めても良いかなと。
**/

ll H, W;
vector<vector<ll>> a;
ll ans = 0;

void dfs1(ll y, ll x, vector<vector<ll>> &mp)
{
	// 最後まで行った
	if(y == H)
	{
		ll i, j;
		ll ret = 0;
		for(i = 0; i < H; i++)
		{
			for(j = 0; j < W; j++)
			{
				if(mp[i][j] == 1) continue;
				ret ^= a[i][j];
			}
		}
		chmax(ans, ret);
		return;
	}

	// 次のマス
	ll ny, nx;
	if(x != W-1)
	{
		nx = x+1;
		ny = y;
	}
	else
	{
		nx = 0;
		ny = y+1;
	}

	// 置かないケース
	dfs1(ny, nx, mp);

	// 置くケース
	if(mp[y][x] == 0)
	{
		// 右方向に置く
		if(x != W-1 && mp[y][x+1] == 0)
		{
			mp[y][x] = mp[y][x+1] = 1;
			dfs1(ny, nx, mp);
			mp[y][x] = mp[y][x+1] = 0;
		}
		// 下方向に置く
		if(y != H-1 && mp[y+1][x] == 0)
		{
			mp[y][x] = mp[y+1][x] = 1;
			dfs1(ny, nx, mp);
			mp[y][x] = mp[y+1][x] = 0;
		}
	}
}

void dfs2(ll y, ll x, vector<vector<ll>> &mp, ll now)
{
	// 最後まで行った
	if(y == H)
	{
		chmax(ans, now);
		return;
	}

	// 次のマス
	ll ny, nx;
	if(x != W-1)
	{
		nx = x+1;
		ny = y;
	}
	else
	{
		nx = 0;
		ny = y+1;
	}

	// 置かないケース
	dfs2(ny, nx, mp, now);

	// 置くケース
	if(mp[y][x] == 0)
	{
		// 右方向に置く
		if(x != W-1 && mp[y][x+1] == 0)
		{
			mp[y][x] = mp[y][x+1] = 1;
			dfs2(ny, nx, mp, now^a[y][x]^a[y][x+1]);
			mp[y][x] = mp[y][x+1] = 0;
		}
		// 下方向に置く
		if(y != H-1 && mp[y+1][x] == 0)
		{
			mp[y][x] = mp[y+1][x] = 1;
			dfs2(ny, nx, mp, now^a[y][x]^a[y+1][x]);
			mp[y][x] = mp[y+1][x] = 0;
		}
	}
}

int main(void)
{
	ll i, j;
	cin >> H >> W;
	a.resize(H, vector<ll>(W));
	ll tot = 0;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			cin >> a[i][j];
			tot ^= a[i][j];
		}
	}
	vector<vector<ll>> mp(H, vector<ll>(W, 0));  // mp[H][W]  そこに置いたら1

//	dfs1(0, 0, mp);
	dfs2(0, 0, mp, tot);
	cout << ans << endl;
	return 0;
}
