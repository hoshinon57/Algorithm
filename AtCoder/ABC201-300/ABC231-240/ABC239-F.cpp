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

// ABC239 https://atcoder.jp/contests/abc239

/*
 * 木の性質を用いた構築問題。むずい、これどうやったら思いつくんだ…
 * 「余り次数が小さいのと大きいのをくっつける」「最後は余り次数が1どうしをくっつける」など
 * 方針のイメージはわいたが、上手い実装ができなかった。
 *   https://atcoder.jp/contests/abc239/editorial/3388 およびリンク先の実装コード
 *   https://blog.bouzuya.net/2022/02/21/
 *   https://scrapbox.io/procon-kirokuyou/%E3%83%87%E3%83%B3%E3%82%BD%E3%83%BC%E3%82%AF%E3%83%AA%E3%82%A8%E3%82%A4%E3%83%88%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%82%B3%E3%83%B3%E3%83%86%E3%82%B9%E3%83%882022_(ABC239)_F_-_Construct_Highway_(500)
 *   https://bondo.hateblo.jp/entry/2022/02/20/202815
 * 
 * ※どこでどれぐらいNG判定を実装するか、は人によって結構違いそう。
 *   自分はある程度はしょり、最後に「連結成分数がNであること」でまとめた感がある。
 * 
 * 基本は上記の解説の通り。
 * まず既に存在する道路の入力について、A,BごとにDを減らしていく。
 * ここでD<0になったらNG終了。
 * ※この時点でサイクル判定によるNG終了をやってもよいが、最後の連結成分数判定でどうせひっかかるので、ここではやらなかった。
 * 
 * 以降の辺構築は、D[i]を残り次数として、
 *   連結成分ごとにDの総和を管理する。
 *   D=1とD>1とで辺を追加していく。D=1どうしを繋ぐと、それ以上辺を拡張できないため避ける。
 *   ※D>1どうしで辺をつなげるケースも(たぶん)存在するが、その判定が難しく、またやる必要もない(処理が複雑になるだけ)。
 *   最後はD=1が2つ残っていて、それどうしを繋げる。
 * という方針になる。
 * 実装では、D=1をc1に、D>1をc2にまとめた。
 * 後者は c2[i]={1,1,3,4,4,4,...} のように、d[4]=3であれば頂点4を3回登録している。
 * できたc1とc2とで辺を構築していく。
 * ※実装方針はもちろん人によるが、c2を使っていった後、D=1になればc1に登録するのがポイントか。
 * 
 * あとは、
 *   c1に唯一2つ残っているはずなので、それを統合。(そうでなければNG)
 *   全体の連結成分数がNじゃなければNG.
 * という判定をしたのち、解を出力すれば完了。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・構築問題は「どのように構築していくか」「どのようにNG判定ができるか」を考えてみる。
 *   ※まあ本問はそれがイメージできても実装できなかったが…
 */

void end(void)
{
	cout << -1 << endl;
	exit(0);
}

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

int main(void)
{
	// 0-indexed
	int i;
	int N, M; cin >> N >> M;
	vector<int> d(N);
	for(i = 0; i < N; i++) cin >> d[i];
	if(reduce(d.begin(), d.end()) != 2*(N-1)) end();

	UnionFind uf(N);
	for(i = 0; i < M; i++)
	{
		int u, v; cin >> u >> v;
		u--; v--;
		uf.unite(u, v);
		d[u]--; d[v]--;
		if(d[u] < 0 || d[v] < 0) end();  // Dが足りない、NG
	}

	// 連結成分ごとにDをまとめる
	vector<vector<int>> sumd(N);
	for(i = 0; i < N; i++)
	{
		// sumd[*]={a,a,b,b,b, ..., i,i,i, ...}
		// 根をiとした連結成分について、次数を1つごとに頂点番号を登録
		for(int j = 0; j < d[i]; j++) sumd[uf.root(i)].push_back(i);
	}

	// 連結成分ごと、残りDが1と2以上とで管理
	vector<int> c1;
	vector<vector<int>> c2;
	for(i = 0; i < N; i++)
	{
		if(sumd[i].size() == 0) continue;
		if(sumd[i].size() == 1) c1.push_back(sumd[i][0]);
		else c2.push_back(sumd[i]);
	}

	// c1とc2とで辺を構築していく
	vector<pair<int,int>> ans;
	for(auto &e2 : c2)  // e2 = {a,a,b,b,b,...}
	{
		for(i = 0; i < (int)e2.size()-1; i++)  // 1個は残してc1へ
		{
			int u, v;
			if(c1.empty()) end();  // 次数1の連結成分無し、次数2以上の連結成分あり、という状況はNG
			u = c1.back();
			v = e2[i];
			uf.unite(u, v);
			ans.push_back({u+1, v+1});
			c1.pop_back();
		}
		c1.push_back(e2.back());
	}

	// 次数が1残っている、という成分が唯一2つだけ存在するはず
	if(c1.size() != 2) end();
	uf.unite(c1[0], c1[1]);
	ans.push_back({c1[0]+1, c1[1]+1});

	if(uf.size(0) != N) end();

	for(auto &e : ans)
	{
		cout << e.first << " " << e.second << endl;
	}

	return 0;
}
