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

// ABC374 https://atcoder.jp/contests/abc374

/*
 * 自力で解けず、解説を見た。二分探索で解く。
 * 
 * まず、「製造能力Wを達成できるか」で二分探索する。(最小値の最大化でしばしば有効である考え方)
 * Wを大きくしていくと達成できる/できないが切り替わるので、そこが答となる。
 * 
 * この判定問題は、
 *   各工程について製造能力Wを達成するための最小コストをXiとしたとき、ΣXi<=Xであること
 * であることから、Xiを求めることを考える。ここからコンテスト時に解けなかった。
 * 
 * ※Wの理論上最大値は、A=100,P=1,X=10^7のときでWmax=10^9.
 *   購入台数として 0～Wmax/A なり 0～Wmax/B の範囲で全探索しようとすると到底間に合わない。
 * 
 * ここで、SをB台、TをA台購入することを考えると、処理能力はどちらもA*Bとなる。
 * つまりこの2つは可換である。
 * さらにSとTのコスパ(※)の良し悪しを考えてみると、  (※)A/PとB/Qの大小関係
 * ・Sの方がコスパが悪い：SをB台は、TをA台に置き換えた方が得
 *   ⇒Sの台数は [0,B) に制限される
 * ・Tの方がコスパが悪い：TをA台は、SをB台に置き換えた方が得
 *   ⇒Tの台数は [0,A) に制限される
 * となり、A,B<=100のため判定問題はO(N*max(A,B))となり、十分に間に合う。
 * 
 * ※可換となる台数は、lcm=lcm(A,B)として Sは(lcm/A)台、Tは(lcm/B)台となる。
 *   これを使うとさらに計算量が改善できるが、そこまでやらなくても十分に高速。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・二分探索はすぐ思いついたが、判定問題が解けなかった。
 *   コスパの異なる2要素を組み合わせるとき、可換に着目して探索範囲を制限できるという考え方を理解する。
 *   (コスパの良し悪しがあるならば、コスパ悪い方は使う数の上限が限られる、という考え方)
 * ※コンテスト時、判定問題は三分探索で実装してWAだった。(嘘解法)
 *   しかし三分探索で求まった点から周辺探索をするとACできることがある。
 *   実際、±2500で探索したらACした。
 *   しかし嘘解法に近いので、「最後の手段」とか「三分探索が想定解だが、求めた値が不安」なときに使えるテクニックとして覚えておく。
 * 
 * [参考ツイート]
 * https://x.com/kyopro_friends/status/1842563281694998929
 * https://x.com/Fine_sugar_hill/status/1842569132358951156
 * 以下は三分探索＋周辺探索
 * https://x.com/MohutonLab_comp/status/1842560820531827143
 * https://x.com/MohutonLab_comp/status/1842561528446513323
 */

// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

int main(void)
{
	int i, j;
	ll N, X; cin >> N >> X;
	vector<ll> a(N), p(N), b(N), q(N); for(i = 0; i < N; i++) {cin >> a[i] >> p[i] >> b[i] >> q[i];}

	// 製造能力wを達成できるならtrue
	auto check = [&](ll w) -> bool
	{
		ll cost = 0;
		for(i = 0; i < N; i++)
		{
			ll mi = INF64;
			// ※以下、コスパが悪い方について全探索したが、
			//   S,Tそれぞれについて上限100(A,Bの上限)で全探索しても十分に間に合う
			//   #else側を参照。
#if 1
			if(a[i]*q[i] >= b[i]*p[i])  // Sの方がコスパ良い, Tの方がコスパ悪い
			{
				for(j = 0; j <= 100; j++)  // Tを0～100個の範囲で全探索 コスト最小を求める
				{
					ll tt = j;  // Tの購入数
					ll ss = ceil_div(w-tt*b[i], a[i]);  // Sの購入数
					chmax(ss, 0LL);
					chmin(mi, ss*p[i]+tt*q[i]);
				}
			}
			else  // Sの方がコスパ悪い
			{
				for(j = 0; j <= 100; j++)
				{
					ll ss = j;  // Sの購入数
					ll tt = ceil_div(w-ss*a[i], b[i]);
					chmax(tt, 0LL);
					chmin(mi, ss*p[i]+tt*q[i]);
				}
			}
#else
			// swapを用いて似た2回の計算をまとめるテクニック
			for(int _ = 0; _ < 2; _++)
			{
				for(j = 0; j <= 100; j++)
				{
					ll ss = j;
					ll tt = ceil_div(w-ss*a[i], b[i]);
					chmax(tt, 0LL);
					chmin(mi, ss*p[i]+tt*q[i]);
				}
				swap(a[i], b[i]);
				swap(p[i], q[i]);
			}
#endif
			cost += mi;
			if(cost > X) return false;
		}
		return cost <= X;
	};

	ll l = -1, r = INF32;  // Wの最大は10^9
	while(r - l > 1)
	{
		ll w = (l+r)/2;  // 製造能力wを達成できるか
		if(check(w)) l = w;
		else r = w;
	}
	cout << l << endl;

	return 0;
}
