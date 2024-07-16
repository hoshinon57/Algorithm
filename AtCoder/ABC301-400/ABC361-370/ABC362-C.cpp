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

// ABC362 https://atcoder.jp/contests/abc362

/*
 * 「最小値と最大値の間は全て作れる」的なやつ。
 *   ex:ABC169-E(Count Median) https://atcoder.jp/contests/abc169/tasks/abc169_e
 * 
 * (Lの総和)<=0 かつ (Rの総和)>=0 であれば、必ず総和を0にすることができる。
 * 全要素が最小値であるLの状態から始め、各要素について
 *   Rより大きくならないように
 *   総和が0を超えないように
 * 調整していく。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> l(N), r(N);
	ll ltot = 0, rtot = 0;
	for(i = 0; i < N; i++)
	{
		cin >> l[i] >> r[i];
		ltot += l[i];
		rtot += r[i];
	}
	if(ltot <= 0 && rtot >= 0)
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
		return 0;
	}

	ll zan = -ltot;  // あとこの値だけ、Lからずらせばよい
	for(i = 0; i < N; i++)
	{
		ll d = min(zan, r[i]-l[i]);  // ずらしたい量とずらせる限界のうち、小さい方
		cout << l[i]+d << endl;
		zan -= d;
	}

	return 0;
}
