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

// ABC379 https://atcoder.jp/contests/abc379

/*
 * コンテスト時に解けず、解説を見た。
 * ヒストグラム中の最大正方形(長方形)問題みたいなやつと、クエリ先読みで解く。
 * 
 * 基本は公式解説の通り。
 *   https://atcoder.jp/contests/abc379/editorial/11309
 * 
 * ビルiからビルjが見える条件は、Hk>Hjとなるビルkが無いこと。
 * つまりビルiの高さは無関係。
 * よってx1<x2<x3として、ビルx1からビルx3が見えるのであれば、ビルx2からも見える。
 * 
 * クエリを先読みした上で、
 * ビルを右から見ていってスタック内に(広義)単調増加になるよう積んでいく。
 * 今見ているビルiがlとなるようなクエリを列挙して、
 * ビルiから見えるビル一覧からrより大きいビルの個数を二分探索で求めれば良い。
 * ※実装では、二分探索のためvectorを使った。
 *   また単調増加になるよう位置を-1倍して登録している。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・最大長方形問題は見えたが、そこから解法に至らなかった。
 * ・あるビルlに対してrに対する答は求められるので、そこからクエリ先読みを思いつけるようにする。
 */

// ソート済のvector<T>a に対して、(未満,以下,より大きい,以上)となるidxと要素数を返す
// {val未満の最右のidx, val未満の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_miman(vector<T> &a, T val) {ll idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}

int main(void)
{
	// 0-indexed
	ll i;
	ll N, Q; cin >> N >> Q;
	vector<ll> h(N); for(i = 0; i < N; i++) {cin >> h[i];}
	vector<vector<pair<ll,ll>>> que(N);  // que[l] = {{r,クエリ番号}, ...}
	for(i = 0; i < Q; i++)
	{
		ll l, r; cin >> l >> r;
		l--; r--;
		que[l].push_back({r,i});
	}

	vector<ll> ans(Q);
	vector<ll> st_h, st_pos;  // 高さと横方向の位置をそれぞれ
	// スタック内は(広義)単調増加になるよう積んでいく -> vectorなので-1倍して積む
	for(i = N-1; i >= 0; i--)
	{
		// この時点のst:ビルiから見ることができるビル
		for(auto [r,idx] : que[i])
		{
			auto [dmy,num] = bi_idxnum_miman(st_pos, -r);  // -1倍して積んでいるので、(-r)未満はrより大きい要素になる
			ans[idx] = num;
		}

		while(!st_h.empty() && h[i] > st_h.back())
		{
			st_h.pop_back();
			st_pos.pop_back();
		}
		st_h.push_back(h[i]);
		st_pos.push_back(-i);  // -1倍して積む
	}

	for(auto &e : ans) cout << e << endl;

	return 0;
}
