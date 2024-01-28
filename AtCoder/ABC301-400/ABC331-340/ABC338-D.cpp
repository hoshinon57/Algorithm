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

// ABC338 https://atcoder.jp/contests/abc338

/*
 * コンテスト中に解けず、解説を見た。
 * いもす法を使う。
 * 
 * 以下、0-indexedで記載する。
 * また島0,1,2,...N-1 の移動を時計回り、逆を反時計回りと表現する。
 * 島a->bへの移動量について、a<bとすると
 *   時計回り：b-a = D1
 *   反時計回り：a+N-b = D2
 * となる。
 * 
 * 島a->bへの移動について、各橋が封鎖されたときに移動量がどれだけ増えるかを考える。
 * 封鎖前、時計回りの方が近い場合、
 *   橋番号が[a,b)のものについて、 D2-D1 だけ増える。
 * 反時計回りの方が近い場合、
 *   橋番号が[b,a)のものについて、 D1-D2 だけ増える。  ただし[b,a)はb->0->aのルート。
 * N本ある橋ごとにこの増加量を計算していって、最も小さいものを封鎖するのが正解。
 * 全ての橋が使えるときの移動量の総和に、上記の最小値を加算したものが答となる。
 * 
 * ただし愚直に計算するとO(NM)であり間に合わない。
 * これを上記計算をいもす法で実装することで、O(N+M)で処理できて間に合う。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・コンテスト時は正しくない方針で進めてしまった。
 * ・島x[i]->x[i+1]への移動ごとに、「どの橋を封鎖すると移動量が変化するか」が決まる。(N*Mの組み合わせ)
 *   橋ごとに変化量を計算していけば、全移動について加算していくことで封鎖すべき橋が分かる。
 *   ただし組み合わせがN*Mになるため工夫が必要。
 *   「移動量が増える橋」は連続していることから、いもす法が使えそう。
 */

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	vector<ll> x(M); for(i = 0; i < M; i++) { cin >> x[i]; x[i]--; }
	vector<ll> imos(N+5);
	ll ans = 0;
	for(i = 0; i < M-1; i++)
	{
		ll a = x[i];
		ll b = x[i+1];
		if(a>b) swap(a,b);  // a<bにする(一般性を失わない)
		ll d1 = b-a;  // 時計回りでの移動量
		ll d2 = a+N-b;  // 反時計回り
		if(d1 < d2)
		{
			// 時計回りの方が近いので、[a,b)の橋を封鎖すると移動量が増える
			ll dd = d2-d1;
			imos[a] += dd;
			imos[b] -= dd;
		}
		else
		{
			// 反時計回りの方が近いので、[b,a)の橋を封鎖すると移動量が増える
			ll dd = d1-d2;
			imos[b] += dd;
			imos[0] += dd;
			imos[a] -= dd;
		}
		ans += min(d1, d2);  // 全ての橋が残っているときの距離を計算しておく
	}

	ll b = imos[0];
	for(i = 0; i < N-1; i++)
	{
		imos[i+1] += imos[i];
		chmin(b, imos[i+1]);
	}
	ans += b;
	cout << ans << endl;

	return 0;
}
