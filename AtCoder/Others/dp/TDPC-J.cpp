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

// TDPC-J https://atcoder.jp/contests/tdpc/tasks/tdpc_ball

/*
 * 期待値DP. &bitDP.
 * 
 * dp[b]を
 *   座標をbit位置に割り当てて(bit0～bit15)、bitが1ならそこの座標に物があるとする。
 *   この状態から全て倒すまでの回数の期待値。
 * と定義する。
 * 物の座標が0～15なので、状態数は2^16.
 * 
 * DP遷移を考える。
 * ボールで狙う場所は自由に選べるので、
 * 全ケースについて計算して最善のものを採用する。[典型]
 * 位置iを狙うとき、
 *   dp[*] = Σ(dp[位置kのボールを倒した状態]+1)/3   (k=i-1～i+1)
 * こんなイメージ。
 * 
 * ただし位置kにボールが無いことがあり、このとき自己ループになるのが厄介。
 * 非自己ループの数をcnt, dp値の和をsumとすると、いろいろ式変形して
 *   dp[] = (sum+3)/cnt
 * となる。
 * cnt=0,つまり非自己ループが無いときはスルー。
 * 
 * 式変形の結果は以下の典型にも通ずる。
 * ＞確率DP,期待値DPで自己ループ(失敗パターン)があるとき、確率pで非自己ループ(成功パターン)になるとすると、
 * ＞非自己ループになるまでの試行回数の期待値は 1/p となる。
 * ＞例えば1/3で非自己ループになるなら、試行回数は 1/(1/3)=3 を乗算する。4/5で非自己ループなら 1/(4/5)=1.25倍。
**/

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

const ll MX = 1<<16;
vector<double> dp;
vector<bool> memo;

// dp[b]を返す
double f(ll b)
{
	auto &d = dp[b];
	if(memo[b]) return d;
	if(b == 0) return 0.0;

	ll i;
	d = INF64;
	for(i = 0; i < 16; i++)  // i中心に投げる
	{
		double sum = 0.0;
		ll cnt = 0;  // 非自己ループのdp総和、数
		for(ll x = i-1; x <= i+1; x++)  // xに当たるとする
		{
			if(x < 0 || x >= 16) continue;
			if(!isbiton(b, x)) continue;  // そこにボールが無い
			ll tmp = b;
			unbit(tmp, x);
			sum += f(tmp);
			cnt++;
		}
		if(cnt == 0) continue;  // 非自己ループ無し
		chmin(d, (sum+3)/cnt);
	}

	memo[b] = true;
	return d;
}

int main(void)
{
	ll i;
	dp.resize(MX);
	memo.resize(MX, false);
	ll N; cin >> N;
	ll b = 0;
	for(i = 0; i < N; i++)
	{
		ll x; cin >> x;
		b |= (1LL<<x);
	}

	cout << std::fixed << std::setprecision(10);
	cout << f(b) << endl;

	return 0;
}
