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

// 競プロ典型90問:80 https://atcoder.jp/contests/typical90/tasks/typical90_cb

/*
 * 自力で解き切れず、解説を見た。
 * 包除原理で解く。
 * 
 * 基本は公式解説の通り。
 *   https://twitter.com/e869120/status/1410368312760934404
 * 
 * (1)A1,A2,...Akについて全て満たすxの数：
 *   直接求めるのは難しい。
 * (2)A1,A2,...Akについてどれも満たさないxの数：
 *   簡単に求められる。後述。
 * (3)A1,A2,...Akについて1つ以上満たすxの数：
 *   全体(=2^D)から(2)を引くと求められる。
 * 
 * (2)はA1|A2|...|Akについて1になっているbitは、xにて0にする必要がある。 -> 1通り
 * 逆に0になっているbitはxにて0,1どちらでもよい。
 * よって0になっているbitの個数をzとすると、2^z通り。
 * 
 * 包除原理より、N=2について考えると、求める集合は
 *   A1+A2-(A1∪A2)
 * となる。(3)を使って求められる。
 * ★Aの集合の数が奇数のとき加算し、偶数のとき減算させると上手くいく。
 * 
 * Aの集合を全探索するのは、bit全探索を用いる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・N=20はbitDPや包除原理を考えてみる。
 * ・「全てを満たす場合の数」は難しくとも、「どれも満たさない場合の数」は簡単だったりする。
 *   そこから包除原理を用いて計算していく。
 * ・4つ以上の包除原理は、集合の数の偶奇によって加算/減算が切り替わる。
 *   奇数なら答に加算、偶数なら答に減算など。
 *   ※問題によって偶奇が逆だったりする。
 */

int main(void)
{
	ll i, j;
	ll N, D; cin >> N >> D;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> pow2(61);  // pow2[i] = 2^i
	pow2[0] = 1;
	for(i = 1; i < 61; i++)
	{
		pow2[i] = pow2[i-1] * 2;
	}
	
	ll ans = 0;
	for(i = 0; i < (1LL<<N); i++)  // bit全探索 iが1になっているbitについてAを考える
	{
		ll mix = 0;
		for(j = 0; j < N; j++)
		{
			if((i>>j)&1)
			{
				mix |= a[j];
			}
		}
		ll z = D - __builtin_popcountll(mix);;  // bitが0の個数
		ll tmp = pow2[D] - pow2[z];  // Aの集合について、1つ以上で条件を満たすもの

		// Aの集合の数が偶数なら減らし、奇数なら足す
		ans += (__builtin_popcountll(i)%2 == 0) ? (-tmp) : (tmp);
	}
	cout << ans << endl;

	return 0;
}
