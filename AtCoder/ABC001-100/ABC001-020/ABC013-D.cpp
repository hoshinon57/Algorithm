#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC013 https://atcoder.jp/contests/abc013

/*
 * まず、あみだくじを1回やったときに各列ごとにfrom->toが一意に定まるので(当たり前だけど)、
 * それを元にfrom->toへ辺を張ったグラフを構築する。
 * すると連結成分ごとにサイクルになっている。
 * 
 * 連結成分ごとにサイクルの頂点番号を列挙して、各頂点からD進んだ先を簡単に計算できる。
 * サイクルの長さをcyc, サイクル上でj番目であれば、(j+D)&cycで求まる。
 * 
 * ※最初、「練習がてら行列累乗でやるかー」とか考えてたが、
 *   計算量がO(N^3*logK)とかなのでダメ。
 *   ダブリングなら大丈夫ね。公式解説がこれか。
**/

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

int main(void)
{
	ll i, j;
	ll N, M, D; cin >> N >> M >> D;

	// nx[i]=x 元の左からx列目は、後でi列目に行く x->i
	vector<ll> nx(N);
	iota(nx.begin(), nx.end(), 0LL);
	for(i = 0; i < M; i++)
	{
		ll a; cin >> a;
		a--;
		// 列aと列(a+1)をswap
		swap(nx[a], nx[a+1]);
	}
	Graph g(N);
	for(i = 0; i < N; i++)
	{
		g[nx[i]].push_back(i);
	}

	vector<bool> seen(N, false), finished(N, false);
	vector<int> ans(N);
	for(i = 0; i < N; i++)
	{
		if(seen[i]) continue;
		vector<int> hist;
		dfs_cycle_detection_directed(g, i, seen, finished, hist);
		// 必ずサイクル検出する
		hist.pop_back();
		ll cyc = (ll)hist.size();
		for(j = 0; j < (ll)hist.size(); j++)
		{
			ll to = (j+D)%cyc;
			ans[hist[j]] = hist[to] + 1;  // +1は1-indexedへ変換
		}
	}
	for(auto &e : ans) cout << e << endl;

	return 0;
}
