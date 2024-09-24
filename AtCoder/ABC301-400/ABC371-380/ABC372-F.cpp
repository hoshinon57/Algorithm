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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;

// ABC372 https://atcoder.jp/contests/abc372

/*
 * 動的計画法(in-place DP)で解く。
 * 基本は公式解説の通り。
 *   https://atcoder.jp/contests/abc372/editorial/10969
 *   https://x.com/kyopro_friends/status/1837490694619037997
 * 
 * グラフと制約をぐっと睨むと、移動先は次のマスがほとんどであり、
 * X->Yへの短縮パスの分だけ増えていくイメージ。
 * つまり各回の移動ごとに、1マスずれるだけでほとんど変化しない。
 * ここから
 *   dp[i]:k回目の移動の後、(i+k)マス目にいる通り数
 * とin-placeで定義すると、逆にaマス目はdp[a-k]で表せる。
 * 
 * X->Yの短縮パスは、(Y-X+1)だけ短縮できるという考えから、
 *   dp[X-k] -> dp[Y-1-k]
 * へDP値を加算すればうまくいく。
 * ただしin-place DPなので、いったん加算値は別要素に設定してからDP値を更新する必要がある。
 * (そうしないと、加算値がM回ループ中にダブルでカウントされるケースがあるため)
 * 
 * ※初めてmodintを使ってみた。
 * 
 * [ACまでの思考の流れ]
 * ・制約メタ読みから、O(MK)なら間に合いそう。
 * ・グラフなど色々書いてみると、X->Yへの短縮パスを除くと
 *   dp[0]=1が1行動ごとに1ずつずれていくイメージになる。
 *   X->Yのパスは、(Y-X+1)だけ短縮できるイメージ。
 * ・以上から、DP値はN要素で使いまわし、オフセットを用いて参照先を変えていくことで上手く解けそう、となった。
 */

int main(void)
{
	ll i;
	ll N, M, K; cin >> N >> M >> K;
	vector<ll> x(M), y(M);
	for(i = 0; i < M; i++)
	{
		cin >> x[i] >> y[i];
		x[i]--; y[i]--;
	}
	vector<mint> dp(N);
	dp[0] = 1;

	ll ofs = 0;
	for(ll k = 0; k < K; k++)
	{
		vector<pair<ll,mint>> add;  // add[i]:{dp[*]を加算する箇所, 加算値}
		// in-place DPなので、加算値を別要素に設定してからDP値を更新する
		for(i = 0; i < M; i++)
		{
			// dp[a] -> a+kマス目を指す
			// bマス目はどこか -> dp[b-k]
			ll from = (x[i]-ofs+N)%N;
			ll to = (y[i]-ofs-1+N+N)%N;
			add.push_back({to, dp[from]});
		}
		for(auto [idx, val] : add)
		{
			dp[idx] += val;
		}
		ofs = (ofs+1) % N;
	}
	mint ans = 0;
	for(auto &e : dp) ans += e;
	cout << ans.val() << endl;

	return 0;
}
