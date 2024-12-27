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

// ABC354 https://atcoder.jp/contests/abc354

/*
 * コンテスト時に解けず、解説を見た。
 * LISの気持ちになって、二分探索で解く。
 * 
 * 基本的には以下の通り。
 *   https://atcoder.jp/contests/abc354/editorial/10027
 *   https://x.com/pixy_kyopro/status/1791830820073934890
 * 
 * LISの考え方として、以下がある。
 * ＞考え方として、dp[i][j]を "i個目まで見て、長さjであるLISの末尾の値の最小値" というのを覚えておく。
 * つまりin-placeでdp[*]で考えたときに、
 * a[i]がdp[idx]に当てはまる場合、LISにa[i]を使うならば左にidx個ある、となる。
 * 
 * A[i]を反転＆-1倍して同じことをやると、右に何個あるか、となる。
 * (実装では2ループさせて左側、右側を同一処理にて行っている)
 * (左側)+(右側)+1がLISの長さであれば、条件を満たす。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・LISの気持ちをしっかり学ぶ。
 * ・a[i]がdp[idx]に当てはまる場合、LISにa[i]を使うならばLISのidx個目である。
 */

// LISの長さを求める
int lis_len(vector<int> &a)
{
	int i;
	int n = (int)a.size();
	vector<int> lis(n, INF32);
	for(i = 0; i < n; i++)
	{
		int idx = lower_bound(lis.begin(), lis.end(), a[i]) - lis.begin();
		lis[idx] = a[i];
	}
	return lower_bound(lis.begin(), lis.end(), INF32) - lis.begin();
}

void solve(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	vector<int> lis(N, INF32);
	vector<vector<int>> pos(2, vector<int>(N, 0));  // pos[*][i]: i番目の要素をLISに使うとき、何番目か  左側からと右側から
	for(int _ = 0; _ < 2; _++)  // 0:左側から 1:右側から
	{
		for(i = 0; i < N; i++)
		{
			int idx = lower_bound(lis.begin(), lis.end(), a[i]) - lis.begin();
			lis[idx] = a[i];
			pos[_][i] = idx;
		}
		// 反転＆-1倍
		reverse(a.begin(), a.end());
		for(auto &e : a) e = -e;
	}

	auto len = lis_len(a);
	reverse(pos[1].begin(), pos[1].end());
	vector<int> ans;
	for(i = 0; i < N; i++)
	{
		int tmp = pos[0][i] + pos[1][i] + 1;
		if(tmp == len) ans.push_back(i+1);
	}
	int sz_ = (int)ans.size();
	cout << sz_ << endl;
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;
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
