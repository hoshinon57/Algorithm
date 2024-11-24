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

// ABC381 https://atcoder.jp/contests/abc381

/*
 * コンテスト時に解けず、解説を見た。bitDPで解く。
 *   https://atcoder.jp/contests/abc381/editorial/11408
 *   https://x.com/kyopro_friends/status/1859958177707753539
 * 
 * 制約よりメタ読みでbit全探索かbitDPだろう、となる。
 * bitDPだとして、dp[S]を
 *   Sでbitが1になっているbitを1122数列に使うときの、
 *   何たらかんたら～～～
 * となる。
 * ではこれでどのような値を保持すればよいか。
 * 
 * ・"1122数列"としての最長
 *   ⇒Sのpopcountから求まるので、これを保持するのは無意味。
 * ・"1122数列"を作れるか否か
 *   ⇒S={1,2,3}のとき、再帰実装にてdp[{1,2}]に{3}を加えることを考えることになるが、
 *     上記だとそれが不可能。{3}を何文字目から使えるのかが分からないので。
 * ・Sにて"1122数列"を作ったとき、Aの何文字目まで使うか (正確には、次は何文字目から使うか)
 *   ⇒これならdp[{1,2}]に{3}を加えることもできそう。
 * 
 * DP遷移は、S={1,2,3}を例として
 *   dp[{1,2}]に{3}を加える
 *   dp[{1,3}]に{2}を加える
 *   dp[{2,3}]に{1}を加える
 * それぞれについて、「Aの何文字目までを使うか」の最小値をdp[{1,2,3}]に採用すればよい。
 * ここの実装はgetNext()も参照。ABC381-Eでも使用した。
 * dp[S]にて"1122数列"が作れない場合は、dp=INFとする。
 * 
 * 答は、dp[S]の全パターンについて、INFではないもの(=数列が作れるもの)について
 *   popcount(S)*2
 * の最大値が答。
 */

const int MAX_A = 20;  // 0-indexedに直して、Aは0～19まで

// dp[S]:Sでbitが1になっているbitを1122数列に使うときの、「何文字目まで使うか+1」を返す (=次はこの文字から使う)
// 数列が作れなければINF.
vector<int> dp;

vector<vector<int>> pos;  // pos[i] = {...} A=iの登場位置を列挙

// 登場位置を列挙しているposについて、位置st以降でnum個目の登場位置の"次の位置"を返す
// ⇒よって戻り値は"次に見るべき位置"となる
// num個なければINFを返す
int getNext(vector<int> &pos_, int st, int num)
{
	if(num == 0) return st;
	auto idx = lower_bound(pos_.begin(), pos_.end(), st) - pos_.begin();
	idx = idx + num - 1;  // num個目の登場位置
	if(idx >= (int)pos_.size()) return INF32;
	return pos_[idx]+1;
};

int bitDP(int S)
{
	if(dp[S] != -1) return dp[S];  // メモ化済み
	int &d = dp[S];

	// ベースケースは呼び出し元でdp[0]=0にて設定済み

	d = INF32;
	for(int i = 0; i < MAX_A; i++)
	{
		if(((S>>i)&1) == 0) continue;
		int S_prev = S & (~(1<<i));  // iに相当するビットを落とす
		// dp[S_prev] に A=iを加える
		int tmp = getNext(pos[i], bitDP(S_prev), 2);  // dp[S_prev]からA=iを2つ取ってくる
		chmin(d, tmp);  // {S}にて1122文字列が作れるなら、なるべく左側で作り切れた方が良い よってmin
	}

	return d;
}

int main(void)
{
	int i;
	int N; cin >> N;
	pos.resize(MAX_A);
	for(i = 0; i < N; i++)
	{
		int a; cin >> a;
		a--;
		pos[a].push_back(i);
	}
	dp.resize(1<<MAX_A, -1);
	dp[0] = 0;

	int ans = 0;
	for(i = 0; i < (1<<MAX_A); i++)
	{
		if(bitDP(i) != INF32)  // INF32はdp[i]にて1122文字列を作れない
		{
			chmax(ans, __builtin_popcount(i) * 2);
		}
	}
	cout << ans << endl;

	return 0;
}
