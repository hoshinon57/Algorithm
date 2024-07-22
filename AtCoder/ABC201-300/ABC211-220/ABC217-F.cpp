#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC217 https://atcoder.jp/contests/abc217

/*
 * 区間DPとまでは分かったが、実装しきれず解説を見た。
 *   https://x.com/kyopro_friends/status/1434154186862579719
 *   https://note.com/syamashi/n/nfe6806c0acca
 * 
 * dp[l][r]を
 *   半開区間[l,r)における通り数
 * として定義する。
 * (半開区間なので、r=N*2まで取るため配列の要素確保に注意)
 * 求める値はdp[0][N*2].
 * 
 * 再帰にて、区間dp[l][r]からの遷移を考える。ここが思いつかなかった。
 * lを固定し、lとペアになる生徒iを全探索する。すると
 *   (1) lとi
 *   (2) [l+1,i)
 *   (3) [i+1,r)
 * の3グループに分けられる。(2)は(1)に包含されているイメージ。
 * (2),(3)の通り数はそれぞれdp[l+1][i], dp[i+][r]となる。
 * また(1)(2)(3)をどういう順番に操作していくかを考える必要があり、
 *   全体の操作回数:(r-l)/2回
 *   (1)(2)を合わせたの操作回数:(i+1-l)/2回
 * となるので、Combinationを使って計算できる。
 * ※(1)は(2)より後ろに置く必要があるが、上記でうまくいく。
 * これらをかけてあげて、各iごとの和を取ると、dp[l][r]になる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・区間DPは思いついたが、生徒i,i+1を取り除いたときに、残りの生徒をうまくDPで表現できなかった。
 *   ⇒DP遷移にて「左端を固定して全探索」という発想ができなかった。覚えておく。
 *     左端を固定して取り除くと、残った要素についてDP管理がうまくできそう。
 */

const ll MOD = 998244353;
const ll NCK_SIZE = 401;  // N<=2*10^5 など、問題文の制約に合わせる
ll fact[NCK_SIZE], inv_fact[NCK_SIZE], inv[NCK_SIZE];

// Combination用の事前計算
void nCk_init(void)
{
	fact[0] = fact[1] = 1;
	inv[0] = inv[1] = 1;
	inv_fact[0] = inv_fact[1] = 1;

	for(int i = 2; i < NCK_SIZE; i++)
	{
		// 階乗
		fact[i] = fact[i - 1] * i % MOD;
		// 逆元
		inv[i] = MOD - inv[MOD%i] * (MOD / i) % MOD;
		// 逆元の階乗
		inv_fact[i] = inv_fact[i - 1] * inv[i] % MOD;
	}
}

// CombinationのnCkを求める
ll nCk(ll n, ll k)
{
	if (n < k) return 0;
	if (n < 0 || k < 0) return 0;

	ll x = fact[n];  // n!の計算
	ll y = inv_fact[n-k];  // (n-k)!の計算
	ll z = inv_fact[k];  // k!の計算
	
	return x * ((y * z) % MOD) % MOD;  //nCkの計算
}

int N;
vector<vector<bool>> ok;  // ok[i][j]:生徒i,jが仲良ければtrue
vector<vector<ll>> dp;  // dp[l][r]:半開区間[l,r)における通り数

// dp[l][r]を返す
ll func(int l, int r)
{
	ll &d = dp[l][r];
	if(d != -1) return d;

	if(l == r) { return d = 1; }  // 空区間は1通りとすると、再帰時の実装が楽
	if((r-l)%2 == 1) { return d = 0; }  // 区間が奇数はNG
	if(r-l == 2)  // ベースケース
	{
		if(ok[l][l+1]) d = 1;
		else d = 0;
		return d;
	}

	d = 0;
	for(int i = l+1; i < r; i++)  // 左端の生徒lとペアを組む相手iを全探索
	{
		if(!ok[l][i]) continue;
		ll pa = func(l+1, i);  // グループA [l+1, i)
		ll pb = func(i+1, r);  // グループB [i+1, r)
		// グループAに生徒{l,i}を加えた [l,i+1) を考える。
		// 操作回数は(r-l)/2回で、そのうち[l,i+1)は(i+1-l)/2回置くことになる。
		// 置き方は C((r-l)/2, (i+1-l)/2) 通り。
		ll c = nCk((r-l)/2, (i+1-l)/2);
		d += pa * pb % MOD * c % MOD;
		d %= MOD;
	}

	return d;
}

int main(void)
{
	nCk_init();

	// 0-indexed
	int i;
	int M; cin >> N >> M;
	ok.resize(N*2, vector<bool>(N*2, false));
	for(i = 0; i < M ; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		ok[a][b] = ok[b][a] = true;
	}
	dp.resize(N*2+1, vector<ll>(N*2+1, -1));  // 半開区間[l,r)のためrはN*2まで取る

	cout << func(0, N*2) << endl;

	return 0;
}
