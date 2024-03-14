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

// 競プロ典型90問:19 https://atcoder.jp/contests/typical90/tasks/typical90_s

/*
 * 区間DPで解く。
 * 以下、入力の時点でNを2倍にしている。
 * 
 * dp[l][r]を以下のように定義する。
 *   a[l],a[l+1],...,a[r-1] つまり区間[l,r)におけるコスト総和の最小値
 * dp[0][N]が答。
 * 
 * dp[l][r]の分割は以下2パターンある。
 * (1)[l,r)を前半i個、後半(r-l-i)個に分ける。
 *    iは2,4,6,... と2ずつ増やしていく。
 * (2)まず[l+1,r-1)を取り除き、最後に両端の{l,r-1}を取り除く。
 *    両端をくっつけるイメージ。
 * それぞれのコストを計算し、その最小値を採用する。
 * 
 * [ACまでの思考の流れ]
 * ・区間の操作なので、区間DPかなと考えた。
 * ・[典型]区間の除去・圧縮・合体などが生じるケースでは区間DPを考えてみる。
 *   このとき、dp[l][r]として半開区間[l,r)で考えると実装しやすそう。
 *   https://algo-logic.info/range-dp/
 */

vector<vector<ll>> dp;
vector<ll> a;

// メモ化再帰を用いてdp[l][r]を返す
ll rec(int l, int r)
{
	ll &d = dp[l][r];
	if(d != -1) return d;  // メモ済み
	if(r-l == 2)  // ベースケース
	{
		d = abs(a[l] - a[r-1]);
		return d;
	}

	d = INF64;
	ll n1, n2;
	for(int i = l+2; i < r; i+=2)
	{
		// [l,i)と[i,r)に分ける
		n1 = rec(l, i);
		n2 = rec(i, r);
		chmin(d, n1+n2);
	}
	// 両端とその間で分ける
	n1 = abs(a[l] - a[r-1]);
	n2 = rec(l+1, r-1);
	chmin(d, n1+n2);

	return d;
}

int main(void)
{
	int i;
	int N; cin >> N;
	N*=2;
	a.resize(N);
	for(i = 0; i < N; i++) cin >> a[i];
	dp.resize(N+1, vector<ll>(N+1, -1));

	cout << rec(0, N) << endl;
	
	return 0;
}
