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

// ABC086 https://atcoder.jp/contests/abc086

/*
 * 2次元累積和で解く。hamayanhamayan氏とほぼ同じ実装方針だった。
 *   https://blog.hamayanhamayan.com/entry/2018/01/21/230717
 * 
 * まず盤面のループに着目する。
 * 縦横2Kでループするので、入力のx,yは2Kでmodを取ってしまってよい。
 * 
 * 2K*2Kの1ループ分について、左下の座標(i,j)を固定すると、
 *   (i  ,j  )からK*K -> 白
 *   (i+K,j  )からK*K -> 黒
 *   (i  ,j+K)からK*K -> 黒
 *   (i+K,j+K)からK*K -> 白
 * で塗ることになる。
 * 入力(x,y)について、縦横Kずつ足した計3つも点があるとすると、
 * 2次元累積和を用いて各(i,j)ごとにO(1)で計算ができる。
 * 
 * ※自分の実装では白黒それぞれの累積和を用意したが、
 *   白の(x,y)を黒の(x+K,y)に置き換えることで、累積和1つで済ませられる。なるほど…
 * 
 * [ACまでの思考の流れ]
 * ・盤面がループするなら、1ループ分ないし2ループ分で考える。
**/

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
	ll i, j;
	ll N, K; cin >> N >> K;
	PrefixSum_2D<int> sb(K*4,K*4), sw(K*4,K*4);  // それぞれ白と黒
	for(i = 0; i < N; i++)
	{
		ll x, y; cin >> x >> y;
		char c; cin >> c;
		x %= K*2;
		y %= K*2;
		if(c == 'B')
		{
			// modを取っているので、(x,y)は重複することがある。=1ではなく++が必要(1WA).
			sb.n[y    ][x    ]++;
			sb.n[y+K+K][x    ]++;
			sb.n[y    ][x+K+K]++;
			sb.n[y+K+K][x+K+K]++;
		}
		else
		{
			sw.n[y    ][x    ]++;
			sw.n[y+K+K][x    ]++;
			sw.n[y    ][x+K+K]++;
			sw.n[y+K+K][x+K+K]++;
		}
	}
	sb.Build();
	sw.Build();

	int ans = 0;
	for(i = 0; i < K*2; i++)  // y
	{
		for(j = 0; j < K*2; j++)  // x
		{
			int tmp = sw.Sum(i, i+K, j, j+K) + sw.Sum(i+K, i+K+K, j+K, j+K+K);
			tmp += sb.Sum(i, i+K, j+K, j+K+K) + sb.Sum(i+K, i+K+K, j, j+K);
			chmax(ans, tmp);
		}
	}
	cout << ans << endl;

	return 0;
}
