#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC278 https://atcoder.jp/contests/abc278

/*
 * 2次元累積和のライブラリを作ったので、それを用いて解く版。
 * 解き方はABC278-E.cppと同じ。
 */

// 2次元累積和のライブラリ
// 使い方
//   (1)コンストラクタ(H, W) or コンストラクタ(void)+Init() にて、nとsumの初期化を行う。
//   (2)変数nに値を設定し、Build()にて累積和を構築する。
//   (3)Sum()にて半開区間の累積和を返す。
// 
// 複数個のオブジェクトを生成したい場合は以下のように定義する。
//   vector<PrefixSum_2D<int>> sum;
//   sum.resize(10, PrefixSum_2D<int>(H, W));  // sum[0]～sum[9]の10個
// 
// 100個などたくさんのオブジェクトを生成する場合、生配列で計算するより実行時間が倍以上になるため注意。
// sumの定義を T sum[105][105] のように生配列にすれば、かなり改善される。
// この場合、Init()のsum.assign()をコメントアウトする。
// 参考：ABC278-E
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

	// [y1,y2) x [x1,x2) の半開区間の累積和を返す
	T Sum(int y1, int y2, int x1, int x2)
	{
		return sum[y2][x2] - sum[y2][x1] - sum[y1][x2] + sum[y1][x1];
	}
};

vector<PrefixSum_2D<int>> sum;

int main(void)
{
	int i, j, k;
	int H, W, N; cin >> H >> W >> N;
	int h, w; cin >> h >> w;
	sum.resize(N+1, PrefixSum_2D<int>(H, W));
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			int a; cin >> a;
			sum[a].n[i][j] = 1;
		}
	}
	for(i = 1; i <= N; i++)
	{
		sum[i].Build();
	}

	for(i = 0; i+h <= H; i++)
	{
		for(j = 0; j+w <= W; j++)
		{
			int ans = 0;
			for(k = 1; k <= N; k++)
			{
				int n1 = sum[k].Sum(i, i+h, j, j+w);
				int n2 = sum[k].Sum(0, H, 0, W);
				if(n1 != n2)
				{
					ans++;
				}
			}
			cout << ans << " ";
		}
		cout << endl;
	}

	return 0;
}
