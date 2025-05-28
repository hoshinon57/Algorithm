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

// ABC030 https://atcoder.jp/contests/abc030

/*
 * 入力はfunctional graph.
 * なので始点aからDFSしてサイクルを見つける方針で良い。
 * ステップkが莫大なのが厄介。
 * 
 * いったんkの大きさはおいといて、
 * サイクルに入る前の頂点数をpre,サイクルのループ数をcycとすると、
 * (k-pre)%cyc が求めたい位置となる。
 * これは
 *   k%=cyc
 *   k-=pre
 *   k<0なら0以上になるまでcycを加算
 * という計算順でも求められる。
 * よって文字列kに対して k%=cyc の部分を頑張って求める。⇒ str_val_mod()
 * 
 * サイクルに入る前にステップが終わるケースを考える必要がある。
 * stoll(k)としてもkが大きすぎると確かエラーになるので、
 * まず適当な文字列長でkのサイズを判定した後、stoi(k)とpreを比較する方針とした。
 * ⇒頂点数Nが10^5なので、7文字とかで区切れば十分。
 *   10文字区切りにし、stoi()を使ってしばらくRE/WAだった。stoi(k)でオーバーフローしてた。
**/

// n%mを計算し、非負つまり[0,m)の範囲で返す
// 参考：https://atcoder.jp/contests/abc376/submissions/58995270 (ecottea氏のテンプレート)
template<typename T>
T smod(T n, T m) {
	n %= m;
	if(n < 0) n += m;
	return n;
}

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

// sを数値として見たとき、mで割った余りを返す
// [制約]sを数値として見たときに合法であること、0以上であること
ll str_val_mod(string s, ll m) {
	ll ret = 0;
	for(auto &c : s) {
		ret = (ret*10 + c-'0') % m;
	}
	return ret;
}

int main(void)
{
	ll i;
	ll N, a; cin >> N >> a;
	a--;
	string k; cin >> k;
	Graph g(N);
	for(i = 0; i < N; i++)
	{
		ll b; cin >> b;
		b--;
		g[i].push_back(b);  // i->bi
	}

	vector<bool> seen(N, false), finished(N, false);
	vector<int> hist;
	dfs_cycle_detection_directed(g, a, seen, finished, hist);

	// サイクルに入る前の頂点数と、サイクルのループ数
	ll pre = find(hist.begin(), hist.end(), hist.back()) - hist.begin();
	ll cyc = hist.size() - pre - 1;

	// サイクルに入る前に終わるケースを探索
	if(k.size() <= 7)
	{
		auto ki = stoi(k);
		if(ki < pre)
		{
			cout << hist[ki]+1 << endl;
			return 0;
		}
	}

	ll idx = str_val_mod(k, cyc);
	idx -= pre;
	idx = smod(idx, cyc);
	cout << hist[pre+idx]+1 << endl;

	return 0;
}
