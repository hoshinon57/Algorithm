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

// ABC313 https://atcoder.jp/contests/abc313

/*
 * 2次元累積和＆二分探索で解く。
 * 別解解説を参考に、勉強のため実装してみた。
 * ※https://atcoder.jp/contests/abc311/editorial/6829
 * 
 * (i,j)を左上マスとしたときに、
 *   幅dの正方形が作れる
 *   幅d+1の正方形が作れない
 * の境界値が存在する。この境界値を二分探索で求める。
 * 判定問題である「正方形が作れる/作れない」は2次元累積和を使う。
 * 
 * 計算量は10^8ほどになるが、実行時間は410ms程度と、十分にACできる範囲であった。
 */

// 2次元累積和のライブラリ
// 使い方
//   (1)コンストラクタ(H, W) or H,Wを直接設定+Init() にて、nとsumの初期化を行う。
//   (2)変数nに値を設定し、Build()にて累積和を構築する。
//   (3)Sum()にて半開区間の累積和を返す。
template <typename T>
struct PrefixSum_2D
{
public:
	int H, W;
	vector<vector<T>> n;
private:
	vector<vector<T>> sum;  // 累積和

public:
	// H:高さ W:幅
	PrefixSum_2D(int H_, int W_) : H(H_), W(W_)
	{
		Init();
	}
	PrefixSum_2D(void) {}

	void Init(void)
	{
		n.assign(H, vector<T>(W, 0));
		sum.assign(H+1, vector<T>(W+1, 0));
	}

	// 累積和を計算する
	// n[][]へは事前に設定しておくこと
	void Build(void)
	{
		int i, j;
		for(i = 0; i < H; i++)
		{
			for(j = 0; j < W; j++)
			{
				sum[i+1][j+1] = sum[i+1][j] + sum[i][j+1] - sum[i][j] + n[i][j];
			}
		}
	}

	// nについて、[y1,y2) x [x1,x2) の半開区間の累積和を返す
	T Sum(int y1, int y2, int x1, int x2)
	{
		return sum[y2][x2] - sum[y2][x1] - sum[y1][x2] + sum[y1][x1];
	}
};

int main(void)
{
	// 0-indexed
	int i, j;
	int H, W, N; cin >> H >> W >> N;
	PrefixSum_2D<int> sum(H, W);
	for(i = 0; i < N; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		sum.n[a][b] = 1;
	}
	sum.Build();

	ll ans = 0;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			// (i,j)を左上マスとする
			if(sum.n[i][j] == 1) continue;  // 穴

			// (i,j)を左上マスとし、幅dの正方形を作れるならtrue
			auto check = [&](int d) -> bool
			{
				if(i+d > H || j+d > W) return false;  // 範囲外
				return (sum.Sum(i, i+d, j, j+d) == 0);
			};

			int l = 0;
			int r = 3005;  // H,W<=3000より
			while(r - l > 1)
			{
				int mid = (l+r)/2;
				if(check(mid))
				{
					l = mid;
				}
				else
				{
					r = mid;
				}
			}
			ans += l;
		}
	}
	cout << ans << endl;

	return 0;
}
