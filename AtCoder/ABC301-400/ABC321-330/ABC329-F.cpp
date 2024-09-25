#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC329 https://atcoder.jp/contests/abc329

/*
 * マージテクで解く。各種解説の通り。
 *   https://x.com/kyopro_friends/status/1725877090753630291
 *   https://atcoder.jp/contests/abc329/editorial/7729
 * 
 * 箱ごとに「入っているボールの種類」をsetで持たせる。
 * ※同じ色のボールが何個入っていても出力には影響がないため、multisetではなくsetで良い。
 * 
 * クエリごとに単純にsetの中身をa->bへ移せばよい。
 * a.size()>b.size()のとき、事前にa,bをswap,つまりaの方がボールの種類が少なくなるようにしておくことで、
 * ボールごとの移動回数が最大でもlogN回となり、全体でO(NlogN)となり間に合う。
 * 
 * [ACまでの思考の流れ]
 * ・箱に入ったボールは分離しないので、setとかunionfindかなと。
 * ・データ構造のマージは「マージテク」で計算量を減らすことを考える。 [典型]
 */

// マージテク(※)を用いて、from->toへ要素をマージする
// (※)サイズが小さい方から大きい方へマージするようにすることで、クエリ全体でO(NlogN)に抑えられる
// [注意]fromは空にはしないが、swapにより中身が変化する可能性があるため使わない方が良い
template <typename T>
void set_merge(set<T> &from, set<T> &to)
{
	if(from.size() > to.size()) swap(from, to);
	for(auto &e : from) to.insert(e);
}

int main(void)
{
	// 0-indexed
	int i;
	int N, Q; cin >> N >> Q;
	vector<set<int>> box(N);  // box[i]:i番目の箱にあるボール(色のみ管理)
	for(i = 0; i < N; i++)
	{
		int c; cin >> c;
		box[i].insert(c);
	}

	while(Q > 0)
	{
		Q--;
		int a, b; cin >> a >> b;  // a->b
		a--; b--;
		set_merge(box[a], box[b]);
		box[a].clear();
		cout << box[b].size() << endl;
	}

	return 0;
}
