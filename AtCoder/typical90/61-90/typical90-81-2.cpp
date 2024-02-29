#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <functional>  // function
#include <limits>  // numeric_limits
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:81 https://atcoder.jp/contests/typical90/tasks/typical90_cc

/*
 * 公式解説を参考に、二次元累積和で解く。
 * 
 * 身長Ai,体重Biの生徒を(Ai,Bi)の二次元平面状にプロットしていくことで、
 *   a<=A<=a+K, b<=B<=b+K
 * となる範囲の生徒はチームを組める。
 * 
 * よって各生徒をプロットした後に二次元累積和を計算し、
 * 全てのK*Kの範囲の和を計算していけば、その最大値が答となる。
 */

// 2次元累積和のライブラリ
// 使い方
//   (1)コンストラクタ(H, W) or H,Wを直接設定+Init() にて、nとsumの初期化を行う。
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

	// nについて、[y1,y2) x [x1,x2) の半開区間の累積和を返す
	T Sum(int y1, int y2, int x1, int x2)
	{
		return sum[y2][x2] - sum[y2][x1] - sum[y1][x2] + sum[y1][x1];
	}
};

int main(void)
{
	int i, j;
	int N, K; cin >> N >> K;
	const int ABMAX = 5000;
	PrefixSum_2D<int> sum(ABMAX+5, ABMAX+5);
	for(i = 0; i < N; i++)
	{
		int a, b; cin >> a >> b;
		sum.n[a][b]++;
	}
	sum.Build();

	int ans = 0;
	for(i = 0; i <= ABMAX; i++)
	{
		if(i+K > ABMAX) break;
		for(j = 0; j <= ABMAX; j++)
		{
			if(j+K > ABMAX) break;
			int d = sum.Sum(i, i+K+1, j, j+K+1);
			chmax(ans, d);
		}
	}
	cout << ans << endl;

	return 0;
}
