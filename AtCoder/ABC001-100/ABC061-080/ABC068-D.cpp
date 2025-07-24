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

// ABC068 https://atcoder.jp/contests/abc068

/*
 * 構築系問題では「最大ケース」で考えるとやりやすいイメージがある。
 * 今回であればNは最大の50として構築していく。
 * 
 * とはいえ最初は検討のため、N=4ぐらいでスタートした。
 * また、各a[i]に1回ずつ操作すると、a[i]はそれぞれ1減るという性質がある。
 *   https://blog.hamayanhamayan.com/entry/2017/07/30/030601
 * 
 * 試行錯誤してみると、
 * K=1のとき {4,2,1,0}
 * K=2 {4,3,1,0}
 * K=3 {4,3,2,0}
 * K=4 {4,3,2,1}
 * K=5 {5,3,2,1}
 * となることから、
 *   ベースは{3,2,1,0}
 *   K/4の値だけ、全体に足す
 *   K%4の値だけ、先頭から+1する
 * ように構築すればうまくいくっぽい、となった。
**/

int main(void)
{
	ll i;
	ll K; cin >> K;
	ll N = 50;  // 最大値固定
	vector<ll> a(N);
	for(i = 0; i < N; i++) a[i] = N-i-1;

	// K/Nを全体に足す
	for(i = 0; i < N; i++) a[i] += K/N;

	// 先頭からK%N個だけ+1
	for(i = 0; i < K%N; i++) a[i]++;

	cout << N << endl;
	for(i = 0; i < N; i++)
	{
		cout << a[i];
		if(i != N-1) cout << " ";
	}
	cout << endl;

	return 0;
}
