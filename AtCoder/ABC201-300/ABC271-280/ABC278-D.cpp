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

// ABC278 https://atcoder.jp/contests/abc278

/*
 * 要素を{最終更新時刻, 値}のペアで管理する。
 * 以下の解説と同じ内容。
 *   https://atcoder.jp/contests/abc278/editorial/5235
 *   https://twitter.com/kyopro_friends/status/1593963585684017153
 * 
 * クエリ1の「全要素にxを代入」した時刻も保持しておくと、
 * クエリ2,3について
 *   A[i]の値を使えばよいのか
 *   クエリ1で設定された値を使えばよいのか
 * を時刻で判断することができる。 (時刻が後の方を使う)
 * 
 * [ACまでの思考の流れ]
 * ・「全ての要素に代入する」は愚直にやると間に合わないので、何か別管理することを考える。
 * ・上記の値を加算や出力クエリに使うか否かは、クエリを時刻で管理すれば実現できそう。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<pair<int,ll>> a(N);  // {time, num}
	pair<int,ll> allset(-1, 0);
	for(i = 0; i < N; i++)
	{
		a[i].first = 0;  // 時刻0
		cin >> a[i].second;
	}
	int Q; cin >> Q;
	for(int t = 0; t < Q; t++)
	{
		int kind; cin >> kind;
		if(kind == 1)
		{
			ll x; cin >> x;
			allset = {t, x};
		}
		else if(kind == 2)
		{
			ll idx, x; cin >> idx >> x;
			idx--;
			if(a[idx].first > allset.first)
			{
				a[idx].second += x;
			}
			else
			{
				a[idx].second = allset.second + x;
			}
			a[idx].first = t;
		}
		else
		{
			ll idx; cin >> idx;
			idx--;
			ll ans = allset.second;
			if(a[idx].first > allset.first)
			{
				ans = a[idx].second;
			}
			cout << ans << endl;
		}
	}

	return 0;
}
