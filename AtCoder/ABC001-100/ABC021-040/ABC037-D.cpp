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
const ll MOD = 1e9+7;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC037 https://atcoder.jp/contests/abc037

/*
 * メモ化再帰で解く。
 * dfs(sy,sx)を、マス(sy,sx)からスタートする場合の経路数を返すとする。
 * 全マスについてdfs()を呼び出した総和が答。
 * 
 * dfs()では上下左右のマスについて再帰呼び出しをし、戻り値を加算する。
 * もちろん移動先のa[][]が大きいことが条件。
 */

int H, W;
vector<vector<ll>> a;
vector<vector<ll>> memo;

ll dfs(int sy, int sx)
{
	ll &m = memo[sy][sx];
	if(m != -1) return m;

	const int DIR = 4;
	int dx[DIR] = {1, -1, 0, 0};
	int dy[DIR] = {0, 0, 1, -1};
	m = 1;  // 最初のマスから動かないケース
	for(int i = 0; i < DIR; i++)
	{
		int ny = sy+dy[i];
		int nx = sx+dx[i];
		if(ny < 0 || ny >= H || nx < 0 || nx >= W) continue;
		if(a[sy][sx] >= a[ny][nx]) continue;
		m += dfs(ny, nx);
		m %= MOD;
	}
	return m;
}

int main(void)
{
	int i, j;
	cin >> H >> W;
	a.resize(H, vector<ll>(W));
	memo.resize(H, vector<ll>(W, -1));
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++) cin >> a[i][j];
	}

	ll ans = 0;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			ans += dfs(i, j);
			ans %= MOD;
		}
	}
	cout << ans << endl;

	return 0;
}
