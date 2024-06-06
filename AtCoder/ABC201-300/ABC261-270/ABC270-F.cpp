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

// ABC270 https://atcoder.jp/contests/abc270

/*
 * 最小全域木で解く。
 * 以下と同じ考え方だった。
 *   https://x.com/kyopro_friends/status/1573672892843589632
 *   https://scrapbox.io/procon-kirokuyou/%E3%83%88%E3%83%A8%E3%82%BF%E8%87%AA%E5%8B%95%E8%BB%8A%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%82%B3%E3%83%B3%E3%83%86%E3%82%B9%E3%83%882022
 *   https://qiita.com/gotoki_no_joe/items/126eaba4f553c9ef13be
 * 
 * 空港に関する超頂点と、港に関する超頂点をそれぞれ用意し、
 * (空港を使うか, 港を使うか)のtrue/falseを
 *   1:(true,true)
 *   2:(true,false)
 *   3:(false,true)
 *   4:(false,false)
 * の4パターンでそれぞれ最小全域木を求め、その最小値が答となる。
 * パターン4のみ、MSTが作れないことがありうる。頂点0グループの頂点数を調べ、N以外ならMSTを作れていないと判定する。
 * 
 * 4パターンそれぞれ実装するのは大変なので、関数化した。
 * 辺は空港/港の辺も常に保持し、
 *   空港の辺だが、今は空港を使わないパターンならその辺は無視
 * としている。
 * 詳細は実装を参照。
 * 
 * パターン1のみで十分ではないか？と考えたが、
 * 本来は道路のみを使うケースが最適にも関わらず、空港に関する辺を使ってしまうことで
 * 正しい値が求まらないケースが存在する。
 * よって4パターンそれぞれ計算必要。
 * 
 * [ACまでの思考の流れ]
 * ・辺の最小コスト問題なので、MSTっぽい。
 * ・ただ空港と港の存在がやっかい。
 *   1つ作っただけでは辺にならないし、2つ以上作ると相互に辺を張ることになるし。
 *   ⇒超頂点を思いついた。
 * ・超頂点は無条件で用意して大丈夫か？
 *   ⇒道路のみを使うMSTが最適のケースにて、空港の辺を入れておくと正しく求まらないケースがありそう。
 *     超頂点を使う/使わないのそれぞれで計算が必要っぽい。
 *     超頂点は空港と港があるので、2*2=4パターンのMSTを求める必要がありそう。
 */

struct UnionFind
{
	vector<int> parent;  // parent[x]:xの親の番号 xが根であれば-1
	vector<int> siz;  // siz[x]:xの属するグループの頂点数

	// 頂点数nで初期化
	UnionFind(int n) : parent(n, -1), siz(n, 1) {}

	// 根を求める
	int root(int x)
	{
		if(parent[x] == -1) return x;  // xが根であれば、xを返す
		else
		{
			parent[x] = root(parent[x]);
			return parent[x];
		}
	}

	// xとyが同じグループに属するか
	bool issame(int x, int y)
	{
		return root(x) == root(y);  // 根が同一なら同じグループ
	}

	// xを含むグループとyを含むグループを併合する
	bool unite(int x, int y)
	{
		// x,yをそれぞれ根まで移動させる
		x = root(x);
		y = root(y);

		// すでに同じグループであれば何もしない
		if(x == y) return false;

		// 頂点数が少ない方が子になるようにする
		if(siz[x] < siz[y])
		{
			// xをyの子にする
			parent[x] = y;
			siz[y] += siz[x];
		}
		else
		{
			// yをxの子にする
			parent[y] = x;
			siz[x] += siz[y];
		}
		return true;
	}

	// xを含むグループの頂点数
	int size(int x)
	{
		return siz[root(x)];
	}
};

// MSTに使う用の辺
// 辺番号など情報を追加したい場合は、メンバ変数およびコンストラクタ、"<"演算子に手を入れること
struct Edge
{
	int u, v;  // 頂点
	ll w;  // 辺の重み
	Edge(void) {}
	Edge(int u_, int v_, ll w_) : u(u_), v(v_), w(w_) {}
	bool operator<( const Edge &right ) const
	{
		return this->w < right.w;
	}
};

int main(void)
{
	int i;
	int N, M; cin >> N >> M;  // 0～(N-1):島, N:空港超頂点, (N+1):港超頂点
	vector<Edge> edge;
	for(i = 0; i < N; i++)
	{
		ll x; cin >> x;
		edge.push_back({i, N, x});
	}
	for(i = 0; i < N; i++)
	{
		ll y; cin >> y;
		edge.push_back({i, N+1, y});
	}
	for(i = 0; i < M; i++)
	{
		int a, b;
		ll z;
		cin >> a >> b >> z;
		a--; b--;
		edge.push_back({a, b, z});
	}
	sort(edge.begin(), edge.end());

	// 空港を使うならair=true, 港を使うならport=true
	auto cal = [&](bool air, bool port) -> ll
	{
		ll ret = 0;
		UnionFind uf(N+2);
		for(auto &e : edge)
		{
			if(uf.issame(e.u, e.v)) continue;
			if((e.u == N || e.v == N) && !air) continue;  // 空港を使わない場合(!air), その辺はスルー
			if((e.u == N+1 || e.v == N+1) && !port) continue;

			ret += e.w;
			uf.unite(e.u, e.v);
		}
		// 空港と港を使わないケースにて、MSTが作れないケースをチェック
		if(!air && !port)
		{
			if(uf.size(0) != N) ret = INF64;
		}
		return ret;
	};

	ll ans = INF64;
	chmin(ans, cal(true, true));
	chmin(ans, cal(true, false));
	chmin(ans, cal(false, true));
	chmin(ans, cal(false, false));
	cout << ans << endl;

	return 0;
}
