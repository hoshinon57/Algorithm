#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC262 https://atcoder.jp/contests/abc262

/*
 * コンテスト中に解けず、解説を見て実装した。
 *   https://twitter.com/kyopro_friends/status/1553740920327409666
 *   https://atcoder.jp/contests/abc262/editorial/4479
 * 
 * 以下(1)(2)が成り立つ。(実際に図を描くと分かりやすい)
 * (1)ある頂点の次数が偶数のとき：
 *   頂点の色を変えても、「異なる色で塗られた頂点を結ぶ辺」の偶奇は変わらない。
 * (2)ある頂点の次数が奇数のとき：
 *   頂点の色を変えると、「異なる色で塗られた頂点を結ぶ辺」の偶奇は反転する。
 * 
 * 初期状態を「青頂点がN個、赤頂点が0個」とすると、「異なる色で～～～辺」は0本。つまり偶数。
 * ここから次数が偶数の頂点については、何個赤にしても「異なる色で～～～辺」は偶数のまま。
 * 一方で次数が奇数の頂点については、偶数個だけ赤にすれば「異なる色で～～～辺」が偶数のままとなる。
 * 
 * よって次数が奇数の頂点を「0個選んで赤にする」「2個選んで赤にする」…「K個(or K-1個)選んで赤にする」の
 * それぞれの場合の数を求めて、その総和が答となる。
 * ※「2個選んで赤にする」ときは、次数が偶数の頂点を「K-2個選んで赤にする」ことになる。
 * 
 * Combinationの計算が必要になるが、単純な実装ではlong long型をオーバーしたり
 * MODにて除算を正しく計算できなかったりする。
 * 以下のサイトからnCk_init(), nCk()を実装した。
 *   https://drken1215.hatenablog.com/entry/2018/06/08/210000
 *   https://cod-aid.com/library-binomial-coefficient
 *   https://algo-logic.info/combination/
 */
using Graph = vector<vector<int>>;

const ll MOD = 998244353;
const ll NCK_SIZE = 200001;  // 問題文の制約 N<=2*10^5 に合わせる
ll fact[NCK_SIZE], inv_fact[NCK_SIZE], inv[NCK_SIZE];

// Combinationの事前計算
void nCk_init(void)
{
	fact[0] = fact[1] = 1;
	inv[0] = inv[1] = 1;
	inv_fact[0] = inv_fact[1] = 1;

	for(int i = 2; i < NCK_SIZE; i++)
	{
		// 階乗
		fact[i] = fact[i - 1] * i % MOD;
		// 逆元
		inv[i] = MOD - inv[MOD%i] * (MOD / i) % MOD;
		// 逆元の階乗
		inv_fact[i] = inv_fact[i - 1] * inv[i] % MOD;
	}
}

// CombinationのnCkを求める
ll nCk(ll n, ll k)
{
	if (n < k) return 0;
	if (n < 0 || k < 0) return 0;

	ll x = fact[n];  // n!の計算
	ll y = inv_fact[n-k];  // (n-k)!の計算
	ll z = inv_fact[k];  // k!の計算
	
	return x * ((y * z) % MOD) % MOD;  //nCkの計算
}

int main(void)
{
	int i;
	int N, M, K;
	cin >> N >> M >> K;
	Graph graph(N);
	for(i = 0; i < M; i++)
	{
		int u, v;
		cin >> u >> v;
		u--;
		v--;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	// 次数が奇数である頂点の数を数える
	int odd_v = 0;
	for(i = 0; i < N; i++)
	{
		if(graph[i].size()%2 != 0) odd_v++;
	}
	int even_v = N - odd_v;

	nCk_init();

	ll answer = 0;
	for(i = 0; i <= K; i+=2)
	{
		// nCk()からの制約
		if(odd_v < i) continue;
		if(even_v < K-i) continue;

		// 次数が奇数の頂点からi個、偶数の頂点からK-i個選ぶ
		ll tmp = nCk(odd_v, i) * nCk(even_v, K-i);
		tmp %= MOD;
		answer = (answer + tmp)%MOD;
	}
	cout << answer << endl;

	return 0;
}
