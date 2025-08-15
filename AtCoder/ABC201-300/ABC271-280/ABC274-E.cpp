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

// 2025/8に再解きしたものに置き換えた。

// ABC274 https://atcoder.jp/contests/abc274

/*
 * コンテスト中に溶けず、解説を見て実装した。
 * bitDPで解く。
 * 
 * dp[s][v]を以下のように定義する。
 *   s: 0:原点, 1～N:街, (N+1)～(N+M):宝に割り当てて、
 *   1になっているビットに対して好きな順に辿った時の、かかる時間の最小値
 *   ただし最後にvに行く、という制約
 * 
 * 通常の巡回セールスマン問題に加えて、移動時間を計算する際に
 * 現在何個のブースターを持っているかをSを元に取得して計算する。
 * これをメモ化再帰にて実装する。
 * 
 * 宝箱の取得は自由であることから、
 *   sのうち、原点および街のbitが全て0
 * をベースケースとして処理する。
 * つまり、ベースケースにて宝の取得状況は無視することになり、「宝の取得は自由」を表現できる。
 * これにより呼び出し元では1回呼び出すだけで良い。
 */

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

ll N, M;
vector<double> x, y;  // 0:原点, 1～N:街, N+1～N+M:宝
vector<vector<double>> dp;  // 負なら未探索

// dp[s][v]を返す
double f(ll s, ll v)
{
	auto &d = dp[s][v];
	if(d > -1.0) return d;
	d = INF64;

	if((s & ((1LL<<(N+1))-1)) == 0)  // ベースケース 原点および街に相当するbitがall 0であるとき(宝は気にしない)
	{
		if(v == 0) d = 0;
		else d = INF64;
		return d;
	}
	if(!isbiton(s, v))  // Sに頂点vが含まれていない
	{
		d = INF64;
		return d;
	}

	auto dist = [&](ll id1, ll id2) -> double
	{
		double dx = x[id1] - x[id2];
		double dy = y[id1] - y[id2];
		return sqrt(dx*dx+dy*dy);
	};
	auto spd = [&](ll ss) -> double
	{
		ss >>= N+1;
		ll cnt = M - __builtin_popcount(ss);  // 取得したブースターの数
		return (double)(1LL<<cnt);
	};

	ll s_prev = s;
	unbit(s_prev, v);
	for(ll u = 0; u < N+M+1; u++)
	{
		// dp(s,v) = dp(prev_s,u) + dist(u,v)
		auto tmp1 = f(s_prev, u);
		double tmp2 = dist(u, v) / spd(s_prev);
		chmin(d, tmp1+tmp2);
	}

	return d;
}

int main(void)
{
	ll i;
	cin >> N >> M;
	ll tot = N+M+1;
	x.resize(tot);
	y.resize(tot);
	for(i = 1; i <= N+M; i++)
	{
		cin >> x[i] >> y[i];
	}
	dp.resize(1LL<<tot, vector<double>(tot, -10.0));

	cout << std::fixed << std::setprecision(15);
	cout << f((1LL<<tot)-1, 0) << endl;

	return 0;
}
