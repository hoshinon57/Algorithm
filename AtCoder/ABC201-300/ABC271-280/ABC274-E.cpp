#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// ABC274 https://atcoder.jp/contests/abc274

/*
 * コンテスト中に溶けず、解説を見て実装した。
 * bitDPで解く。
 * 
 * dp[S][v]を以下のように定義する。
 *   (N+M)ビット幅のSを、上位ビット側を街(N), 下位ビット側を宝箱(M)として割り当てて、
 *   1になっているビットに対して好きな順に辿った時の、かかる時間の最小値
 *   ただし最後にvに行く、という制約
 *   未探索であればINF64 (ただしdouble型のため、==ではなく幅を持たせて判定する)
 *   S:(N+M)ビット幅
 *   v:0～(N+M)-1
 * 
 * 通常の巡回セールスマン問題に加えて、移動時間を計算する際に
 * 現在何個のブースターを持っているかをSを元に取得して計算する。
 * これをメモ化再帰にて実装する。
 * 
 * 宝箱の取得は自由であることから、呼び出し元では
 * ・各宝箱について取る/取らないの全パターン
 * ・街と宝箱合わせて、最後に辿る地点について全探索
 * の2重ループにて最短時間を求める。
 * 
 * 計算量は、
 *   dpの遷移数が2^(N+M)*(N+M)
 *   各dp状態における更新にてO(N+M)
 * であることから、O((N+M)^2 * 2^(N+M)) …な気がするが、
 * 公式解説における
 * ＞「訪問する宝箱の集合を固定し、2^M通りそれぞれについてTSPを解く」とした場合、計算量は O((N+M)^2 * 2^N * 3^M)
 * こちらに該当するような気もする。
 */

// dp[S][v]
//   (N+M)ビット幅のSを、上位ビット側を街(N), 下位ビット側を宝箱(M)として割り当てて、
//   1になっているビットに対して好きな順に辿った時の、かかる時間の最小値
//   ただし最後にvに行く、という制約
//   未探索であればINF64 (ただしdouble型のため、==判定ではなく多少幅を持たせて判定する)
// S:(N+M)ビット幅
// v:0～(N+M)-1
vector<vector<double>> dp;

int N, M;
vector<int> x, y;  // x[i]:0～(M-1)は宝箱の、M～(N+M-1)は街のx座標  yも同様

// dp[S][v]におけるSを用いて、ブースターを何個取得しているか返す
int speed(int S)
{
	// N+Mビットで、下位側Mビットが宝箱
	// 下位側Mビットが全て1であるようなビットマスク：(1<<M)-1   例)M=3ならb'111
	int booster_num = __builtin_popcount(S & ((1<<M)-1));

	return 1<<booster_num;
}

// 地点iとjの距離を返す
// ただし0～(M-1)は宝箱を、M～(N+M-1)は街を指す
// また、i=-1は原点を指す
double dist(int i, int j)
{
	double x1, y1, x2, y2;
	x1 = x[i];
	y1 = y[i];
	x2 = x[j];
	y2 = y[j];
	if(i == -1)  // 原点
	{
		x1 = y1 = 0;
	}

	return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

// dp[S][v]を、メモ化再帰を用いて計算して返す
double func(int S, int v)
{
	double &d = dp[S][v];

	// メモ化再帰
	if(d < INF64-100) return d;  // double型のため、少し余裕を持たせて判定

	// 1になっているビットが1つ、つまり最初の地点の場合
	if(S == (1<<v))
	{
		d = dist(-1, v);  // 原点からの距離を計算  この時点でブースターは0個
		return d;
	}

	// vに行く前のSの状態を取得する
	int prev_S = S &(~(1<<v));
	
	// dp[prev_S][i] -> dp[S][v]を計算する
	for(int i = 0; i < N+M; i++)
	{
		if( (prev_S & (1<<i)) == 0) continue;  // ビットiが立っていなければスルー

		// dp[S][v]を求めたい
		// 地点vを除いた集合の移動時間(終点i) + i->vの移動時間 となる

		// dp[S][v] = dp[S-{v}][i] + (i->vの距離)/(ブースターの数)
		double tmp;
		tmp = func(prev_S, i) + dist(i, v) / speed(prev_S);
		d = min(d, tmp);
	}

	return d;
}

int main(void)
{
	int i, j;
	cin >> N >> M;
	// 前半M個は宝箱の、後半N個は街の座標とする
	x.resize(N+M);
	y.resize(N+M);
	for(i = 0; i < N; i++)
	{
		cin >> x[i+M] >> y[i+M];
	}
	for(i = 0; i < M; i++)
	{
		cin >> x[i] >> y[i];
	}
	dp.resize(1<<(N+M), vector<double>(N+M, INF64));

	int N_mask = ((1<<(N+M))-1) - ((1<<M)-1);  // N+Mビットについて、上位側Nビットが全て1
	double answer = INF64;
	for(i = 0; i < (1<<M); i++)  // 各宝箱について取る／取らないの全パターン
	{
		int S = N_mask | i;
		for(j = 0; j < N+M; j++)  // 最後に辿る地点について全探索
		{
			if( (S&(1<<j)) == 0) continue;  // jがSに含まれていなければスルー

			double tmp = func(S, j);
			tmp += dist(-1, j) / speed(S);  // 最後に原点に戻る
			answer = min(answer, tmp);
		}
	}
	cout << std::fixed << std::setprecision(10);
	cout << answer << endl;

	return 0;
}
