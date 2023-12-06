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

// ABC331 https://atcoder.jp/contests/abc331

/*
 * コンテスト中に解ききれなかった。
 * 2次元累積和を使って解く。
 * 考え方は公式解説と同じ。
 *   https://atcoder.jp/contests/abc331/editorial/7822
 * 
 * まず、C'=C+1, D'=D+1 としておく。
 * y:[0,y), x:[0,x) の範囲に含まれる黒マスの数を返す関数calc()があるとすると、
 *   calc(C', D') - calc(C', B) - calc(A, D') + calc(A, B)
 * がクエリの答になる。これは2次元累積和の考え方。
 * 
 * calc()の実装については、上記解説が分かりやすい。
 * y:[0,y), x:[0,x) の範囲を、周期をNとして
 *   N*Nのフルで囲まれている部分
 *   Y方向がNマス、X方向がN未満の端数
 *   Y方向がN未満の端数、X方向がNマス
 *   Y,Xともに端数
 * の部分に分けて計算する。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・コンテスト中に方針は立ったものの、細かい部分でバグ散らかしてしまった。
 *   「累積和は半開区間で考える」を守ればACできたか。大反省。
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
	ll N, Q; cin >> N >> Q;
	PrefixSum_2D<ll> sum(N, N);
	for(i = 0; i < N; i++)  // y
	{
		string s; cin >> s;
		for(j = 0; j < N; j++)  //x 
		{
			if(s[j] == 'B') sum.n[i][j] = 1;
		}
	}
	sum.Build();

	// y:[0,y), x:[0,x) の範囲に含まれる黒マスの数を返す
	auto calc = [&](ll y, ll x) -> ll
	{
		ll yfull = y/N, yzan = y%N;  // full:Y方向について、周期Nの個数  zan:周期Nの余り
		ll xfull = x/N, xzan = x%N;
		ll ans = 0;
		ans += sum.Sum(0, N, 0, N) * yfull * xfull;  // N*Nかたまりの個数
		ans += sum.Sum(0, N, 0, xzan) * yfull;  // Y方向が周期のN個、X方向がN未満の端数
		ans += sum.Sum(0, yzan, 0, N) * xfull;
		ans += sum.Sum(0, yzan, 0, xzan);  // Y,X方向それぞれN未満の端数
		return ans;
	};
	while(Q > 0)
	{
		ll A, B, C, D; cin >> A >> B >> C >> D;
		C++; D++;
		ll ans = 0;
		// 2次元累積和の考え方で足し引き
		ans  = calc(C, D);
		ans -= calc(C, B);
		ans -= calc(A, D);
		ans += calc(A, B);
		cout << ans << endl;
		Q--;
	}

	return 0;
}
