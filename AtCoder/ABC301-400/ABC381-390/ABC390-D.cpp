#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <unordered_set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC390 https://atcoder.jp/contests/abc390

/*
 * コンテスト時に解けず、解説を見た。
 * 深さ優先探索による全探索で解く。
 * 
 * ベル数、というらしい。
 * N個の要素をグループ分けする。これはDFSにて
 *   既に存在するグループに追加する
 *   新しいグループを作り、そこに追加する
 * という流れとなる。
 * N=12のときグループ分けやり方は4*10^6ほどであり、全探索できる。
 * 類題：ABC310-D(Peaceful Teams)
 * 
 * XORした結果を保持するとき、unordered_setが主流(？)かと思う。
 * しかしvectorにて積んでいき、最後にソート＆重複削除、というテクニックもある。
 * ⇒unordered_setは1271ms, vectorで最後にソート＆重複削除は385msだった。
 *   本ソースコードでは後者としている。
 * 
 * [補足]
 * ・コンテスト時、「部分集合の部分集合」によるbitDPを考えた。
 *   集合SをS1,S2に分け、S2に該当するa[i]を1つの袋にまとめる、という方針。
 *   しかしこれはどうやってもTLEだった。
 *   (unordered_setではなくvectorにて最後に重複削除、色々最適化などやってもダメだった)
 *   Twitterを見たらこの方針で解いた人がわずかに見つかったが…
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・全探索できないか、を考える。基本は全探索[典型].
 * ・N個の要素をグループ分けするテクニック。ベル数というらしい。
 */

int main(void)
{
	int N; cin >> N;
	vector<ll> a(N); for(int i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> ans;

	// li:現在作ったグループの石数のリスト
	// idx:次に見るa[idx]
	auto dfs = [&](auto &self, vector<ll> &li, int idx) -> void
	{
		int sz = (int)li.size();
		
		// 最後まで見たので、xor計算
		if(idx == N)
		{
			ll xr = 0;
			for(auto &e : li) xr ^= e;
			ans.push_back(xr);
			return;
		}

		// 既存の袋に入れる
		for(int i = 0; i < sz; i++)
		{
			li[i] += a[idx];
			self(self, li, idx+1);
			li[i] -= a[idx];
		}

		// 新しい袋を作る
		li.push_back(a[idx]);
		self(self, li, idx+1);
		li.pop_back();
	};

	vector<ll> li;
	dfs(dfs, li, 0);

	// 元データをソートし、重複を削除する
	sort(ans.begin(), ans.end());
	ans.erase(unique(ans.begin(), ans.end()), ans.end());  // unique()で隣り合う重複を削除し、erase()で末端までのゴミを削除する
	cout << ans.size() << endl;

	return 0;
}
