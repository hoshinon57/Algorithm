#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC344 https://atcoder.jp/contests/abc344

/*
 * コンテスト中に解けず、解説を見た。
 * 動的計画法で解く。
 * 
 * 基本は以下の公式解説の通り。
 *   https://atcoder.jp/contests/abc344/editorial/9473
 *   https://twitter.com/kyopro_friends/status/1766462419290001718
 * 
 * dp[y][x][Pmax:y][Pmax:x] を以下のように定義する。
 *   現在マス(y,x)まで来ており、
 *   それまでのPの最大値マスが(my,mx)であるときの、
 *   {移動回数, 残金} の最適値。
 *   ただし移動回数は小さい方が最善で、移動回数が同じときは残金が大きい方が最善。
 *   ※実装では大小比較の観点から、残金を-1倍して保持している
 * dp[N-1][N-1][*][*]の"移動回数"の最小値が答。
 * 
 * 配るDPで、nowD:dp[y][x][my][mx]から下方向への遷移を考える。(右方向の遷移も同様となる)
 * [mv,mon]=nowD とすると、移動するために
 *   mon+P[my][mx]*k >= D[y][x]
 * となる最小のkを求める。これは除算の小数点以下切り上げで求まる。
 * また遷移先のmy,mxは、現在のPmaxと移動先のPとの比較で求められる。
 * すると移動先は
 *   dp[y+1][x][my'][mx'] = {mv+1+k, mon+P[my][mx]*k-D}
 * となる。  ※小さい場合のみ更新すること
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・いやーこれ難しい、どうしたものか。
 *   コンテスト中は「半分全列挙を4回…？」とか思ってしまった。
 * ・DPを思いつくとして、状態はまず現在のマスは自明。
 *   あとは「補給するならこれまでのルートでのPmaxマスでやるべき」というのを思いつく必要がある。
 *   それが可能なら、dp[{現在のマス}][{これまでのルートのPmaxマス}] という状態を作れそう。
 */

const int NMAX = 80;
pair<ll,ll> dp[NMAX][NMAX][NMAX][NMAX];  // [y][x][Pmax:y][Pmax:x]  // {移動回数, 所持金の-1倍}
ll p[NMAX][NMAX], r[NMAX][NMAX], d[NMAX][NMAX];

int main(void)
{
	// 0-indexed
	int i, j, k, l;
	int N; cin >> N;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			cin >> p[i][j];
		}
	}
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N-1; j++)
		{
			cin >> r[i][j];
		}
	}
	for(i = 0; i < N-1; i++)
	{
		for(j = 0; j < N; j++)
		{
			cin >> d[i][j];
		}
	}
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			for(k = 0; k < N; k++)
			{
				for(l = 0; l < N; l++)
				{
					dp[i][j][k][l] = {INF64, 0};
				}
			}
		}
	}
	dp[0][0][0][0] = {0, 0};  // {移動0, 所持金0}

	// 配るDP
	for(int y = 0; y < N; y++)
	{
		for(int x = 0; x < N; x++)  // dp[y][x]
		{
			for(int my = 0; my < N; my++)
			{
				for(int mx = 0; mx < N; mx++)  // dp[y][x][my][mx] からの移動
				{
					auto [mv, mon] = dp[y][x][my][mx];
					mon = -mon;  // -1倍して持っているので
					ll pmax = p[my][mx];
					ll nxtMy, nxtMx;
					// 下方向
					if(y != N-1)
					{
						// mon+P*k >= D となる最小のkを求める  k:補給回数
						k = (d[y][x]-mon+(pmax-1)) / pmax;
						if(k < 0) k = 0;  // 下限0
						ll nxtMv = mv+1+k;
						ll nxtMon = mon+pmax*k-d[y][x];
						if(pmax < p[y+1][x])  // 移動先のマスが、これまでのルートで最もPが大きくなるか
						{
							nxtMy = y+1;
							nxtMx = x;
						}
						else
						{
							nxtMy = my;
							nxtMx = mx;
						}
						chmin(dp[y+1][x][nxtMy][nxtMx], {nxtMv, -nxtMon});  // moneyは-1倍して保持
					}

					// 右方向
					if(x != N-1)
					{
						// mon+P*k >= D となる最小のkを求める  k:補給回数
						k = (r[y][x]-mon+(pmax-1)) / pmax;
						if(k < 0) k = 0;  // 下限0
						ll nxtMv = mv+1+k;
						ll nxtMon = mon+pmax*k-r[y][x];
						if(pmax < p[y][x+1])  // 移動先のマスが、これまでのルートで最もPが大きくなるか
						{
							nxtMy = y;
							nxtMx = x+1;
						}
						else
						{
							nxtMy = my;
							nxtMx = mx;
						}
						chmin(dp[y][x+1][nxtMy][nxtMx], {nxtMv, -nxtMon});
					}
				}
			}
		}
	}

	ll ans = INF64;
	// dp[N-1][N-1][*][*]の最小値が答
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			ll cnt = dp[N-1][N-1][i][j].first;
			chmin(ans, cnt);
		}
	}
	cout << ans << endl;

	return 0;
}
