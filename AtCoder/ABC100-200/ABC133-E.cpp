#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1000000007;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC133 https://atcoder.jp/contests/abc133

/*
 * 深さ優先探索で解く。
 * 
 * dfs()を以下のように定義する。
 *   void dfs(int v, int parent, int d1_num, int d2_num)
 *   頂点vを起点にDFS. 親はparent.
 *   d1_num,d2_num：vから親方向へ見て、距離がそれぞれ1,2の頂点数
 * 
 * すると、頂点vに使える色の種類は K-(d1_num+d2_num) となる。(下限0でクリップ必要)
 * 子へ再帰していくときは、
 *   まずd1_num -> d2_numへスライド
 *   d1_numはv自身の1
 *   vの子のループにて、d2_numを1ずつ足していく
 * とする。
 * 
 * ※解説の中では、hamayanhamayan氏のものと近かった。
 *   https://blog.hamayanhamayan.com/entry/2019/07/08/212955
 */

using Graph = vector<vector<int>>;
Graph g;
int N, K;

ll ans = 1;

// 頂点vを起点にDFS.
// vの親はparentであり、親方向へは探索しないようにする。
// d1_num,d2_num：vから親方向へ見て、距離がそれぞれ1,2の頂点数
void dfs(int v, int parent, int d1_num, int d2_num)
{
	// 頂点vに使える色数は、 K-(d1_num+d2_num) となる
	ans *= max(K-(d1_num+d2_num), 0);  // 下限0でクリップ  クリップされたら以降は全て0
	ans %= MOD;

	// vの子を探索するにあたって更新
	d2_num = d1_num;  // 距離1のを2へスライド
	d1_num = 1;  // v (eから見た親)

	for(auto &e : g[v])
	{
		if(e == parent) continue;  // 親へは行かない
		dfs(e, v, d1_num, d2_num);
		d2_num++;  // きょうだい
	}
}

int main(void)
{
	int i;
	cin >> N >> K;
	g.resize(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}

	dfs(0, -1, 0, 0);
	cout << ans << endl;

	return 0;
}
