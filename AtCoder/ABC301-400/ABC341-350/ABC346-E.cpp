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

// ABC346 https://atcoder.jp/contests/abc346

/*
 * 操作を逆順に見ていく、という考え方。
 *   https://twitter.com/kyopro_friends/status/1771534576629776896
 *   https://atcoder.jp/contests/abc346/editorial/9637
 * 
 * 逆から見ていくと、T=1の操作は「行AのマスをXで確定させる」と読み替えられる。
 * 同じ行に対する操作は、「確定済みの行はスルー」で対応できる。
 * 
 * gyou_fix[i]:i行目が確定していたらtrue
 * gyou_zan:まだ確定していない行数
 * とする。列側も同様。
 * T=1について、まずgyou_fix[]をチェックし、retu_zanの分だけ色Xで確定となる。
 * T=2も同様。
 * 
 * 未処理の色は初期値の0であるため、
 * gyou_zan*retu_zanを色0の個数に加算しておく。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・後の操作で上書きされる場合、逆から考えてみる。[典型]
 */

int main(void)
{
	ll i;
	ll H, W, M; cin >> H >> W >> M;
	vector<ll> t(M), a(M), x(M);
	for(i = 0; i < M; i++)
	{
		cin >> t[i] >> a[i] >> x[i];
		a[i]--;
	}

	vector<bool> gyou_fix(H, false), retu_fix(W, false);  // gyou_fix[i]:i行目が既に塗られていたらtrue
	ll gyou_zan = H;  // まだ塗られていない行数
	ll retu_zan = W;
	vector<ll> cnt(200005);  // cnt[i]:色iで塗った数
	for(i = M-1; i >= 0; i--)
	{
		if(t[i] == 1)
		{
			if(gyou_fix[a[i]]) continue;  // 塗り済み
			cnt[x[i]] += retu_zan;
			gyou_fix[a[i]] = true;
			gyou_zan--;
		}
		else
		{
			if(retu_fix[a[i]]) continue;
			cnt[x[i]] += gyou_zan;
			retu_fix[a[i]] = true;
			retu_zan--;
		}
	}

	ll K = 0;
	cnt[0] += gyou_zan * retu_zan;  // 処理されなかった色は、初期値の0
	for(auto &e : cnt)
	{
		if(e != 0) K++;
	}
	cout << K << endl;
	for(i = 0; i < 200002; i++)
	{
		if(cnt[i] == 0) continue;
		cout << i << " " << cnt[i] << endl;
	}

	return 0;
}
