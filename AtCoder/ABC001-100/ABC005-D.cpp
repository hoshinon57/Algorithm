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

// ABC005 https://atcoder.jp/contests/abc005

/*
 * 2次元累積和で解く。
 * 
 * まず、全ての長方形(位置、大きさ全パターン)について全探索し、美味しさの合計を求める。
 * これは長方形の探索パターンがO(N^4)で、
 * 美味しさの計算部分は2次元累積和を事前計算することでO(1)となり、
 * 全体でO(N^4)で計算できる。
 * 
 * これを、長方形の面積ごとに美味しさの最大値を保持しておく。
 * このとき、面積5よりも面積4の方が美味しい場合、面積4の値を面積5にコピーしておく。
 * 小さい方から最大値を伝搬させるイメージ。
 * 
 * あとは各クエリにおいて、上記の最大値を使えばO(1)で回答できる。
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
	int i, j;
	int N; cin >> N;
	PrefixSum_2D<int> sum(N, N);
	// 2次元累積和の構築
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			int d; cin >> d;
			sum.n[i][j] = d;
		}
	}
	sum.Build();

	vector<int> ans(N*N+1);  // ans[x]:面積x以下での美味しさ合計最大値
	int x, y;
	for(y = 0; y < N; y++)
	{
		for(x = 0; x < N; x++)
		{
			int dy, dx;  // 縦横取る個数
			for(dy = 1; y+dy <= N; dy++)
			{
				for(dx = 1; x+dx <= N; dx++)
				{
					int area = dy*dx;  // 取る面積
					ans[area] = max(ans[area], sum.Sum(y, y+dy, x, x+dx));
				}
			}
		}
	}
	// 小さい側の最大値を伝搬させる
	for(i = 1; i <= N*N; i++)
	{
		ans[i] = max(ans[i], ans[i-1]);
	}

	int Q; cin >> Q;
	while(Q > 0)
	{
		int P; cin >> P;
		cout << ans[P] << endl;
		Q--;
	}

	return 0;
}
