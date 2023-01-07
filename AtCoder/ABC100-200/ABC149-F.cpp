#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1000000007;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題75より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc149/tasks/abc149_f

/*
 * 方針としては、頂点ごとに
 *   自身が白で、かつSに含まれるような塗り方の場合の数(★)
 * を加算していく。
 * これを最後に2^Nで割れば答となる。
 * 
 * まず(★)の求め方を考える。
 * ある頂点に対して、次数が1であれば「Sに含まれる白」にはなりえないため、0通り。
 * 2以上のとき、以下のように余事象で考える。
 * 頂点から先にある部分木について、2つ以上の部分木に黒があればよいので、
 *   (1)部分木が全て白のパターン：1通り
 *   (2)1つの部分木にだけ黒があり、残りの部分木は全て白のパターン：
 *      黒がある部分木の頂点数をXとすると、全て白(1パターン)の余事象になるため、
 *      (2^X)-1 通り。
 * としたときに、
 *   (頂点i以外のN-1個の白黒の選び方) - ( (1)+(2)のパターン数 )
 * が、ある頂点における(★)の場合の数となる。
 * 
 * これを全頂点に対して加算していき、
 * 全頂点の白黒の選び方(=2^N)で割ったものが答となる。
 * 
 * 次に、上記(2)の「部分木の頂点数」を求める必要がある。
 * これは頂点0を親としたDFSを実装した。
 * 各頂点iについて、その子側の部分木の頂点数をv_num_list[i]に入れていく。
 * 親側の頂点数は、 N-(1+Σv_num_list[*]) で求められる。(+1は自身の頂点)
 * 
 * 
 * AC後に公式解説を見ると、解法2の「頂点に注目する方法」に該当している。
 * また色々なWEBサイトの解説を見たところ、自分の実装と近そうなのは以下だった。
 * https://ikatakos.com/pot/programming_algorithm/contest_history/atcoder/2019/1229_abc149
 * https://starpentagon.net/analytics/surrounded_nodes/
 * https://maspypy.com/atcoder-%E5%8F%82%E5%8A%A0%E6%84%9F%E6%83%B3-2019-12-29abc-149
 * https://emtubasa.hateblo.jp/entry/2019/12/30/165145
 * 
 * 
 * 正確な計算量は分からず… O(N)かO(NlogN)とかだろうか…
 */

// mod mの世界におけるaの逆元を返す
// 以下URLのコードをそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modinv(long long a, long long m)
{
	long long b = m, u = 1, v = 0;
	while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= m; 
    if (u < 0) u += m;
    return u;
}

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	long long ans = 1;
	while(n > 0)
	{
		if((n&1) != 0)  // nの最下位ビットが立っているなら、答に掛ける
			ans = (ans * a) % mod;
		a = (a * a) % mod;
		n >>= 1;
	}
	return ans;
}

using Graph = vector<vector<int>>;
Graph graph;

// list[i]:頂点iから出る辺それぞれについて、その先の頂点数を保持する
vector<vector<int>> v_num_list;

int N;

// vを起点に子方向へ深さ優先探索し、頂点数を求めてv_num_list[]に登録していく
// parent:vの親
// 戻り値:v自身と、vの子の頂点数
int dfs(int v, int parent)
{
	int sum = 0;  // v自身と、vの子の頂点数
	for(auto &e : graph[v])
	{
		if(e == parent) continue;  // 親方向は見ない
		int n = dfs(e, v);  // 子方向へ再帰
		v_num_list[v].push_back(n);
		sum += n;
	}

	sum++;  // v自身を加算

	if(parent != -1)
	{
		v_num_list[v].push_back(N-sum);  // 親方向の頂点数を登録
	}
	// else 親が無いときは登録しない

	return sum;
}

int main(void)
{
	// 0-indexed
	int i;
	cin >> N;
	graph.resize(N);
	v_num_list.resize(N);
	for(i = 0; i < N-1; i++)
	{
		int a, b;
		cin >> a >> b;
		a--;  b--;  // 0-indexedに変換
		graph[a].push_back(b);
		graph[b].push_back(a);
	}

	// 頂点0を親として深さ優先探索
	// 各頂点ごとに出ている辺それぞれについて、その先の頂点数を計算してv_num_listに設定する
	dfs(0, -1);

	// 頂点iが「Sに含まれる白」になるようなケースは何通りか？ を調べる
	// count:上記の頂点ごとの場合の数
	// total:countの総和
	ll pow_2_Nminus1 = modpow(2, N-1, MOD);  // 2^(N-1)
	ll total = 0;
	for(i = 0; i < N; i++)
	{
		// 頂点iの次数が0か1であれば、頂点iは「Sに含まれる白」にはなりえないためスルー
		// (木なので0はありえないが、一応)
		if(v_num_list[i].size() < 2) continue;

		ll count = 0;
		count++;  // (1)周囲が全部白のパターン：1通り
		for(auto &e : v_num_list[i])  // e:頂点iから出る先の頂点数
		{
			// (2)
			// 部分木に黒が1個以上ある場合の数を計算する
			// 全て白(1パターン)の余事象となり、2^e-1 となる
			count += modpow(2, e, MOD) + MOD - 1;
			count %= MOD;
		}
		// 頂点iが「Sに含まれる白」になる場合の数は、以下となる
		// (頂点i以外のN-1個の頂点の白黒の選び方) - ( (1)+(2)のパターン数 )
		total += pow_2_Nminus1 + MOD - count;
		total %= MOD;
	}

	// 全ての場合の数 / 全ての白黒の選び方(=2^N) が答となる
	ll pow_2_N = modpow(2, N, MOD);  // 2^N
	ll answer = total * modinv(pow_2_N, MOD);
	answer %= MOD;
	cout << answer << endl;

	return 0;
}
