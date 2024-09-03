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

// ABC264 https://atcoder.jp/contests/abc264

/*
 * 動的計画法で解く。
 * DPの持ち方は色々あるが、自分はdp[y][x][i][j]を以下のように定義した。
 *   マス(y,x)まで来ていて、
 *   i:その行を反転したなら1, そうでないなら0
 *   j:その列について同様
 *   のときのコスト最小値。
 * dp[H-1][W-1][*][*]の最小値が答。
 * ※これが近いかな、と思う https://bondo.hateblo.jp/entry/2022/08/14/093820
 * 
 * 考え方として、
 * ・反転は行および列ごとに0回か1回まで。2回以上は無意味。
 * ・右に移動したい場合、マスの色が異なって反転が必要な場合は列を反転させる一択。
 *   行を反転しても色は異なるままだし、左右でマスの色が同じならそもそも行の反転が不要。
 * ・同様に下に移動したい場合、行を反転させる一択。
 * といったのを考えていって、前述のDPの持ち方になった。
 * 
 * 右方向に移動する場合、 dp[y][x-1][i2][j2], dp[y][x][i1][j1]とすると、
 * i2,j2,i1,j1の全パターンについて確認していく。2^4通り。
 * ただし反転するなら列なので、j1=j2は前提。
 * またi2,j2,i1,j1にてAの値は確定するので、これが左右で一致していることも前提。
 * その上で、i1=1なら行を反転させるのでR[y]を加算する。
 * 
 * 下方向に移動する場合も同様。
 * 
 * dp[]は4次元なので生配列で持たせた。vectorだと定義を書くのが面倒そうだったので。
 * またvectorだと次元数が多すぎるとだいぶ遅くなるイメージ。maspy氏の提出が1968msとだいぶギリギリだった。
 *   https://atcoder.jp/contests/abc264/submissions/33998562
 * 
 * [ACまでの思考の流れ]
 * ・DPっぽい。
 * ・どんな値を持たせるかを考えると、まず現在のマス(y,x)は必須だろう。
 * ・同じ行は2回以上反転させる必要は無さそう、右に移動するときは反転するなら列側、などを考えると
 *   今見ているマスについて「行/列を反転させたか」という情報があれば良さそう。
 */

ll dp[2005][2005][2][2];  // (y,x,i:その行を反転したら1,j:その列を反転したら1)

int main(void)
{
	int i1, j1, i2, j2;
	int y, x;
	int H, W; cin >> H >> W;
	vector<ll> r(H); for(i1 = 0; i1 < H; i1++) {cin >> r[i1];}
	vector<ll> c(W); for(i1 = 0; i1 < W; i1++) {cin >> c[i1];}
	vector<vector<int>> a(H, vector<int>(W, 0));  // a[H][W]
	for(i1 = 0; i1 < H; i1++)
	{
		string s; cin >> s;
		for(j1 = 0; j1 < W; j1++) a[i1][j1] = s[j1]-'0';
	}
	dp[0][0][0][1] = c[0];
	dp[0][0][1][0] = r[0];
	dp[0][0][1][1] = c[0]+r[0];

	for(y = 0; y < H; y++)
	{
		for(x = 0; x < W; x++)
		{
			if(y == 0 && x == 0) continue;
			dp[y][x][0][0] = dp[y][x][0][1] = dp[y][x][1][0] = dp[y][x][1][1] = INF64;
			for(i1 = 0; i1 < 2; i1++)
			{
				for(j1 = 0; j1 < 2; j1++)  // 今の(y,x)マスのdp[y][x][i1][j1]
				{
					for(i2 = 0; i2 < 2; i2++)
					{
						for(j2 = 0; j2 < 2; j2++)  // 1つ前のマスの [i2][j2]
						{
							int pa, na;  // pre,now
							// 上から
							if(y-1 >= 0)
							{
								pa = a[y-1][x] ^ i2 ^ j2;
								na = a[y][x] ^ i1 ^ j1;
								// 列の反転状況は一致していること、またAの値が一致していること、が条件
								if(j1 == j2 && pa == na)
								{
									ll ofs = 0;
									if(i1 == 1) ofs = r[y];
									chmin(dp[y][x][i1][j1], dp[y-1][x][i2][j2] + ofs);
								}
							}

							// 左から
							if(x-1 >= 0)
							{
								pa = a[y][x-1] ^ i2 ^ j2;
								na = a[y][x] ^ i1 ^ j1;
								// 行の反転状況は一致していること、またAの値が一致していること、が条件
								if(i1 == i2 && pa == na)
								{
									ll ofs = 0;
									if(j1 == 1) ofs = c[x];
									chmin(dp[y][x][i1][j1], dp[y][x-1][i2][j2] + ofs);
								}
							}
						}
					}
				}
			}
		}
	}

	ll ans = INF64;
	for(i1 = 0; i1 < 2; i1++)
	{
		for(j1 = 0; j1 < 2; j1++) chmin(ans, dp[H-1][W-1][i1][j1]);
	}
	cout << ans << endl;

	return 0;
}
