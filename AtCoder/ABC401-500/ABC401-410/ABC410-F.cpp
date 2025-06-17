#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <unordered_map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC410 https://atcoder.jp/contests/abc410

/*
 * 自力で解けず、解説を見た。
 * 二次元累積和をベースに、配列使いまわしをして解く。
 * 
 * 基本は公式解説の通り。
 *   https://atcoder.jp/contests/abc410/editorial/13301
 * H*Wに関して制約があるので、小さい方がHになるよう必要に応じて回転。
 * 長方形の上辺と下辺を固定して全探索すると、ここまででまず O(H*HW) となる。
 * 
 * 累積和は#を+1,.を-1にして構築する。
 * 各全探索について、x方向を[0,xx)について累積和を取ると、
 * 累積和に同じものがある＝その区間の総和が0,となる。[典型]
 * 
 * ただし累積和の登場回数カウント(以降cnt)に単にlogが付くようなことをするとTLE.
 * ※unordered_mapにて、reserveしたり#pragmaしたりしてもTLEが1つ残った
 *   https://atcoder.jp/contests/abc410/submissions/66852199
 * 公式解説の通り、cntを使いまわして変化したところだけを元に戻す、という方針でAC.
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・H*Wについて制約があったとき、H<=WとしてHについて全探索するテクニック。
 * ・配列の使いまわしテクニック。変化したところを記録しておき、後でそこだけ元に戻す。
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

// a[Y][X]を時計回りに90度回転させたものを返す (戻り値のサイズはa[X][Y]になる)
template <typename T>
vector<vector<T>> rotate_2(vector<vector<T>> &a)
{
	int y = (int)a.size();
	int x = (int)a[0].size();
	vector<vector<T>> a_r(x, vector<T>(y));  // a_r[x][y]
	for(int i = 0; i < y; i++)
	{
		for(int j = 0; j < x; j++)  // a[i][j]がどこに移動するか
		{
			a_r[j][y-i-1] = a[i][j];
		}
	}
	return a_r;
}

void solve(void)
{
	int i, j;
	int H, W; cin >> H >> W;
	vector<vector<int>> a(H, vector<int>(W, 0));  // a[H][W]
	for(i = 0; i < H; i++)
	{
		string s; cin >> s;
		for(j = 0; j < W; j++)
		{
			if(s[j] == '#') a[i][j] = 1;
			else a[i][j] = -1;
		}
	}
	if(H > W)  // H<=Wにする
	{
		a = rotate_2(a);
		swap(H, W);
	}

	PrefixSum_2D<ll> s(H, W);
	for(i = 0; i < H; i++)
	{
		for(j = 0; j < W; j++)
		{
			s.n[i][j] = a[i][j];
		}
	}
	s.Build();

	ll y1, y2, xx;
	ll ans = 0;
	vector<int> cnt(H*W*2+1);  // -H*W～+H*W  cnt[x]:総和がxになる個数 毎回のループで使いまわす
	const int ofs = H*W;
	for(y1 = 0; y1 < H; y1++)
	{
		for(y2 = y1+1; y2 <= H; y2++)  // yについて[y1,y2)
		{
			cnt[0+ofs] = 1;
			vector<int> li;  // cntを使いまわすので、変化したところだけを元に戻す用
			for(xx = 1; xx <= W; xx++)
			{
				ll sum = s.Sum(y1, y2, 0, xx);
				ans += cnt[sum+ofs];
				cnt[sum+ofs]++;
				li.push_back(sum+ofs);
			}
			for(auto &e : li) cnt[e] = 0;  // cntが変化したところを元に戻す
		}
	}
	cout << ans << endl;
}

int main(void)
{
	int T; cin >> T;
	while(T > 0)
	{
		T--;
		solve();
	}

	return 0;
}
