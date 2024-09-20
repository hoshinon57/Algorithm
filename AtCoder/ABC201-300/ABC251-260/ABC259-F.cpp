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

// ABC259 https://atcoder.jp/contests/abc259

/*
 * 自力で解けず、解説を見た。
 * 動的計画法(木DP)で解く。以下の記事を参考にした。
 *   https://atcoder.jp/contests/abc259/editorial/4287
 *   https://drken1215.hatenablog.com/entry/2023/05/04/015400
 *   https://scrapbox.io/procon-kirokuyou/ABC259_F_-_Select_Edges_(500)
 * 
 * 頂点vを根とした部分木で考えたときに、
 * 子uが既にuの子側で辺を使い切っていた場合、v-u間の辺は使えない。
 * 逆に使い切ってない場合は、v-u間の辺は使える。
 * 
 * またd[u]=3として、uの子側の辺の重みが大きい順から{5,4,3}のとき、
 *   d[u]を使い切る場合：総和12
 *   d[u]を余らせる場合：総和9
 * であるので、v-u間の重みをwとすると、
 * 使い切りから余らせへ「鞍替え」してv-uの辺を採用するならば、(w-3)だけ総和が良化することになる。
 * 
 * 以上を元に、頂点0を根として以下のDP値を考える。
 * dp_full[i]:
 *   頂点iを根とした部分木について、(子の方向に対し)辺をd[i]本以下の範囲で選ぶときの、重み総和の最大値
 * dp_less[i]:
 *   fullに対し、d[i]本"未満"の範囲で選ぶときの最大値
 * dp_full[0]が答となる。
 * 
 * 頂点vと、その子u1,u2,... で考えると、
 * まずΣ(u=u1,u2,...)dp_full[u] がvのDP値になる。
 * その上で、v-u間の辺を使うように鞍替えすると、w-(full-less)だけ良化する。
 * これを大きい順から貪欲に採用すればよい。
 * full[v]はd[v]本まで、less[v]はd[v]-1本まで選べる。
 * 
 * 各種解説では、d[i]=0のとき"0本未満"は選べないので、less[i]=-INFとしている。
 * 自分の実装ではd[i]=0のときiと繋がる辺の重みを0にして対応した。
 * ※less[i]=-INFにする場合、
 *   dfs()のfor(auto &cc : c)の手前に d[v]=0ならless[v]=-INF という処理を追加すればACした。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・木DPっぽい。
 * ・辺の採用数に制限があるので、
 *   「自身の子だけで使い切る(自身の親とは繋げない)」ケースと「自身の親とつなげる余地を残す」ケースで分けてみる。
 */

// 頂点を結ぶ辺
struct Edge
{
	int to;  // 隣接頂点番号
	ll weight;  // 辺の重み
	Edge(int to_, ll weight_) : to(to_), weight(weight_) {}
};
using Graph = vector<vector<Edge>>;

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> d(N); for(i = 0; i < N; i++) {cin >> d[i];}
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int u, v, w; cin >> u >> v >> w;
		u--; v--;
		if(d[u] == 0 || d[v] == 0) w = 0;  // 辺が選べない場合は重みを0に
		g[u].push_back({v, w});
		g[v].push_back({u, w});
	}

	vector<ll> dp_full(N), dp_less(N);

	auto dfs = [&](auto &self, int v, int p = -1) -> void
	{
		ll tot = 0;
		vector<ll> c;
		for(auto &e : g[v])
		{
			if(e.to == p) continue;
			self(self, e.to, v);
			tot += dp_full[e.to];
			c.push_back(e.weight - (dp_full[e.to] - dp_less[e.to]));
		}
		dp_full[v] = dp_less[v] = tot;

		// fullからlessへ鞍替えして良化する量が大きい順に採用
		sort(c.begin(), c.end(), greater<ll>());
		for(auto &cc : c)
		{
			if(cc <= 0) break;  // 逆に悪化する
			d[v]--;
			if(d[v] >= 0) dp_full[v] += cc;  // d本まで
			if(d[v] >  0) dp_less[v] += cc;  // (d-1)本まで
		}
	};
	dfs(dfs, 0);
	cout << dp_full[0] << endl;

	return 0;
}
