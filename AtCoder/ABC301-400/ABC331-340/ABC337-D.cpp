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

// ABC337 https://atcoder.jp/contests/abc337

/*
 * 各行/各列についてそれぞれチェックしていく。
 * 各行ごとO(W), 各列ごとO(H)で判定できれば、全体でO(HW)となって間に合う。
 * 
 * 1行/1列抜き出した文字列sに対し、K個連続して'o'となるような操作回数を返す関数solve()を作成する。
 * 各行列ごとにsolve()を呼び出し、その最小値が答となる。
 * 
 * solve()は尺取り法で実装する。
 * 'o'と'x'の数をカウントしながら1マスずつずらしていく。
 * ※sの長さがKより小さい場合、そもそもNG. (NGにさせないと、以降の処理で添え字オーバーしてしまう)
 * 
 * [ACまでの思考の流れ]
 * ・H*Wについて制約があるので、O(HW)なら間に合う。
 *   なら各行、各列ごとに適切に実装すれば間に合いそう。
 * ・各行ごとの判定について、各マスごとKマス分のチェックをするとO(HWK)とかになってTLEする。
 *   1マスずらしても変化点が少ないことから、変化点に着目して尺取り法で解けそう。[差分に着目する典型]
 */

int H, W, K;
vector<string> mp;

// 文字列sに対し、K個連続して'o'となるような操作回数を返す
// K個連続できないならINF32を返す
int solve(string s)
{
	int i;
	int slen = s.size();
	if(slen < K) return INF32;

	int ok = 0, ng = 0;
	// 先頭K個を事前チェック
	for(i = 0; i < K; i++)
	{
		if(s[i] == 'o') ok++;
		if(s[i] == 'x') ng++;
	}
	int ans = INF32;
	// 1マスずつずらしながらチェック
	for(i = 0; i <= slen-K; i++)
	{
		if(ng == 0)  // 'x'が1つでもある区間はNG
		{
			chmin(ans, K-ok);
		}
		if(i+K < slen)  // ループの最後でなければ更新処理(ループ終端だと添え字オーバーする)
		{
			if(s[i] == 'o') ok--;
			if(s[i] == 'x') ng--;
			if(s[i+K] == 'o') ok++;
			if(s[i+K] == 'x') ng++;
		}
	}
	return ans;
}

int main(void)
{
	int i, j;
	cin >> H >> W >> K;
	mp.resize(H);
	for(i = 0; i < H; i++)  cin >> mp[i];

	int ans = INF32;
	// 各行チェック
	for(i = 0; i < H; i++)
	{
		chmin(ans, solve(mp[i]));
	}
	// 各列チェック
	for(i = 0; i < W; i++)
	{
		string t;
		for(j = 0; j < H; j++) t += mp[j][i];
		chmin(ans, solve(t));
	}
	if(ans == INF32) ans = -1;
	cout << ans << endl;

	return 0;
}
