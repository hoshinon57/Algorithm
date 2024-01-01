#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC308 https://atcoder.jp/contests/abc308

/*
 * もう1つの貪欲法での解き方。
 * 値段の安い順に商品を見ていく。
 *   https://atcoder.jp/contests/abc308/editorial/6706
 * 
 * 各商品について、その時点で使えるクーポン(の割引額)をキューに入れていく。
 * そしてクーポンの割引額が大きい方から使っていく。
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	vector<ll> p(N); for(i = 0; i < N; i++) {cin >> p[i];}
	sort(p.begin(), p.end());  // 商品Pは小さい順
	vector<ll> l_(M); for(i = 0; i < M; i++) {cin >> l_[i];}
	vector<ll> d_(M); for(i = 0; i < M; i++) {cin >> d_[i];}
	vector<pair<ll,ll>> cp;
	for(i = 0; i < M; i++) cp.push_back({l_[i], d_[i]});
	sort(cp.begin(), cp.end());  // {L,D} 使える定価下限Lが小さい順

	priority_queue<ll> que;  // 割引額 大きい順に
	ll ans = 0;
	int idx = 0;
	for(auto &e : p)
	{
		// 定価eの商品を見る時点で、使えるクーポン(の割引額)をキューに入れる
		for(i = idx; i < M; i++)
		{
			if(cp[i].first <= e) que.push(cp[i].second);
			else break;
		}
		idx = i;
		if(!que.empty())
		{
			ll q = que.top();  // 割引額
			que.pop();
			ans += e-q;
		}
		else
		{
			ans += e;
		}
	}
	cout << ans << endl;
	return 0;
}
