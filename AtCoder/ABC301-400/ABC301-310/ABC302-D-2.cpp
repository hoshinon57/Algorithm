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

// ABC302 https://atcoder.jp/contests/abc302

// 尺取り法で解いてみた版
// 参考記事：https://drken1215.hatenablog.com/entry/2023/05/30/024900

int main(void)
{
	int i, j;
	ll N, M, D; cin >> N >> M >> D;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> b(M); for(i = 0; i < M; i++) {cin >> b[i];}
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());

	ll ans = -1;
	for(i = 0, j = 0; i < N; i++)  // a[i]とb[j]
	{
		// b[j]-a[i]<=D を満たす間、jをインクリメント
		while(j < M-1 && b[j+1]-a[i] <= D) j++;  // jはM-1でストップ

		// 今のa[i],b[j]が差D以下とは限らないので、チェックしてからans更新
		if(abs(a[i]-b[j]) <= D) chmax(ans, a[i]+b[j]);
	}
	cout << ans << endl;

	return 0;
}
