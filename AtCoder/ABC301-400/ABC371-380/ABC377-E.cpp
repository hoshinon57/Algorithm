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

// ABC377 https://atcoder.jp/contests/abc377

/*
 * 自力で解けず、解説を見た。
 * 
 * Pの初期値(入力値)について i->Pi へ辺を張るグラフを考える。
 * Pが1,2,...,Nの順列であることから、連結成分ごとに「ヒゲの無い」サイクルとでも書けば良いのか、
 * 全頂点の次数が2であるような形になっている。
 * 
 * v=iである頂点について、2^Kマス先に進んだ頂点が、iに対する答となる。
 * これ自分で説明がつかないが、「行き先の頂点も操作で同じだけ進むので」とか、そういう感覚だろうか…
 * 
 * 実装としては、連結成分ごとにサイクルを列挙し、頂点ごと2^K先のマスを計算する。
 * 2^Kは大きすぎるので、サイクル長で割った余りで考えればよい。
 * 
 * ※初期値のPiを基点に、答の各Pを求める考え方もある。
 *   この場合、求めるのは (2^K)-1 マス先となる。
 *   初期値の各Pに対し、idx[P[i]]=i となるインデックスを最初に求めておく。
 *   https://atcoder.jp/contests/abc377/submissions/59234332
 */

using Graph = vector<vector<int>>;

// 有向グラフにて、頂点vを起点にDFS.
// サイクルを見つけたらtrueを返す。その場合、vからサイクル終端までをhistoryに保持する。
//   historyの例：0->1->2->3->4->2  サイクルは2-3-4の部分。末尾の2が2回出てくることに注意すること。
// seen, finishedの要素数は頂点数で、falseで初期化しておくこと。
// 
// このDFSにて、
// (1)1つの連結成分に2つ以上のサイクルがある場合、
//    サイクル有無の判定は可能。
//    サイクル検出は何らか1つなら可能。全てのサイクル検出は不可能。
// (2)Functional Graphなら連結成分ごとにサイクルは1個なので、
//    呼び出し元で未探索の頂点からDFSすることで全てのサイクルを検出可能。
bool dfs_cycle_detection_directed(Graph &g, int v, vector<bool> &seen, vector<bool> &finished, vector<int> &history)
{
	seen[v] = true;
	history.push_back(v);  // vを行きがけ時に登録し、帰りがけ時に削除

	for(auto &e : g[v])
	{
		if(finished[e]) continue;
		if(seen[e] && !finished[e])
		{
			// サイクル検出した
			finished[v] = true;  // 頂点vについて探索完了
			history.push_back(e);  // サイクルの終端
			return true;
		}
		if(dfs_cycle_detection_directed(g, e, seen, finished, history))  // サイクル検出してreturnしてきた場合は、再帰的にreturnしていく
		{
			finished[v] = true;  // この場合も、頂点vについて探索完了扱いとする
			return true;
		}
	}

	finished[v] = true;
	history.pop_back();
	return false;
}

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// ※modは素数といった制約は無い
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	// a %= mod;  // a*aにてオーバーフローしそうな場合に有効化
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

int main(void)
{
	// 0-indexed
	int i, j;
	ll N, K; cin >> N >> K;
	Graph g(N);
	vector<ll> p(N);
	for(i = 0; i < N; i++) {
		cin >> p[i]; p[i]--;
		g[i].push_back(p[i]);  // i->Pi
	}
	vector<int> ans(N);
	vector<bool> seen(N, false), finished(N, false);
	for(i = 0; i < N; i++)
	{
		if(seen[i]) continue;  // 探索済み
		vector<int> hist;
		dfs_cycle_detection_directed(g, i, seen, finished, hist);
		// 連結成分ごと、頂点の次数は全て2になっている
		hist.pop_back();  // ライブラリの仕様上、末尾に重複があるため
		ll lp = hist.size();  // サイクル長
		ll mov = modpow(2, K, lp);  // 2^Kマス移動先 サイクルのサイズでMODを取る
		for(j = 0; j < lp; j++)  // サイクルのj番目の頂点について
		{
			int idx = hist[j];  // ans[idx]
			ans[idx] = hist[(j+mov)%lp];
			seen[idx] = true;
		}
	}

	int sz_ = (int)ans.size();
	for(i = 0; i < sz_; i++) {
		cout << ans[i]+1;
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}
