#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <map>
#include <array>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC298 https://atcoder.jp/contests/abc298

/*
 * 優先度付きキューを用いて解く。
 * 公式解説と同じ考え方だった。kyopro_friends氏の解説が分かりやすい。
 *   https://x.com/kyopro_friends/status/1647236251647635458
 *   https://atcoder.jp/contests/abc298/editorial/6211
 *   https://atcoder.jp/contests/abc298/editorial/6189
 * 
 * 選ぶマス(R,C)について、Y,Xどちらの方向を見ても数値が書かれているのが最適。
 * (数値が書かれていない方向がある場合、より良い選び方が必ず存在する)
 * ただしこれを単に全探索すると、O(N^2)となり到底間に合わない。
 * 
 * (R,C)に対するSは以下となる。
 *   R行目に書かれた値の総和 + C列目に書かれた値の総和 - (R,C)の値
 * Rについて、つまりX=x1,x2,... について全探索することを考えると、
 * 例えばX=x1について、列に書かれた値の総和が大きい順に探索していくのが良い。
 * 
 * ここで (R,C)の値=0 となるケースが登場したとすると、以降、これより大きいSは存在しないため探索を打ち切れる。
 * これはN+1回目までには必ず登場するため、O(NlogN)にて処理できる。
 * 
 * 実装が結構大変だった。
 * R方向/C方向の総和はまずmapで求め、vectorに移行してからソートした。
 * またX方向に番兵を追加することで、必ず (R,c)の値=0 のケースが登場するようにした。(実装の簡易化)
 * 
 * [ACまでの思考の流れ]
 * ・値が書かれている点についてX/Y方向に線を伸ばし、その交点が答の候補になりそう。
 * ・X方向の和＋Y方向の和 とすると、ダブリがあって減算しなきゃダメなので、大きい順に並ばないのでどうするか。
 * ・ダブリを引くというのを考えると、ダブってない、つまり減算0のケースが見つかったら
 *   以降は答候補が更新されることは無く、探索を打ち切れるのでは。
 *   制約より、N+1回目には必ず終わりそう。
 * ・2要素の組み合わせについて最大値を求めたい場合、
 *   片方について全探索し、もう片方は大きい方から選んでいくのを優先度付きキューを使って求めるという考え方。
 *   制約によっては「N回目までには必ず最大値が登場する」ことがある。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	using pll = pair<ll,ll>;
	map<pll, ll> p;  // p[{r,c}] = 整数x
	map<ll, ll> xsum_mp, ysum_mp;  // xsum_mp[pos] = sum  横軸xで見たときの総和
	for(i = 0; i < N; i++)
	{
		ll r, c, x; cin >> r >> c >> x;
		p[{r,c}] = x;
		ysum_mp[r] += x;
		xsum_mp[c] += x;
	}
	vector<pll> xsum, ysum;  // xsum[i] = {sum, xpos}  横軸xposで見たときの総和
	for(auto &[key, val] : ysum_mp) ysum.push_back({val, key});
	for(auto &[key, val] : xsum_mp) xsum.push_back({val, key});
	xsum.push_back({0, INF64});  // 番兵
	sort(ysum.begin(), ysum.end(), greater<pll>());
	sort(xsum.begin(), xsum.end(), greater<pll>());
	ll xsz = xsum.size(), ysz = ysum.size();

	priority_queue<array<ll,3>> que;  // {sum, ysum_idx, xsum_idx}  sumの大きい順
	for(i = 0; i < xsz; i++)  // xsum[i]
	{
		array<ll,3> tmp = {xsum[i].first+ysum[0].first, 0, i};  // yのidxは0, xについては全探索
		que.push(tmp);
	}
	ll ans = 0;
	while(!que.empty())
	{
		auto [sum, ysum_idx, xsum_idx] = que.top();
		que.pop();
		ll ypos = ysum[ysum_idx].second;
		ll xpos = xsum[xsum_idx].second;
		ll pp = p[{ypos, xpos}];
		chmax(ans, sum-pp);
		if(pp == 0) break;  // 0が書かれているマス目 以降はansが更新されることは無い

		// xsum_idxは固定で、次のyを探す
		if(++ysum_idx == ysz) continue;  // もう全て探した
		array<ll,3> t = {xsum[xsum_idx].first + ysum[ysum_idx].first, ysum_idx, xsum_idx};
		que.push(t);
	}
	cout << ans << endl;

	return 0;
}
