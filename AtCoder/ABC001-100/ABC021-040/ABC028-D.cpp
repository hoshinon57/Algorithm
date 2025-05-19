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

// ABC028 https://atcoder.jp/contests/abc028

/*
 * 以下の4パターンに分けられる。
 * 
 * 1)(1～K-1), K, (K+1～N)
 *   (K-1)*(N-K)通りで、並べ方で6通り。
 * 2)K, K, (K+1～N)
 *   (N-K)通りで、並べ方で3通り。
 * 3)(1～K-1), K, K
 *   (2)と同様。
 * 4)K, K, K
 *   1通り。
 * 
 * (1)～(4)の総和をN^3で割ったものが答。
**/

int main(void)
{
	ll N, K; cin >> N >> K;

	ll le = K-1;
	ll gr = N-K;
	ll cnt = 0;
	cnt += le*gr*6;
	cnt += le*3 + gr*3;
	cnt++;

	cout << std::fixed << std::setprecision(20);
	cout << (double)cnt/N/N/N << endl;

	return 0;
}
