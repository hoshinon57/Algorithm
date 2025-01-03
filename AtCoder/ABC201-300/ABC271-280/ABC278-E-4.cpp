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

// ABC278 https://atcoder.jp/contests/abc278

/*
 * 以下の解説を参考に、二次元いもす法で解いてみた版。
 *   https://atcoder.jp/contests/abc278/editorial/5269
 *   https://x.com/kyopro_friends/status/1593964352893509632
 * 
 * 各Aの値について、「塗りつぶす長方形の左上マスがどの範囲のとき、その値が全て塗りつぶされるか？」を考える。
 * まず、各Aの値のy,xの最小と最大を求めておく。
 * マス(i,j)を塗りつぶすためには、長方形の左上座標が
 *   y:[i-h+1,i], x:[j-w+1,j]
 * の範囲である必要があることから、線分の交わり判定と同じ要領で
 *   y:[ymax-h+1, ymin], x:[xmax-w+1, xmin]
 * の範囲に左上座標があれば、値Aが全て塗りつぶされる。
 * (上限下限クリップなどは必要)
 * 
 * 二次元いもす法にて、上記座標の左上と右下(の1つ先)を+1, 右上と左下を-1すればよい。
 * 
 * ただし二次元いもす法を使わずO(HWN)の実装でも、実行時間は変わらなかった。
 * 制約の値が小さいからと思われる。
 * Nが大きい場合は二次元いもす法の適用が必要になると思われる。
 */

#define IMOS  // 2次元いもす法を使ってO(HW+N)の場合は定義、使わずO(HWN)の場合は定義を外す

int main(void)
{
	// 値aについて、0-indexed
	int i, j;
	int H, W, N; cin >> H >> W >> N;
	int h, w; cin >> h >> w;
	vector<int> ymax(N, -INF32), ymin(N, INF32), xmax(N, -INF32), xmin(N, INF32);  // ymax[a]:値aのy座標max
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)  // (i,j)
		{
			int a; cin >> a;
			a--;
			chmax(ymax[a], i);
			chmin(ymin[a], i);
			chmax(xmax[a], j);
			chmin(xmin[a], j);
		}
	}

	int tot = 0;  // 登場する値の種類数 入力に対し1～N全ての値が登場するとは限らないため
	vector<vector<int>> ans(H, vector<int>(W, 0));  // ans[H][W]
	vector<vector<int>> imos(H+1, vector<int>(W+1, 0));
	for(i = 0; i < N; i++)
	{
		if(ymax[i] == -INF32) continue;
		tot++;
		int yst = max(ymax[i]-h+1, 0);
		int yed = min(ymin[i]+1, H);  // [yma,ymi)
		int xst = max(xmax[i]-w+1, 0);
		int xed = min(xmin[i]+1, W);
#ifdef IMOS
		if(yst >= yed) continue;
		if(xst >= xed) continue;
		imos[yst][xst]++;
		imos[yst][xed]--;
		imos[yed][xst]--;
		imos[yed][xed]++;
#else
		for(int y = yst; y < yed; y++)
		{
			for(int x = xst; x < xed; x++)
			{
				ans[y][x]++;
			}
		}
#endif
	}

#ifdef IMOS
	{
		int y, x;
		for(x = 0; x <= W; x++)
		{
			for(y = 0; y <= H-1; y++)
			{
				imos[y+1][x] += imos[y][x];
			}
		}
		for(y = 0; y <= H; y++)
		{
			for(x = 0; x <= W-1; x++)
			{
				imos[y][x+1] += imos[y][x];
			}
		}
	}
#endif

	for(i = 0; i <= H-h; i++)
	{
		for(j = 0; j <= W-w; j++)
		{

#ifdef IMOS
			cout << tot-imos[i][j];
#else
			cout << tot-ans[i][j];
#endif
			if(j != W-w) cout << " ";
		}
		cout << endl;
	}

	return 0;
}
