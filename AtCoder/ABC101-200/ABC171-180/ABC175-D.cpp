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

// ABC175 https://atcoder.jp/contests/abc175

/*
 * ※AC後、「P1～Pnはすべて異なる」という制約に気づいた…
 *   となると、各連結成分ごとにサイクルになる形になるので、もう少し簡単な実装にできたかな…
 * 
 * i->Piに辺を張ったグラフを考えると、functional graphになる。
 * 各頂点ごとに「この頂点をスタートとしたときの最大スコア」をO(N)で計算する。
 * 全体でO(N^2)となり間に合う。
 * 
 * スタートとなる頂点stについて、
 *   path:stから辿る頂点の一覧
 *   bef_loop:ループに入る前の頂点数
 *   loop_num:ループ部分の頂点数
 *   sum_c:ループ部分のスコア
 * を求める。
 * 次に各path[*]を終点としたときのスコアを計算する。
 * (ただし1回以上動くという制約より、path[1]から)
 * これは
 *   始点からpath[i]までのスコアを計算
 *   path[i]がサイクル中であれば、残りの移動回数でループできる回数を計算
 * により求めることができる。
 */

int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	vector<int> p(N); for(i = 0; i < N; i++) {cin >> p[i]; p[i]--;}  // i->Pi
	vector<ll> c(N); for(i = 0; i < N; i++) {cin >> c[i];}

	ll ans = -INF64;
	int st;
	for(st = 0; st < N; st++)  // 頂点stをスタート
	{
		vector<bool> used(N, false);  // 探索済みか
		vector<int> path;  // スタートから辿る頂点を順に格納する
		int cur = st;
		while(!used[cur])
		{
			path.push_back(cur);
			used[cur] = true;
			cur = p[cur];
		}

		int bef_loop = find(path.begin(), path.end(), cur) - path.begin();
		int loop_num = (int)path.size() - bef_loop;
		ll sum_c = 0;  // サイクル部分の和
		for(i = 0; i < loop_num; i++)
		{
			sum_c += c[path[i+bef_loop]];
		}
		// サイクル終点を入れておく
		// 次はi=1から計算するため、curを入れないと始点がすでにサイクル中のケースでcurに対する計算がされない
		path.push_back(cur);

		ll sum = 0;
		for(i = 1; i < (int)path.size(); i++)  // path[i]を終点としたときのスコアを計算
		{
			// まず始点からpath[i]までのスコアを計算し、そこからループ回数だけ加算する
			if(i > K) break;
			sum += c[path[i]];  // 始点からpath[i]までのスコア
			// 以降、ループする部分を加算
			ll cnt = 0;  // ループ回数
			if(i < bef_loop) {;}  // ループしない
			else
			{
				cnt = (K-i) / loop_num;
				if(sum_c < 0) cnt = 0;  // ループの和が負なら、ループしない方が有利
			}
			chmax(ans, sum + cnt*sum_c);
		}
	}
	cout << ans << endl;

	return 0;
}
