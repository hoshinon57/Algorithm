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

// ABC371 https://atcoder.jp/contests/abc371

/*
 * コンテスト時に考えた解法。
 * 「チェックする対象をi個目⇒i+1個目に変えるときにほとんど変化しないならば、差分に着目してO(1)で解く」
 * の典型を使う。
 * 
 * 例えばA=(1,2,3,3,1)を考えると、
 * まずi=0のときのf(i,j)は順に(1,2,3,3,3)となる。
 * i=1のときは(-,1,2,2,3)となり、
 * i=0の部分が消えることで、i=0から次にAi=1となるi=4まで、[0,4)の範囲で1ずつ減っていくことになる。
 * 
 * よってpos[x]={*,*,...} を値xの登場位置とすると、二分探索により次に登場するxの位置が分かり、
 * そこからiが1つ増えるときのf(i,j)の減少量が分かる。
 * 
 * pos[]には末尾にNの番兵を入れておくと処理が楽。
 */

int main(void)
{
	ll i, j;
	ll N; cin >> N;
	vector<ll> a(N);
	vector<vector<ll>> pos(N+5);  // pos[x]:値xの登場位置
	vector<ll> cnt(N+5);
	ll now = 0;
	// 入力しつつ、i=0だけ愚直に計算
	for(i = 0, j = 0; i < N; i++)
	{
		cin >> a[i];
		pos[a[i]].push_back(i);
		if(++cnt[a[i]] == 1) j++;
		now += j;
	}
	for(auto &e : pos) e.push_back(N);  // 末尾番兵

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		ans += now;
		// 次iに向けての更新
		// a[i]が消える -> i以降、次にa[i]が登場するまで、f(i,j)は1ずつ減る
		auto idx = upper_bound(pos[a[i]].begin(), pos[a[i]].end(), i) - pos[a[i]].begin();  // [i,pos[a[i]][idx] )
		now -= (pos[a[i]][idx] - i);
	}
	cout << ans << endl;

	return 0;
}
