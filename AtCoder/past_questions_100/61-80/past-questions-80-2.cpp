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

// 「分野別 初中級者が解くべき過去問精選 100問」の問題80
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/gigacode-2019/tasks/gigacode_2019_d

/*
 * past-questions-80.cppではO(H^2*W^2)だったが、これをO(H^2*W*logW)で解く版。
 * 二分探索を用いる。
 *   https://scrapbox.io/procon-kirokuyou/GigaCode2019_D_-_%E5%AE%B6%E3%81%AE%E5%BB%BA%E8%A8%AD_(100)
 *   https://atcoder.jp/contests/gigacode-2019/editorial/10167
 * 
 * 購入する土地の左上のマス(i,j)と高さhについて全探索する。
 * 幅wを伸ばしていくと支払うコストは単調増加になることから、これは二分探索で解ける。
 * 
 * 二分探索の初期値に注意。
 * マス(i,j)から幅wの区画を購入する、つまり[j,j+w)の場合、
 *   wの最小値は0. 1マスも買えないことがあるため。
 *   wの最大値はW-j+1. 全マス買えるケースがあるため、その+1．
 * を初期値とする必要がある。
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
	ll i, j;
	ll H, W, K, V; cin >> H >> W >> K >> V;
	PrefixSum_2D<ll> s(H, W);
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++) cin >> s.n[i][j];
	}
	s.Build();

	ll ans = 0;
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)  // 左上(i,j)
		{
			for(ll h = 1; h+i <= H; h++)  // 購入する土地の高さh
			{
				auto check = [&](ll x) -> bool  // 購入する土地の高さx
				{
					ll tmp1 = s.Sum(i, i+h, j, j+x);
					ll tmp2 = h*x * K;
					return tmp1+tmp2 <= V;
				};

				ll ok = 0;
				ll ng = W-j+1;
				while(ng-ok > 1)
				{
					ll mid = (ok+ng)/2;
					if(check(mid)) ok = mid;
					else ng = mid;
				}
				chmax(ans, h*ok);
			}
		}
	}
	cout << ans << endl;

	return 0;
}
