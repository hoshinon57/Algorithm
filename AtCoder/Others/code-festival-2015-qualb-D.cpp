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

// https://atcoder.jp/contests/code-festival-2015-qualb/tasks/codefestival_2015_qualB_d

/*
 * 「区間をsetで管理」で解く。
 * 
 * 黒マスの区間を管理する。
 * またクエリs,cについて、cを「残り何マス塗れるか」の残カウントとして扱う。
 * 
 * c>0の間、以下を行う。
 *   まずはsを白マスまで移動させる。
 *   その後、右にある黒マスを探す。
 *   1)右に黒マスがある場合
 *     sが今いる区間の白マスの個数か残カウントのうち、少ない方だけ塗れる
 *     塗りつつ、merge()にて前後の隣接した黒マス区間を統合しておく。
 *   2)右に黒マスが無い場合
 *     いくらでも塗れるので、残カウント分だけ塗れる。
 * c=0になったときのsが答。
 */

/*
 * 区間をsetで管理する構造体 閉区間で表すことに注意
 * llとINF64を定義しておくこと
 * メインは(1)(2)で、問題によっては(3)以降を使う感じか
 * またはset構造体のsを直接操作する
 * [verify]ABC364-F
 * 
 * コンストラクタ : interval_set s; or interval_set s(l,r);  後者はl<=i<=rを満たす[i,i]を区間登録
 * (1)get(pos) : 値posが含まれる区間のイテレータを返す。該当区間がなければs.end()を返す。
 * (2)ins(l,r) : 閉区間[l,r]を区間に追加する。その際に重なる区間は統合される。戻り値は区間数。
 * (3)same(x,y) : x,yが同一区間か判定
 * (4)merge(pos) : 位置posを指定して、隣接した区間が両隣にあれば統合する
 *    例){1,3},{4,6},{7,9}でmerge(4);で呼び出すと{1,9}になる
 *    マス目を埋めていくような問題で使えるかも
 * (5-1)get2_next(pos) : 値posが区間に含まれていればそこを、無ければ次の区間のイテレータを返す
 * (5-2)get2_prev(pos) : 同じく手前の区間のイテレータを返す 手前が無ければ該当無しとしてendを返す
 */
struct interval_set {
	using pll = pair<ll, ll>;
	set<pll> s;
	interval_set() {}
	interval_set(ll st, ll ed) { init(st, ed); }
	// チェック無しでst<=i<=edを満たす[i,i]を区間登録
	void init(ll st, ll ed) {
		for(ll i = st; i <= ed; i++) {
			s.insert({i, i});
		}
	}
	// 値posが含まれる区間のイテレータを返す
	// 該当区間がなければs.end()を返す
	set<pll>::iterator get(ll pos) {
		if(s.size() == 0) return s.end();
		// upper_boundの1つ手前をチェック
		auto itr = s.upper_bound({pos, INF64});
		if(itr == s.begin()) return s.end();
		itr--;
		if(itr->first <= pos && pos <= itr->second) return itr;  // left<=pos<=right
		else return s.end();
	}
	// 値posが区間に含まれていればそこを、無ければ次の区間のイテレータを返す
	set<pll>::iterator get2_next(ll pos) {
		if(s.size() == 0) return s.end();
		auto itr = get(pos);
		if(itr != s.end()) return itr;  // 区間があればそこを返す
		else return s.upper_bound({pos, INF64});  // 無い場合、次のイテレータ
	}
	// get2_nextと同じく手前の区間のイテレータを返す 手前が無ければ該当無しとしてendを返す
	set<pll>::iterator get2_prev(ll pos) {
		if(s.size() == 0) return s.end();
		auto itr = get(pos);
		if(itr != s.end()) return itr;  // 区間があればそこを返す
		// 無い場合は手前のイテレータ ただしbeginなら該当無しとしてendを返す
		if( (itr = s.upper_bound({pos, INF64})) == s.begin()) return s.end();
		else return prev(itr);
	}
	// 閉区間[l,r]を区間に追加し、その際に重なる区間は統合する
	size_t ins(ll l, ll r) {
		ll st, ed;  // [l,r]を囲む区間[st,ed]
		auto itr = get(l);
		if(itr == s.end()) {  // lが区間に存在しない
			st = l;
			s.insert({st, st});  // 後述の実装を簡易にするため区間登録しておく
		}
		else st = itr->first;

		if( (itr = get(r)) == s.end()) ed = r;
		else ed = itr->second;

		// 先に重なる区間を消してから、統合された区間を登録
		itr = get(st);
		while(itr != s.end() && itr->first <= ed) {
			itr = s.erase(itr);
		}
		s.insert({st, ed});
		return s.size();
	}
	// x,yが同一区間か判定
	bool same(ll x, ll y) {
		auto itr = get(x);
		if(itr == s.end()) return false;  // xが区間に含まれていないならfalse
		return (itr->first <= y && y <= itr->second);  // yがxの区間内か
	}
	// 位置posを指定して、隣接した区間が両隣にあれば統合する
	// 例：{1,3},{4,6},{7,9} で merge(4); で呼び出せば {1,9} となる
	// マス目を埋めていくような問題で使えるかも
	// posが区間に含まれないときの動作は未定義
	void merge(ll pos) {
		auto itr = get(pos);
		auto nxt = next(itr);
		if(nxt != s.end()) {
			if(itr->second+1 == nxt->first) ins(itr->second, nxt->first);
		}
		itr = get(pos);
		if(itr != s.begin()) {
			auto prv = prev(itr);
			if(prv->second+1 == itr->first) ins(prv->second, itr->first);
		}
	}
	void debug(void) {
		// return;
		for(auto &e : s) {
			cout << "[" << e.first << "," << e.second << "], ";
		}
		cout << endl;
	}
};

int main(void)
{
	ll i;
	ll N; cin >> N;
	interval_set is;  // 黒マスを区間で管理
	for(i = 0; i < N; i++)
	{
		ll s, c; cin >> s >> c;
		while(c > 0)
		{
			// まずはsを白マスまで移動させる
			auto itr = is.get(s);
			if(itr != is.s.end()) s = itr->second+1;  // 区間の次マスへ

			itr = is.get2_next(s);  // 右にある黒マスを探す
			if(itr != is.s.end())  // 右に黒マス有り
			{
				ll tmp = min(itr->first-s, c);  // 白マスの個数か残カウントのうち、少ない方だけ塗れる
				is.ins(s, s+tmp-1);
				is.merge(s);  // 前後の隣接した黒マス区間を統合
				s = s+tmp-1;
				c -= tmp;
			}
			else  // 右に黒マス無し
			{
				is.ins(s, s+c-1);
				is.merge(s);
				s = s+c-1;
				c = 0;
			}
		}
		cout << s << endl;
	}

	return 0;
}
