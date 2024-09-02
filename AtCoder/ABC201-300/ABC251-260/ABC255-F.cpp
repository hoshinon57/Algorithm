#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <functional>  // function
#include <limits>  // numeric_limits
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC255 https://atcoder.jp/contests/abc255

/*
 * 深さ優先探索で解く。各種解説と同一方針だった。
 *   https://atcoder.jp/contests/abc255/editorial/4105
 *   https://atcoder.jp/contests/abc255/submissions/32442529  snuke氏の実装
 *   https://x.com/kyopro_friends/status/1535619770992979970
 * 
 * 各種解説の通りで、行きがけ順P, 通りがけ順Iについて
 *   P:(根), (左側の部分木), (右側の部分木)
 *   I:(左側の部分木), (根), (右側の部分木)
 * の順に並んでいる。
 * 左右それぞれの部分木について再帰していけば解ける。
 * 
 * 部分木の根はPの先頭要素を見れば良い。
 * I側でそれがどこにあるかは、事前にidx[i]を用意してO(1)でアクセスできるようにしておく。
 * Pにて取得した根の頂点番号が、I側にて見ている範囲外にあるならば、入力に矛盾があると判断できる。
 * 
 * [ACまでの思考の流れ]
 * ・行きがけ順と通りがけ順のグラフをぐっとにらんで、どういう性質を持つかを考える。
 * ・P,Iが前述のように並んでいることが分かったので、部分木ごとに再帰すればよさそう、となった。
 */

void end(void)
{
	cout << -1 << endl;
	exit(0);
}

vector<int> P, I;
vector<int> vl, vr;  // vl[i]:頂点iの左頂点番号  -1なら子無し
vector<int> idx;  // idx[i]:頂点iが通りがけ順にて登場する位置

// 半開区間[l1,r1), [l2,r2) それぞれ行きがけ順、通りがけ順での範囲を表す
// 2つの区間長さは同じ
// 指定された半開区間における部分木の根の番号を返す 空区間であれば-1を返す
int dfs(int l1, int r1, int l2, int r2)
{
	int top = P[l1];  // 今見ている部分木の根の番号
	int pos = idx[top];  // 通りがけ順にて登場する位置

	if(r1-l1 == 0) return -1;  // ベースケース 空区間

	// 通りがけ順にて、今見ている範囲[l2,r2)の外にあればNGで終了してしまう
	if(pos < l2 || pos >= r2) end();

	int sz;
	// 左側で再帰
	sz = pos-l2;
	vl[top] = dfs(l1+1, l1+1+sz, l2, l2+sz);

	// 右側
	sz = r2-(pos+1);
	vr[top] = dfs(r1-sz, r1, r2-sz, r2);

	return top;
}

int main(void)
{
	// 0-indexed
	int i;
	int N; cin >> N;
	P.resize(N);
	I.resize(N);
	idx.resize(N);
	for(i = 0; i < N; i++) {cin >> P[i]; P[i]--;}
	for(i = 0; i < N; i++)
	{
		cin >> I[i]; I[i]--;
		idx[I[i]] = i;
	}
	if(P[0] != 0) end();  // 先頭が頂点1でなければ、終了してしまう
	vl.resize(N, -1);
	vr.resize(N, -1);

	dfs(0, N, 0, N);
	for(i = 0; i < N; i++)
	{
		cout << vl[i]+1 << " " << vr[i]+1 << endl;
	}

	return 0;
}
