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

// ABC377 https://atcoder.jp/contests/abc377

/*
 * 解説を元に尺取り法で解く。
 *   https://x.com/kyopro_friends/status/1850174438026916332
 *   https://atcoder.jp/contests/abc377/editorial/11226
 * 
 * ※コンテスト時はmultisetを用いた実装だった。
 *   入力の[l,r]に対しrの一覧とlごとのr一覧を用意しておき、
 *   各lに対して条件を満たすrの範囲で加算していく感じ。
 *   https://atcoder.jp/contests/abc377/submissions/59173814
 * 
 * [l,r]が条件を満たすならば、[l+1,r]も条件を満たす。
 * よってrを固定したときに条件を満たす最小のlが分かれば、r-l+1がrに対する答となる。
 * rが右に1動くとき、新たなrが入力[l,r]のrと等しいものについて、
 * max(l, max(l)+1) を新たなlとすればよい。
 * 
 * 実装では半開区間としている。
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	vector<vector<int>> llist(M+1);  // llist[r]:入力[l,r]について、rとなるlを列挙
	for(i = 0; i < N; i++)
	{
		int l, r; cin >> l >> r;
		llist[r].push_back(l);
	}
	for(auto &e : llist) sort(e.begin(), e.end());

	ll ans = 0;
	// [l,r)で考える rの範囲は1～M+1
	int l = 1;
	for(int r = 1; r <= M+1; r++)
	{
		if(llist[r-1].size() != 0)  // [l,r)なので参照するのはr-1
		{
			chmax(l, llist[r-1].back() + 1);
		}
		ans += (ll)(r-l);
	}
	cout << ans << endl;

	return 0;
}
