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

// ABC378 https://atcoder.jp/contests/abc378

/*
 * 公式解説のLCAの手法で解いてみた版。
 *   https://atcoder.jp/contests/abc378/editorial/11293
 *   https://atcoder.jp/contests/abc378/submissions/59328581
 *   https://x.com/kyopro_friends/status/1852710254167081353
 * 
 * 頂点0を根として木DPを考える。
 * 追加する辺として選ぶ2頂点のLCAを考えたときに、木DPにて各頂点がそのLCAになるようなケースを数え上げていく。
 * 
 * dfs(v)を、vの子の方から次数が 2,3,3,...,3 と続いている個数を返すよう定義する。
 * (直下の子の次数が2というケースも許容する)
 * ・deg[v]=3のとき
 *   子頂点eに対してdfs(e)で再帰し、その戻り値を vector<ll>cnt に積んでいく。
 *   cntの要素ごとに子が分かれているので、ある要素と別の要素の組み合わせ数を考えればよく、
 *   これは cnt[i]*(total_cnt-cnt[i]) の総和を、最後に2で割ったもので求められる。
 *   自身の戻り値はcntの総和。
 *   
 * ・deg[v]=2のとき
 *   この場合、子孫で次数が2(かつその間は次数が全て3)の頂点と結べる。
 *   子頂点eに対してdfs(e)で再帰し、その戻り値の総和が答となる。
 *   …が、子の次数が2はカウントしてはダメ。自身と直下の子を結ぶことになり、多重辺になってしまうため。
 *   戻り値は1.
 * 
 * ・それ以外のとき
 *   とりあえずDFSを呼び出し、戻り値は0.
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・木で2頂点x,yのパスを考えるとき、LCA(x,y)を必ず通る。
 *   木DPにて各頂点がLCAになるケースを数え上げていく、という手法。
 */

using Graph = vector<vector<int>>;

ll ans = 0;

// vの子の方から次数が 2,3,3,...,3 と続いている個数を返す
ll dfs(Graph &g, vector<ll> &deg, int v, int p = -1)
{
	if(deg[v] == 2)
	{
		ll sum = 0;
		for(auto &e : g[v])
		{
			if(e == p) continue;
			ll tmp = dfs(g, deg, e, v);
			if(deg[e] != 2) sum += tmp;  // 子の次数=2はダメ
		}
		ans += sum;
		return 1;

	}
	else if(deg[v] == 3)
	{
		ll sum = 0;
		vector<ll> cnt;
		for(auto &e : g[v])
		{
			if(e == p) continue;
			ll tmp = dfs(g, deg, e, v);
			cnt.push_back(tmp);  // 各子のDFS結果をvectorに積んでいく
			sum += tmp;
		}
		// 次数が2の子同士、かつ頂点vを通るような組み合わせを考えたい
		// cnt[i]とcnt[not i]との組み合わせの総和
		ll ans_tmp = 0;
		for(auto &ee : cnt)
		{
			ans_tmp += ee * (sum-ee);  // 自分と自分以外との積
		}
		ans_tmp /= 2;
		ans += ans_tmp;
		return sum;
	}
	else
	{
		// DFS呼び出すだけ
		for(auto &e : g[v])
		{
			if(e == p) continue;
			dfs(g, deg, e, v);
		}
		return 0;
	}
}

int main(void)
{
	int i;
	ll N; cin >> N;
	Graph g(N);
	vector<ll> deg(N);
	for(i = 0; i < N-1; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(v);
		g[v].push_back(u);
		deg[u]++;
		deg[v]++;
	}

	dfs(g, deg, 0);
	cout << ans << endl;

	return 0;
}
