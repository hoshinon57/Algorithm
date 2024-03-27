#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1e9+7;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:73 https://atcoder.jp/contests/typical90/tasks/typical90_bu

/*
 * 自力で解けず、解説AC. いやーこれは解説見ても理解に時間がかかった、難しかった。
 *   https://takeg.hatenadiary.jp/entry/2021/12/03/113718    これが分かりやすかった
 *   https://twitter.com/e869120/status/1407510677934149635
 *   https://twitter.com/AtCoder8/status/1408071367694503945
 * 
 * dp[][]の定義について、ソースコード内のコメントを参照。答はdp[0][2].
 * 頂点vが'a'のときを考えてみる。
 * dp[v][0]：vの子の部分木が、
 *   aのみからはOK (つなげる)
 *   bのみからはNG (つないだら[0]にならないし、切ったら部分木側がa,b含まれない)
 *   a,b両方からはOK (切る)
 * dp[v][2]：
 *   aのみからはOK (つなげる) (※1)
 *   bのみからはOK (つなげる)
 *   a,b両方からはOK (つなげる、切る(※2)どちらも可能)
 * これらの和を、子頂点のdp[e][*]ごとに計算し、子頂点ごとに独立なので積算すればよい。
 * 
 * (※1)はその時点ではa,bともに含まれず、dp[v][2]に加算してよいのか？となる。
 * これは他の辺からbが入ってくれば良いので、ひとまず足しておくという考え方。
 * dp[v][0]が含まれちゃってるのは事実なので、後でdp[v][0]を引いてやればOK.
 * (※2)も同様。
 */

using Graph = vector<vector<int>>;
Graph g;
int N;
vector<int> c;
// dp[i][j]
//   頂点iの部分木を考えたときに、 (i=0～N-1)
//   現在の連結成分に j=0:aのみ含まれる, j=1:bのみ, j=2:a,b両方含まれる
//   ときの通り数
vector<vector<ll>> dp;

void dfs(int v, int p = -1)
{
	ll val1 = 1, val2 = 1;
	for(auto &e : g[v])
	{
		if(e == p) continue;
		dfs(e, v);
		if(c[v] == 0)  // 'a'
		{
			val1 *= dp[e][0] + dp[e][2];
			val2 *= dp[e][0] + dp[e][1] + dp[e][2]*2;
		}
		else
		{
			val1 *= dp[e][1] + dp[e][2];
			val2 *= dp[e][0] + dp[e][1] + dp[e][2]*2;
		}
		val1 %= MOD;
		val2 %= MOD;
	}
	if(c[v] == 0)
	{
		dp[v][0] = val1;
		dp[v][2] = (val2-val1+MOD)%MOD;
	}
	else
	{
		dp[v][1] = val1;
		dp[v][2] = (val2-val1+MOD)%MOD;
	}
}

int main(void)
{
	int i;
	cin >> N;
	c.resize(N);
	for(i = 0; i < N; i++)
	{
		char ch; cin >> ch;
		c[i] = ch-'a';
	}
	g.resize(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	dp.resize(N, vector<ll>(3, 0));
	dfs(0);
	cout << dp[0][2] << endl;

	return 0;
}
