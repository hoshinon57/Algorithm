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

// ABC355 https://atcoder.jp/contests/abc355

/*
 * コンテスト中に解けず、解説を見た。
 * クラスカル法の気持ちになって解く。
 * snuke氏の解説のうち、1つ目の方針。
 *   https://www.youtube.com/live/4-TEF7DPvEY
 *   https://atcoder.jp/contests/abc355/submissions/53902472
 * 
 * 「コスト1以下の辺のみを使ってクラスカル法」、「コスト2以下の辺のみを使ってクラスカル法」…「コスト10以下の辺のみを使ってクラスカル法」
 * をそれぞれ考えると、それぞれのグラフでの辺数をn[i]として、
 *   Σ(k*(n[k]-n[k-1]))
 * が最小全域木のコストとなる。
 * 例えばコストが2以下と3以下を比べると、
 * n[2]からn[3]への増分は、MSTにてコスト3の辺が採用された本数と等しくなるため。なるほど…
 * 
 * 制約より辺のコスト最大は10のため、MSTを10本管理して十分に間に合う。
 * 
 * 上記をほぼそのまま実装すればよい。
 * (N-1)本の辺とクエリのQ本を同一のforループにまとめている。
 * 0-indexedで(N-1)番目以降の辺がクエリになる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・辺のコストが10以下、という制約はどう見てもポイントだったが、そこから先に行けなかった。
 * ・手元でコストが1～3の範囲でいくつか辺を作ってみて、法則を見つけ出す感じかなあ。
 * ・クラスカル法の気持ちを理解することが必要。
 *   まずはコスト1の辺のみを使い、次はコスト2の辺のみを使い…という形。
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

int main(void)
{
	int N, Q; cin >> N >> Q;
	const int CM = 10;  // 重み最大値
	vector<UnionFind> uf(CM+1, UnionFind(N));
	vector<int> num(CM+1);  // num[i]:コストiまでのグラフにおける辺数
	int i, j;
	for(i = 0; i < N-1+Q; i++)
	{
		int a, b, c; cin >> a >> b >> c;
		a--; b--;
		for(j = c; j <= CM; j++)  // グラフは「コストxまで」なので、c～CMが対象
		{
			if(uf[j].issame(a, b)) continue;
			uf[j].unite(a, b);
			num[j]++;
		}
		if(i < N-1) continue;

		// クエリ処理
		int ans = 0;
		for(j = 1; j <= CM; j++)
		{
			ans += j * (num[j]-num[j-1]);
		}
		cout << ans << endl;
	}

	return 0;
}
