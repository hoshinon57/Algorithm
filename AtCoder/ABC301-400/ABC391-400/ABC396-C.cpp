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

// ABC396 https://atcoder.jp/contests/abc396

// 黒数>=白数なので、白で価値が負のものは0に直しておくと、
// 黒と白を同数選ぶ、という簡潔な処理が実現できる。
// また白が黒より個数が少なければ、価値0の白ボールを増やして黒と同じ個数にしておくと処理が楽。

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	vector<ll> b(N); for(i = 0; i < N; i++) {cin >> b[i];}
	vector<ll> w(M); for(i = 0; i < M; i++) {cin >> w[i];}
	sort(b.begin(), b.end(), greater<ll>());
	sort(w.begin(), w.end(), greater<ll>());
	for(auto &e : w) if(e < 0) e = 0;  // 0でクリップ
	w.resize(N, 0);  // M個より多いぶんは0で埋めておく

	ll ans = 0;
	ll sum = 0;
	for(i = 0; i < N; i++)  // 黒からi個選ぶことを考える
	{
		sum += b[i] + w[i];
		chmax(ans, sum);
	}
	cout << ans << endl;

	return 0;
}
