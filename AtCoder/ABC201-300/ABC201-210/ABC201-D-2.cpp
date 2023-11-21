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

// ABC201 https://atcoder.jp/contests/abc201

/*
 * メモ化再帰で解く版。
 * 
 * またdpの定義を
 *   今回移動するプレイヤーのスコアが最大になるように移動するときの、スコアの最大値
 * とすることで、マス目の偶奇計算を不要とした。
 * 
 * 次のマス目は相手プレイヤー目線での得点なので、-1倍して計算すればよい。
 * 以下の解説が参考になった。
 *   https://twitter.com/kyopro_friends/status/1393563380640747529
 *   https://atcoder.jp/contests/abc201/submissions/22633546
 *   https://drken1215.hatenablog.com/entry/2023/07/21/235300
 */

int h, w;
vector<vector<bool>> vis;
vector<vector<int>> dp, a;

// (y,x)マスを起点に、移動するプレイヤーのスコアが最大になるように移動するときの
// スコアの最大値を返す
int func(int y, int x)
{
	if(vis[y][x]) return dp[y][x];  // メモ済み
	if(y == h-1 && x == w-1) return 0;

	int d = -INF32;
	if(y != h-1) d = max(d, -func(y+1, x) + a[y+1][x]);
	if(x != w-1) d = max(d, -func(y, x+1) + a[y][x+1]);
	dp[y][x] = d;
	vis[y][x] = true;

	return d;
}

int main(void)
{
	int i, j;
	cin >> h >> w;
	vis.resize(h, vector<bool>(w, false));
	dp.resize(h, vector<int>(w, -INF32));
	a.resize(h, vector<int>(w, 0));
	for(i = 0; i < h; i++)
	{
		string s; cin >> s;
		for(j = 0; j < w; j++)
		{
			a[i][j] = (s[j] == '+') ? 1 : -1;
		}
	}

	int sc = func(0, 0);
	string ans = "Draw";
	if(sc > 0) ans = "Takahashi";
	else if(sc < 0) ans = "Aoki";
	cout << ans << endl;

	return 0;
}
