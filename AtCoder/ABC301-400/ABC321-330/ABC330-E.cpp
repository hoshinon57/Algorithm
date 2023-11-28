#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC330 https://atcoder.jp/contests/abc330

/*
 * キーワード：要素数がN個のとき、MEXは最大でもNまで。N+1以上は無視してよい。
 * 例：N=5のとき、要素={0,1,2,3,4}のときのMEX=5が最大。
 *     MEX=6になることはあり得ない。
 * 
 * ・cnt[i]:iが数列Aに登場する個数
 * ・non:数列Aに存在しない値の集合＝MEX候補
 * を用意する。
 * nonのうち最小の値がMEXとなる。
 * また初期状態は愚直にnonを求めておく。
 * 
 * 各クエリについて、cnt[i]を更新したときに
 *   1->0 になればnonに追加
 *   0->1 になればnonから削除
 * することで、nonに含まれる最小値が答になり続ける。
 */

int main(void)
{
	ll i;
	ll N, Q; cin >> N >> Q;
	set<ll> non;  // 数列Aに存在しない値のリスト＝MEX候補
	map<ll,ll> cnt;  // cnt[i]:数値iの個数
	vector<ll> a(N);
	for(i = 0; i <= N; i++)  // 「数列Aに存在しない値のリスト」の初期化
	{
		non.insert(i);
	}
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		if(++cnt[a[i]] == 1)
		{
			if(a[i] <= N) non.erase(a[i]);  // リストから削除
		}
	}

	while(Q > 0)
	{
		ll idx, x; cin >> idx >> x;
		idx--;

		// 値の個数が1->0になればリストに登録し、
		// 0->1になればリストから削除する
		if(--cnt[a[idx]] == 0)
		{
			if(a[idx] <= N) non.insert(a[idx]);
		}
		if(++cnt[x] == 1)
		{
			if(x <= N) non.erase(x);
		}
		a[idx] = x;
		cout << *non.begin() << endl;

		Q--;
	}

	return 0;
}
