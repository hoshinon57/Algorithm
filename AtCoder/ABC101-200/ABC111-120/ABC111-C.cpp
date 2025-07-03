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

// ABC111 https://atcoder.jp/contests/abc111

/*
 * 実装が面倒だったやつ。各種解説と同じ方針。
 *   公式解説
 *   https://drken1215.hatenablog.com/entry/2019/12/17/130900
 *   https://blog.hamayanhamayan.com/entry/2018/09/30/002021
 * 
 * 基本は
 *   偶数番目の要素について、最頻値を残して、他はその値に合わせる
 *   奇数番目について同様
 * で良い。
 * 
 * しかしコーナーケースが厄介。サンプル3にあるのが親切。
 * ＞数列に現れる数はちょうど2種類
 * より、偶数/奇数番目に残す値は異なる必要があるので、
 * 最頻値が同じなら2番目に頻出するものを使う必要がある。
 * これの実装が面倒だった。
**/

// [verify]ABC111-C
// a[]における値の登場回数をカウントし、{回数,値}で降順のヒストグラムを作成する
// T:a[]の型。intで十分だが、面倒ならllで良い
// li:作成したヒストグラム
// 制約：0<=a[i]<=MX
// aの要素数をNとして O(MX*log(MX)+N) とかになるので、MXの値には注意 大きすぎる場合は座標圧縮を検討する
template <typename T>
void hist(vector<T> &a, vector<pair<ll,T>> &li, ll MX)
{
	vector<ll> cnt(MX+1); for(auto &e : a) {cnt[e]++;}
	li.resize(MX+1);
	for(ll i = 0; i <= MX; i++) {
		li[i] = {cnt[i], i};
	}
	sort(li.begin(), li.end());
	reverse(li.begin(), li.end());
}

int main(void)
{
	int i;
	ll N; cin >> N;
	vector<ll> vo, ve;
	for(i = 0; i < N; i++) {
		ll v; cin >> v;
		if(i%2 == 0) vo.push_back(v);
		else ve.push_back(v);
	}

	using pll = pair<ll,ll>;
	vector<pll> odd, even;
	hist(vo, odd, 100000);
	hist(ve, even, 100000);

	ll t;  // 残す数
	if(odd[0].second != even[0].second)
	{
		t = odd[0].first + even[0].first;
	}
	else
	{
		// 「2番目に頻出の値」を偶数/奇数番目どちらの要素で使うかを両方試す
		t = odd[0].first + even[1].first;
		chmax(t, odd[1].first + even[0].first);
	}
	cout << N-t << endl;

	return 0;
}
