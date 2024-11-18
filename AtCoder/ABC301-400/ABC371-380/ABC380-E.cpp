#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC380 https://atcoder.jp/contests/abc380

/*
 * 区間をsetで管理するテク、で解く。公式解説と同じ方針だった。
 *   https://atcoder.jp/contests/abc380/editorial/11356
 * 
 * マスを左右で同じ色のマスでまとめて
 *   {同じ色のマスのうち、最左の座標, 色}
 * の形でsetで管理する。
 * またnum[c]を色cの個数として管理する。
 * 
 * クエリ2はnum[]を返すだけで良い。
 * クエリ1は位置xに対し、setにてそれが属する要素を二分探索で求める。
 * ⇒upper_bound()して-1する、で上手くいく
 * その要素と前後の要素とをマージしていく。これは実装を参照。
 * 
 * マスの先頭末尾に番兵を用意しておくと実装が楽。
 */

int main(void)
{
	// 1-indexed
	ll i;
	ll N, Q; cin >> N >> Q;
	vector<ll> num(N+1);
	set<pair<ll,ll>> d;  // {同じ色のマスのうち、最左の座標, 色}
	for(i = 1; i <= N; i++)
	{
		num[i] = 1;
		d.insert({i, i});  // マスiが色i
	}
	d.insert({0, -INF64});
	d.insert({N+1, -INF64});  // 番兵

	while(Q > 0)
	{
		Q--;
		int k; cin >> k;
		if(k == 1)
		{
			ll x, c; cin >> x >> c;  // 色cにする
			auto itr = d.upper_bound({x, INF64});  // 位置xに相当する区間を取得
			itr--;
			auto pr = prev(itr);
			auto nx = next(itr);
			// num[]更新
			ll len = nx->first - itr->first;
			num[itr->second] -= len;
			num[c] += len;

			// 区間情報更新
			if(nx->second == c)  // 次の区間とマージできる場合は、そのまま次を削除してよい
			{
				d.erase(nx);
			}
			ll pos = itr->first;  // 登録する区間の左端
			if(pr->second == c)  // 手前の区間とマージできる場合は、posを更新してから削除
			{
				pos = pr->first;
				d.erase(pr);
			}
			d.erase(itr);
			d.insert({pos, c});
		}
		else
		{
			ll c; cin >> c;
			cout << num[c] << endl;
		}
	}

	return 0;
}
