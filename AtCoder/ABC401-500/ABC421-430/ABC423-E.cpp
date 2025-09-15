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

// ABC423 https://atcoder.jp/contests/abc423

/*
 * コンテスト時に解けず、解説を見た。
 * 主客転倒で解く。各種解説の通り。
 *   https://atcoder.jp/contests/abc423/editorial/13865
 *   https://x.com/kyopro_friends/status/1967223544238244172
 * 
 * 各クエリにて[L,R]内のAiが足される回数(どれだけ寄与するか)を考える。
 * Aiが区間に含まれるのは
 *   区間の左端がL～iまで：(i-L+1)個
 *   区間の右端がi～Rまで：(R-i+1)個
 * より、Aiが含まれるような区間は (i-L+1)*(R-i+1) 通りとなる。
 * よってAiは答に (i-L+1)*(R-i+1)*Ai だけ寄与するので、
 * Σ{i:L～R}(i-L+1)*(R-i+1)*Ai が答となる。
 * 
 * これを整理すると
 * -Σi^2*Ai + (R+L)*Σi*Ai + (R-L-LR+1)*ΣAi
 * となり、
 *   i^2*Ai
 *   i*Ai
 *   Ai
 * それぞれの累積和を事前計算しておくことで、クエリごとO(1)で解ける。
 * 
 * iの値そのものが答に影響するので、下手に0-indexedに変換するとバグりやすいので注意。
 * 実装では1-indexedのまま処理した。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・「和の形の数え上げ」や二重シグマ(Σ)などは、主客転倒を考えてみる。
 *   というのをapproach.txtに残していたが活用できなかった、反省。
**/

int main(void)
{
	// 1-indexed
	ll i;
	ll N, Q; cin >> N >> Q;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	vector<ll> as(a.size()+1); for(int kk = 0; kk < (int)a.size(); kk++) {as[kk+1] = as[kk] + a[kk];}
	vector<ll> asi(a.size()+1); for(int kk = 0; kk < (int)a.size(); kk++) {asi[kk+1] = asi[kk] + a[kk]*kk;}
	vector<ll> asii(a.size()+1); for(int kk = 0; kk < (int)a.size(); kk++) {asii[kk+1] = asii[kk] + a[kk]*kk*kk;}

	while(Q > 0)
	{
		Q--;
		ll l, r; cin >> l >> r;
		ll ans = -(asii[r+1] - asii[l]);
		ans += (asi[r+1] - asi[l]) * (r+l);
		ans += (as[r+1] - as[l]) * (r-l-l*r+1);
		cout << ans << endl;
	}

	return 0;
}
