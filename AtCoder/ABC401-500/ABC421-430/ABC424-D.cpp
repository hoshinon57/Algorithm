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

// ABC424 https://atcoder.jp/contests/abc424

/*
 * コンテスト時に解けず、解説を見た。
 * DPで、bitDPになるのかな？ で解く。
 *   https://atcoder.jp/contests/abc424/editorial/13947
 * 
 * まず、最も外側のマスは白く塗る必要は無い。
 * そこを白に塗って最小値が求まるのなら、塗るマスを1つ内側にしても2x2の条件は満たせるため。
 * 
 * また、入力を拡張してH=7,W=7固定で考えた方が、実装は楽だった。
 * 拡張したマスは白にしておく。
 * 以下その前提で記述する。
 * 
 * 行を上から見ていったときに、
 *   現在注目している行の操作内容
 *   1つ前の行の操作内容
 * が分かれば、1つ前の行を2x2の上側のマスにしたケースにて、条件を満たすかが確定する。
 * (もっと言えば、その前後の行の操作は、結果に影響しない)
 * 
 * 0-indexedで、dp[i][s]を以下のように定義する。
 *   i行目まで見て、  (i=0～6)
 *   s:横方向の1マス目～5マス目の操作有無をbitで表現したときの、 (s=0～2^5-1)
 *   (i-1)行目まで条件を満たすような操作回数の最小値。
 *   条件を満たさない場合はINF.
 * 
 * (i-1)行目について、dp[i][s]とdp[i-1][ps]から決まるイメージ。
 * s,psは組み合わせを全探索することになるが、2^10=1024より、
 * 行数やテストケース数を踏まえても十分に間に合う。
 * 
 * 条件を満たすかは関数化した。chk()を参照。
 * これを使うと、外周の行をチェックする際もうまく使える。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・上の行から順に見ていったときに、前の行と今見ている行の操作内容が分かれば、前の行の結果が確定する。
 *   「直近3つの状態から確定する」ときに、DPでは直近2つの状態を持って、1つ前の状態と組み合わせる典型がある。
 *   それに近いと思う。
**/

bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)

void solve(void)
{
	// 0-indexed
	// H,Wは7固定で考える (拡張分は白マス)
	int i, j;
	ll H, W; cin >> H >> W;
	vector<vector<ll>> m(7, vector<ll>(7, 0));  // 0:白, 1:黒
	for(i = 0; i < H; i++)
	{
		string s; cin >> s;
		for(j = 0; j < W; j++)
		{
			if(s[j] == '#') m[i][j] = 1;
		}
	}

	ll S = 1<<5;  // 32
	vector<vector<ll>> dp(7, vector<ll>(S, INF64));  // dp[7][1<<5]
	dp[0][0] = 0;  // 0行目は操作しないので、無操作にあたる[0]=0.
	for(auto &e : dp[0]) e = 0;  // dp[0][*]=0

	// y-1行目の操作をps, y行目をsとしたとき、y-1行目を左上とした2x2が合法ならtrue
	auto chk = [&](ll y, ll s, ll ps) -> bool
	{
		ll x;
		vector<vector<ll>> tmp(2, vector<ll>(7, 0));
		tmp[0] = m[y-1];
		tmp[1] = m[y];
		for(x = 1; x <= 5; x++)
		{
			// 操作するなら白にする
			if(isbiton(ps, x-1)) tmp[0][x] = 0;
			if(isbiton(s, x-1))  tmp[1][x] = 0;
		}
		for(x = 0; x <= 5; x++)  // (y-1,x)を2x2の左上
		{
			if(tmp[0][x] && tmp[0][x+1] && tmp[1][x] && tmp[1][x+1]) return false;
		}
		return true;
	};
	ll s, ps;
	for(i = 1; i <= 6; i++)  // i行目 i=6は操作しない
	{
		for(s = 0; s < S; s++)  // sのbitが1なら白に塗る
		{
			if(i == 6 && s != 0) continue;  // 6行目は、この行の操作は0固定
			for(ps = 0; ps < S; ps++)  // 前の行の操作
			{
				if(i == 1 && ps != 0) continue;  // 1行目は、前の行の操作は0固定
				if(chk(i, s, ps))
				{
					chmin(dp[i][s], __builtin_popcountll(s) + dp[i-1][ps]);
				}
			}
		}
	}

	// dp[6][*]の最小値が答
	ll ans = *min_element(dp[6].begin(), dp[6].end());
	cout << ans << endl;
}

int main(void)
{
	int t; cin >> t;
	while(t > 0)
	{
		t--;
		solve();
	}

	return 0;
}
