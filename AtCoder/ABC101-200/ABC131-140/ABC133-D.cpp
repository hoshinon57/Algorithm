#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC133 https://atcoder.jp/contests/abc133

/*
 * 数学問題。以下と同じ考え方だった。
 *   https://blog.hamayanhamayan.com/entry/2019/07/08/212938
 * 
 * 各山に振った雨の量をm[i]とすると、a[i]=(m[i]+m[i+1])/2となる。
 * Nが奇数であることから、
 *   a1-a2+a3-a4+ ... +aN
 * を計算すると打ち消しあってm1と等しくなる。
 * 
 * m2以降の上記式を1ずつずらして計算できるが、計算量がO(N^2)になってしまうためNG.
 *   a[i]=(m[i]+m[i+1])/2 より
 *   m[i+1]=2*a[i]-m[i]
 * となるので、O(1)で1つずつ求めていけばよい。
 * 
 * [ACまでの思考の流れ]
 * ・Nが奇数という制約が気になる。
 * ・N=3,5辺りで「どうやったら求められるか」を紙でごちゃごちゃ書くと、上記式が出てきた。
 * ⇒気になった制約を元に手で色々と試行錯誤してみる。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> m(N);
	// a1-a2+a3-a4+ ... +aNがm1になる
	for(i = 0; i < N; i++)
	{
		if(i%2==0) m[0] += a[i];
		else m[0] -= a[i];
	}
	// 以降のm[i]を求める
	for(i = 1; i < N; i++)
	{
		m[i] = a[i-1]*2 - m[i-1];
	}

	for(auto &e : m) { cout << e << " "; }
	cout << endl;

	return 0;
}
