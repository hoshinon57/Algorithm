#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題66
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1127

/*
 * クラスカル法で最小全域木を構築して解く。
 * 
 * ※問題文に日本語訳が存在しない。
 *   日本語は以下記事を参考。
 *   https://www.utakata.work/entry/2015/06/08/011704
 *  
 * セルを頂点、廊下を辺とみなす。
 * セルを2つ選ぶ全ての組み合わせについて、廊下の長さを計算してEdgeに登録していく。
 * 廊下の長さは以下で計算できる。
 *   (2つのセルの距離) - (2つのセルの半径の和)
 * 上記式が0未満ならば、セル同士が重なっている。
 * このときは廊下の建築は不要であり、下限0でクリップする。
 * 
 * あとは廊下の長さが短い順にソートしてから、最小全域木を構築すればよい。
 * 
 * 辺の数がN^2であり、計算量はソート部分でO(N^2*logN).
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

// 2つのセル(頂点)と、それを結ぶ廊下(辺)を指す構造体
struct Edge
{
	int a, b;  // セルの番号
	double dist;  // 廊下の長さ
	Edge(int a_, int b_, double d_) : a(a_), b(b_), dist(d_) {}
};
bool myCompare(const Edge &a, const Edge &b) { return a.dist < b.dist; }

int main(void)
{
	// 0-indexed
	int i, j;
	while(true)
	{
		int n;
		cin >> n;
		if(n == 0) break;

		vector<double> x(n), y(n), z(n), r(n);
		for(i = 0; i < n; i++)
		{
			cin >> x[i] >> y[i] >> z[i] >> r[i];
		}

		// セル(頂点)を2つ選ぶ全ての組み合わせについて、廊下(辺)の長さを計算してedgeに登録する
		vector<Edge> edges;
		for(i = 0; i < n; i++)  // 1つ目のセル
		{
			for(j = i+1; j < n; j++)  // 2つ目のセル
			{
				// 廊下の長さは以下で計算できる。
				//   (2つのセルの距離) - (2つのセルの半径の和)
				// 上記式が0未満ならば、セル同士が重なっている。
				// このときは廊下の建築は不要であり、下限0でクリップする。
				double tmp = (x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j]) + (z[i]-z[j])*(z[i]-z[j]);
				tmp = sqrt(tmp);
				double dist = tmp - (r[i]+r[j]);
				if(dist < 0.0) dist = 0.0;  // 下限0でクリップ

				edges.push_back({i, j, dist});
			}	
		}

		// 廊下の長さが短い順にソート
		sort(edges.begin(), edges.end(), myCompare);

		// クラスカル法で最小全域木を求める
		double answer = 0.0;
		UnionFind uf(n);
		for(auto &e : edges)
		{
			if(uf.issame(e.a, e.b)) continue;

			answer += e.dist;
			uf.unite(e.a, e.b);
		}
		cout << std::fixed << std::setprecision(3);  // 小数点以下3桁で表示、の指示
		cout << answer << endl;
	}

	return 0;
}
