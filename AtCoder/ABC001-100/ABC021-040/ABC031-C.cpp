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

// 意外と実装難だった
// 
// 高橋くんが丸をつける位置で全探索。
// 高橋くんが丸をつけると、青木くんが丸をつける位置は一意に決まる。
// そのときの高橋くんのスコアを記録しておき、高橋くんが丸をつける全ケースでの最大値が答。
// 
// 数列に負の値が出てくるので、スコア初期値に注意。
// AC後にWEB検索したら、以下がほとんど同じだった。
//   https://mmxsrup.hatenablog.com/entry/2016/09/22/174141

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	ll ans = -INF64;  // 全体での高橋スコア
	for(ll pt = 0; pt < N; pt++)  // 高橋君の丸
	{
		ll st = -INF64, sa = -INF64;  // (今回高橋くんが丸をつけたケースでの)高橋スコア, 青木スコア
		for(ll pa = 0; pa < N; pa++)  // 青木くんの丸
		{
			if(pt == pa) continue;
			// スコア計算
			ll st2 = 0, sa2 = 0;
			int cnt = 1;
			for(i = min(pt, pa); i <= max(pt, pa); i++)
			{
				if(cnt%2 == 1) st2 += a[i];
				else sa2 += a[i];
				cnt = 1-cnt;
			}

			if(chmax(sa, sa2))  // 最大値更新したらtrueになるので、「最も左側にある要素に丸を付ける」の条件を満たす
			{
				st = st2;
			}
		}
		chmax(ans, st);
	}
	cout << ans << endl;

	return 0;
}
