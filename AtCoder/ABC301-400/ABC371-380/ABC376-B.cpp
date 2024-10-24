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

// ABC376 https://atcoder.jp/contests/abc376

// 円環ライブラリを作成したのでverify用

/*
 * 円環上の距離や移動、公差判定のライブラリ
 * [使い方]
 * コンストラクタ、またはset_n()にて1周の長さを指定する。
 *   move_on_ring ring(N);
 *   or ring.set_n(N);
 * 
 * 座標は 0,1,2,...,n-1 の並びで、n-1の次は0とする。
 * (動作には影響しないが、座標は時計回りで1ずつ増え、反時計回りで1ずつ減るイメージ)
 * 参考：https://atcoder.jp/contests/abc376/tasks/abc376_b
 * 
 * 座標は[0,n)の範囲で指定すること。
 * ※範囲外になりそうな場合、呼び出し元で nの剰余を求めて負ならnを加算 でいける、はず。
 * 
 * [参考]
 * https://x.com/eco4kb/status/1847677647482245472
 * https://qiita.com/NokonoKotlin/items/f2cf76a7abf19b6b67eb
 */
template <typename T>
class move_on_ring {
	T n;  // 1周の長さ
public:
	const int DIR_PLUS_CLOCK = 1;  // 方向指定 座標プラス方向 (時計回り)
	const int DIR_MINUS_REVCLOCK = -1;  // 方向指定 座標マイナス方向 (反時計回り)
	move_on_ring(T n_) : n(n_) {}
	move_on_ring(void) : n(0) {}
	void set_n(T n_) { n = n_; }
	// s,t間の距離を返す (時計回りと反時計回りとで短い方)
	T dist(T s, T t) {
		if(s == t) return (T)0;
		if(s > t) swap(s, t);  // s<=tにする 対称性よりswapして良い
		return min(t-s, n-(t-s));
	}
	// s,t間の距離を返す (s->tへdir方向へ移動したときの距離)
	T dist_dir(T s, T t, int dir) {
		if(s == t) return (T)0;  // n-(t-s)==n の対策
		if(s > t) {  // s<=tにする その場合dirも反転
			swap(s, t);
			dir = ((dir == DIR_PLUS_CLOCK) ? (DIR_MINUS_REVCLOCK) : (DIR_PLUS_CLOCK));
		}
		if(dir == DIR_PLUS_CLOCK) return t-s;
		else return n-(t-s);
	}
	// sからdir方向へiだけ移動した後の座標を返す
	T move(T s, T i, int dir) {
		i %= n;
		if(dir == DIR_PLUS_CLOCK) s = (s+i)%n;
		else s = (s-i+n)%n;
		return s;
	}
	// s->tへdir方向で移動するとき、途中でxを通るならtrue (x=s,tのときもtrue, つまり両端含む)
	bool cross(T s, T t, T x, int dir) {
		return dist_dir(s, x, dir) <= dist_dir(s, t, dir);
	}
};

int main(void)
{
	// 0-indexed
	int i;
	int N, Q; cin >> N >> Q;
	move_on_ring ri(N);
	int l = 0, r = 1;
	int ans = 0;
	for(i = 0; i < Q; i++)
	{
		char h; cin >> h;
		int t; cin >> t; t--;
		if(h == 'L')  // l to t
		{
			// プラス方向で動かしたときRとぶつからないなら、プラス方向で移動
			// ぶつかるならマイナス方向で移動
			if(!ri.cross(l, t, r, ri.DIR_PLUS_CLOCK)) ans += ri.dist_dir(l, t, ri.DIR_PLUS_CLOCK);
			else ans += ri.dist_dir(l, t, ri.DIR_MINUS_REVCLOCK);
			l = t;
		}
		else  // r to t
		{
			if(!ri.cross(r, t, l, ri.DIR_PLUS_CLOCK)) ans += ri.dist_dir(r, t, ri.DIR_PLUS_CLOCK);
			else ans += ri.dist_dir(r, t, ri.DIR_MINUS_REVCLOCK);
			r = t;
		}
	}
	cout << ans << endl;

	return 0;
}
