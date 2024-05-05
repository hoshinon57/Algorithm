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

// ABC352 https://atcoder.jp/contests/abc352

/*
 * 問題文の理解に時間がかかったが、N=10,K=5であれば
 *   Pの中で1,2,3,4,5が登場する位置を求め、その最右-最左を計算(これがiK-i1)
 *   Pの中で2,3,4,5,6について同上
 *   Pの中で3,4,5,6,7について同上
 *   …
 *   Pの中で6,7,8,9,10について同上
 * とし、その最小値を求めるという内容。
 * 
 * まず"1,2,3,4,5"の登場する位置が分かれば、iK-i1は求められる。
 * ここから"2,3,4,5,6"へは「要素を1つ消し、要素を1つ足す」という操作なので、O(1)相当で処理できる。
 * ⇒集合で管理するのでO(logK).
 * 
 * [ACまでの思考の流れ]
 * ・チェックする対象をi個目⇒i+1個目に変えるときにほとんど変化しないならば、差分に着目してO(1)で解く。[典型]
 */

int main(void)
{
	// 1-indexed
	int i;
	int N, K; cin >> N >> K;
	vector<int> p(N+1);
	vector<int> pos(N+1);  // pos[i]:値iの位置
	for(i = 1; i <= N; i++)
	{
		cin >> p[i];
		pos[p[i]] = i;
	}

	set<int> s;  // 値i～(i+K-1)の計K個の位置
	for(i = 1; i <= K; i++) 
	{
		s.insert(pos[i]);
	}

	ll ans = *s.rbegin() - *s.begin();
	for(i = 2; i <= N-K+1; i++)
	{
		// i-1を消し、i+K-1を加える
		s.erase(pos[i-1]);
		s.insert(pos[i+K-1]);
		ll tmp = *s.rbegin() - *s.begin();
		chmin(ans, tmp);
	}
	cout << ans << endl;

	return 0;
}
