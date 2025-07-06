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

// ABC413 https://atcoder.jp/contests/abc413

/*
 * マージソートっぽい感じ。
 * 
 * まず、a*2^b ～ (a+1)*2^b といった形が出たときは、セグメント木の内部での要素の持ち方をイメージする。
 * 区間[l,r)としてlは2^bごとにずれていく感じ。また区間長も2^b.
 * aは単に「何番目の[l,r)か」を指定する程度の立ち位置、ぐらいにイメージする。bが大事。
 * 
 * いろいろ考えると、マージソートっぽく
 *   b=1,つまり区間長2のとき、区間の左側と右側をswap
 *   b=2,つまり区間長4のとき、区間の左側と右側をswap
 *   b=3,つまり区間長8のとき、区間の左側と右側をswap
 *   …
 * となる。
 * ※操作は「位置を反転させる」だが、操作は何回でも実施できるので、b=1,b=2,b=1といった順の操作も可能。
 *   これにより 2 3 4 1 を 1 4 2 3 にすることもできる。柔軟性が結構ある。
 * 
 * 「辞書順最小」なので、区間の左側と右側を比較するときに
 *   左側の先頭要素 > 右側の先頭要素
 * であれば、swapさせた方が良い。
 * 
 * コンテスト時はb=1から順にforで処理していったが、セグ木実装と同じく再帰した方が楽。
 * dfs(l,r)で [l,r)の区間で上手いことやる とすると、再帰でどんどん分割していく感じ。
 * mainから呼び出すときはdfs(0,2^N)として全区間を指定する。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・a*2^b ～ (a+1)*2^b といった形が出たときは、セグメント木の内部での要素の持ち方をイメージする。
 *   区間[l,r)としてlは2^bごとにずれていく感じ。また区間長も2^b.
 *   aは単に「何番目の[l,r)か」を指定する程度の立ち位置、ぐらいにイメージする。bが大事。
 *   またこのとき、[l,r)=[0,2^N)で呼び出し、再帰させる方が実装が楽かも。
**/

void solve(void)
{
	ll i;
	ll N; cin >> N;
	ll pn = 1LL<<N;  // 2^N
	vector<ll> p(pn); for(i = 0; i < pn; i++) {cin >> p[i];}
	
	auto dfs = [&](auto &self, ll l, ll r) -> void
	{
		if(r-l == 1) return;  // ベースケース
		ll m = (l+r)/2;  // [l,m)と[m,r)
		self(self, l, m);
		self(self, m, r);
		if(p[l] > p[m])
		{  // 区間の左側と右側をswap
			for(i = 0; i < m-l; i++)
			{
				swap(p[l+i], p[m+i]);
			}
		}
	};
	dfs(dfs, 0, pn);

	int sz_ = (int)p.size();
	// cout << sz_ << endl;
	for(i = 0; i < sz_; i++) {
		cout << p[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;
}

int main(void)
{
	int T; cin >> T;
	while(T > 0)
	{
		T--;
		solve();
	}

	return 0;
}
