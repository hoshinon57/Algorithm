#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC259 https://atcoder.jp/contests/abc259

/*
 * Union-Findで解く。
 * 
 * 以下の3ステップで解く。(1)(2)はABC259-D.cppと同じ。
 * (1)全ての円どうしで交差または接するかの判定
 * (2)点s,tが円1～円Nのどの円周上にいるか判定
 * (3)点sの円と点tの円が同じグループにいるか、Union-Findで解く
 * 
 * (1)全ての円どうしで交差または接するかの判定
 * 2つの円i,jを全列挙し、それぞれについて交差判定を行う。
 * 条件はcheck_circle_cross()のコメントを参照。
 * 交差していれば、Union-Findにて2つの円を同じグループに統合する。
 * 
 * (2)点s,tが円1～円Nのどの円周上にいるか判定
 * 複数の円上にいた場合、どの円を採用しても結果は同じとなる。(1)により複数の円は同じグループに属しているため。
 * よって先優先でよい。
 * 
 * (3)点sの円と点tの円が同じグループにいるか、Union-Findで解く
 * (2)で求めた点s,tの円が同じグループか否か、で判定できる。
 * 同じグループであれば、点sから点tへ移動可能。
 */

ll sx, sy, tx, ty;
vector<ll> x, y, r;

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

// 円i,jが交差または接するかの判定
// 交差する条件は、円の中心間の距離をdとして、
//   |r-r'| <= d <= r+r'
// となる。
// 参考：https://examist.jp/mathematics/figure-circle/two-circle/
bool check_circle_cross(ll i, ll j)
{
	// 本問題ではACだったが、誤差の観点でsqrtは使わない方が良い。
	ll dx = x[i] - x[j];
	ll dy = y[i] - y[j];
	long double tmp = sqrtl(dx*dx+dy*dy);
	if((long double)(abs(r[i]-r[j])) <= tmp && tmp <= (long double)(r[i]+r[j]))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 点(x,y)が円i上にいるか判定
// 点と円iの中心の距離を求めて、それが半径rと等しいかで判定する
bool check_dot(ll dot_x, ll dot_y, ll i)
{
	// sqrtを使わず、2乗した状態で判定する
	ll dx = dot_x - x[i];
	ll dy = dot_y - y[i];
	ll tmp1 = dx*dx+dy*dy;
	ll tmp2 = r[i]*r[i];
	if(tmp1 == tmp2)
	{
		return true;
	}
	return false;
}

int main(void)
{
	// 0-indexed
	ll N;
	ll i, j;
	cin >> N;
	cin >> sx >> sy >> tx >> ty;
	x.resize(N);
	y.resize(N);
	r.resize(N);
	for(i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i] >> r[i];
	}

	UnionFind uf(N);
	// 円i,jが交差または接するかの全チェック
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)
		{
			if(check_circle_cross(i, j))
			{
				uf.unite(i, j);  // 2つの円を同じグループにする
			}
		}
	}

	// 点s,tがどの円周上にいるか
	// 複数の円上にいた場合、どの円を採用しても結果は同じになるので、先優先でよい。
	ll s_circle_no, t_circle_no;
	for(i = 0; i < N; i++)
	{
		if(check_dot(sx, sy, i))
		{
			s_circle_no = i;
			break;
		}
	}
	for(i = 0; i < N; i++)
	{
		if(check_dot(tx, ty, i))
		{
			t_circle_no = i;
			break;
		}
	}

	// 点sの円と点tの円が同じグループにいるか判定
	if(uf.issame(s_circle_no, t_circle_no))
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
