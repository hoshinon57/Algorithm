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

/*
 * コンテスト時に解けず、円環ライブラリを作成したり解説を見たりした。
 * 動的計画法で解く。
 * 
 * dp[i][j]を以下のように定義する。
 *   クエリiまで見て、 (i=1～Q)
 *   クエリで指定された手とは反対の手の位置がjのときの、 (j=0～N-1)
 *   操作回数の最小値
 * クエリで指定された手の位置は固定なので、DP情報から落として次元を減らせる、という典型。
 * 
 * DP遷移は配るDPとした。
 * dp[i][j]として「ありえない」状態が多いこと、
 * 次の状態へありうる場合の数は少ないこと、
 * などから、配るDPが良いみたい。もらうDPではDP遷移を書けなかった。
 * 
 * クエリにてLを動かすとき、時計回り(+方向)に動かすケースを考えると
 *   Lの移動途中にRがいる：RはLの次マスに移動する
 *   Lの移動途中にRがいる：Rはそのまま
 * となり、それぞれL,Rの移動後の位置が決まる。
 * 反時計回りも同様。
 * 実装は頭がこんがらがるので、作った円環ライブラリを最大限に活用した。
 * 
 * クエリでRを動かす場合、L,Rをスワップさせることで
 * 前述のLを動かすケースにまとめることができ、場合分けを減らせる。
 * 
 * dp[Q][*]の最小値が答。
 */

/*
 * 円環上の距離や移動、公差判定のライブラリ
 * [verify]ABC376-B
 * [使い方]
 * コンストラクタ、またはset_n()にて1周の長さを指定する。
 *   move_on_ring ring(N);
 *   or ring.set_n(N);
 * 
 * 座標は 0,1,2,...,n-1 の並びで、n-1の次は0とする。
 * (動作には影響しないが、座標は時計回りで1ずつ増え、反時計回りで1ずつ減るイメージ)
 * 参考：https://atcoder.jp/contests/abc376/tasks/abc376_b
 * 
 * 方向指定の関数は DIR_PLUS_CLOCK, DIR_MINUS_REVCLOCKを指定する。
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
	int i, j;
	int N, Q; cin >> N >> Q;
	move_on_ring ri(N);
	vector<int> h(Q+1), t(Q+1);  // h[i]:クエリiがLなら0, Rなら1
	for(i = 1; i <= Q; i++)
	{
		char k;
		cin >> k >> t[i];
		h[i] = (k == 'L') ? 0 : 1;
		t[i]--;
	}
	vector<vector<int>> dp(Q+1, vector<int>(N, INF32));  // dp[Q+1][N]
	h[0] = 0; t[0] = 0; dp[0][1] = 0;  // 初期状態は操作'L', L=0,R=1とする

	// 配るDP
	for(i = 0; i < Q; i++)  // クエリiまで処理した状態
	{
		for(j = 0; j < N; j++)  // dp[i][j]からの遷移
		{
			if(dp[i][j] == INF32) continue;
			int l, r;
			if(h[i] == 0)  // 'L'
			{
				l = t[i];
				r = j;
			}
			else
			{
				l = j;
				r = t[i];
			}
			
			int to = t[i+1];

			if(h[i+1] == 1) swap(l, r);  // Rを動かす場合、L,RスワップしてLを動かすケースに変換
			// 以下、Lを動かす前提
			// l->to, r->nr へそれぞれ移動
			int nr;
			int cnt = 0;
			// +方向
			if(ri.cross(l, to, r, ri.DIR_PLUS_CLOCK))  // Lの移動途中にRがいるか
			{
				nr = ri.move(to, 1, ri.DIR_PLUS_CLOCK);  // Rを目的地の1つ先にずらす
			}
			else
			{
				nr = r;  // Rそのまま
			}
			cnt = ri.dist_dir(l, to, ri.DIR_PLUS_CLOCK) + ri.dist_dir(r, nr, ri.DIR_PLUS_CLOCK);
			chmin(dp[i+1][nr], dp[i][j] + cnt);

			// -方向
			if(ri.cross(l, to, r, ri.DIR_MINUS_REVCLOCK))
			{
				nr = ri.move(to, 1, ri.DIR_MINUS_REVCLOCK);
			}
			else
			{
				nr = r;  // Rそのまま
			}
			cnt = ri.dist_dir(l, to, ri.DIR_MINUS_REVCLOCK) + ri.dist_dir(r, nr, ri.DIR_MINUS_REVCLOCK);
			chmin(dp[i+1][nr], dp[i][j] + cnt);
		}
	}

	int ans = INF32;
	for(i = 0; i < N; i++) chmin(ans, dp[Q][i]);
	cout << ans << endl;

	return 0;
}
