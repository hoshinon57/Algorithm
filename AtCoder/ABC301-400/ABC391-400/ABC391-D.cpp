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

// ABC391 https://atcoder.jp/contests/abc391

/*
 * 結構な実装問題な気がする。基本は各種解説の通り。
 *   https://x.com/kyopro_friends/status/1886359680106279076
 *   https://atcoder.jp/contests/abc391/editorial/12102
 * 
 * 愚直にブロックを動かすのはもちろん間に合わない。
 * 行数やクエリの時刻も10^9と大きな値なので、計算量にこれが入るのは厳しそう。
 * ブロック数Nや列数Wを基点に考える。
 * 
 * 列iの下からj個目のブロック、について考える。
 * 例えばix回目にブロックが消えることを考えると、それは各列の下からix個目のブロックであり、
 * max(y1,y2,y3,...,yW)+1 が消える時刻となる。
 * 1つでも下からix番目のブロックが無い列があれば、それはずっと消えない。
 * 
 * よってブロックが消える順番が若い順に、上記のように見ていく。
 * 初めて消滅しない回が見つかったら、以降の回もずっと消えないので、そこで探索を打ち切れる。
 * 
 * 以上により、ブロックiが消える時刻ans[i]が分かる。
 * クエリTに対し、ans[i]<=Tであれば、対象のブロックは消えている、つまり"No"回答となる。
 */

int main(void)
{
	// 0-indexed
	ll i, j;
	ll N, W; cin >> N >> W;
	vector<vector<pair<ll,ll>>> posy(W);  // posy[i][j]:i列目の、下からj番目のブロックの{y座標,idx}
	for(i = 0; i < N; i++)
	{
		ll x, y; cin >> x >> y;
		x--; y--;
		posy[x].push_back({y, i});
	}
	for(auto &e : posy)
	{
		sort(e.begin(), e.end());
	}

	vector<ll> ans(N, INF64);  // ans[i]:ブロックiが消える時刻 消えなければINF
	auto f = [&](void) -> void
	{
		for(i = 0; i < N; i++)  // i回目に消える列について考える
		{
			ll mx = -1;
			for(j = 0; j < W; j++)
			{
				if(i >= (int)posy[j].size()) return;  // 下からi番目のブロックが存在しない
				chmax(mx, posy[j][i].first);
			}
			for(j = 0; j < W; j++)
			{
				ans[posy[j][i].second] = mx+1;
			}
		}
	};
	f();

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		ll t, a; cin >> t >> a;
		a--;
		if(ans[a] <= t) cout << "No" << endl;
		else cout << "Yes" << endl;
	}

	return 0;
}
