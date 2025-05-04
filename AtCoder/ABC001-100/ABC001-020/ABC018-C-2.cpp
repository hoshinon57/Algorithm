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

// ABC013 https://atcoder.jp/contests/abc013

/*
 * 以下を参考に45度回転＆2次元累積和で解いた版。
 *   https://perogram.hateblo.jp/entry/2020/10/05/151022
 *   https://scrapbox.io/magurofly/%E3%83%9E%E3%83%B3%E3%83%8F%E3%83%83%E3%82%BF%E3%83%B3%E8%B7%9D%E9%9B%A2%E3%81%A845%E5%BA%A6%E5%9B%9E%E8%BB%A2
 *   https://algo-logic.info/how-to-think-cp/
 * 
 * (x,y)を(X=x+y,Y=x-y)と変換すると、
 * 変換前のマンハッタン距離は、max(|X1-X2|, |Y1-Y2|) のチェビシェフ距離になる。
 * するとある点がひし形の中央にできるかどうかは、
 *   [X-(K-1),X+(K-1)]
 *   [Y-(K-1),Y+(K-1)]
 * の範囲にxが存在するか、となる。
 * 変換後の座標で2次元累積和を構築して解く。
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
	int i;
	int y, x;
	int R, C, K; cin >> R >> C >> K;
	vector<string> mp(R);
	for(i = 0; i < R; i++) cin >> mp[i];

	const int geta = 1000;  // 変換後は負の値を取ることがあるので
	PrefixSum_2D<int> sum(3000, 3000);
	for(y = 0; y < R; y++)
	{
		for(x = 0; x < C; x++)
		{
			if(mp[y][x] != 'x') continue;
			int xx = x+y+geta;
			int yy = x-y+geta;
			sum.n[yy][xx] = 1;
		}
	}
	sum.Build();

	int ans = 0;
	for(y = 0; y < R; y++)
	{
		if(y < K-1 || y > R-K) continue;
		for(x = 0; x < C; x++)
		{
			if(x < K-1 || x > C-K) continue;
			if(mp[y][x] != 'o') continue;
			int xx = x+y+geta;
			int yy = x-y+geta;
			int tmp = sum.Sum(yy-(K-1), yy+(K-1)+1, xx-(K-1), xx+(K-1)+1);
			if(tmp == 0) ans++;
		}
	}
	cout << ans << endl;

	return 0;
}
