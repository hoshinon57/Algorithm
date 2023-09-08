#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 998244353;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC222 https://atcoder.jp/contests/abc222

/*
 * DFSとDPで解く。
 * 
 * 操作によって辺ごとに何回通ったかをカウントしておき、
 * 辺ごとに赤青を塗り分けることでR-B=Kとなるような組み合わせを求める、と考える。
 * 前者をDFS, 後者をDPで求める。
 * 
 * (1)DFS部分
 * cnt[i]:i番目の辺を使う回数 を用意しておく。
 * bool dfs(Graph &g, int v, int goal, int p = -1)
 * にてv->goalへのパスを探索する。これを各Ai->A(i+1)について呼び出す。
 * 
 * dfs()の中身は、vがgoalにたどり着いたらtrueを返し、
 * 再帰呼び出し元でもtrueでそのまま返す。
 * cntは、再帰呼び出し前に+1, falseで返ってきたら-1する。(行きがけ時/帰りがけ時の考えに近い)
 * 
 * (2)DP部分
 * dp[i][j]を、
 *   cnt[i]まで見て、(i:0～N-1)
 *   j=R-Bとなるような  (j:-K～0～+K  K=NM)
 *   辺の塗り方の個数
 * と定義すると、比較的単純なDPに見える。
 * …が、2つの問題がある。
 *   (2-a)
 *     dp[i][j]のサイズがN*NMで10^8にもなるため、MLE/TLEが不安。
 *   (2-b)
 *     j部分が負になるケースがある。
 * これらを解決するため、mapを用いることとした。詳しくは実装を参照。
 * 計算量が気になったが、
 *   例えばcnt={2,2,2,2}の場合、-8～0～8の計9通りしかない。
 *   要素数が一番多くなるのは cnt={1,2,4,8,16, ...}のようなケースと思われるが、
 *   cntの最大値はM(=100)なので、これでも大して増えなさそう。
 * と要素数はそれほど増えず、むしろループごとN*M回計算するより早いかも、と予測した。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・各辺が、操作全体で何回通るかを考える。
 * ・DPについて、「mapやsetを用いた方が良いケースがある」というのを覚えておく。
 *   取りうる要素数の範囲がとても広い（とはいえ実際に取る種類は少ない）とか、
 *   添字が負になるので、実装はできるもののバグりやすいとか。
 */

// 頂点を結ぶ辺
struct Edge
{
	int no;  // 辺の番号
	int to;  // 隣接頂点番号
	Edge(int no_, int to_) : no(no_), to(to_) {}
};
using Graph = vector<vector<Edge>>;

vector<int> cnt;  // cnt[i]:i番目の辺を使う回数

// p:vの親 逆流防止
bool dfs(Graph &g, int v, int goal, int p = -1)
{
	if(v == goal) return true;

	for(auto &e : g[v])
	{
		if(e.to == p) continue;
		cnt[e.no]++;
		if(dfs(g, e.to, goal, v)) return true;
		cnt[e.no]--;
	}
	return false;
}

int main(void)
{
	// 0-indexed
	int i;
	int N, M, K; cin >> N >> M >> K;
	vector<int> a(M); for(i = 0; i < M; i++) {cin >> a[i]; a[i]--;}
	Graph g(N);
	for(i = 0; i < N-1; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		g[u].push_back(Edge(i, v));
		g[v].push_back(Edge(i, u));
	}
	cnt.resize(N);

	for(i = 0; i < M-1; i++)
	{
		dfs(g, a[i], a[i+1]);
	}

	map<int, ll> mp;  // mp[値] = その個数
	mp[0] = 1;
	// 配るDP
	for(i = 0; i < N-1; i++)
	{
		map<int, ll> mp_new;
		for(auto &[val, c] : mp)
		{
			mp_new[val+cnt[i]] += c;
			mp_new[val+cnt[i]] %= MOD;
			mp_new[val-cnt[i]] += c;
			mp_new[val-cnt[i]] %= MOD;
		}
		swap(mp, mp_new);
	}
	cout << mp[K] << endl;

	return 0;
}
