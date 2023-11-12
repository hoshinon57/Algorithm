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

// ABC171 https://atcoder.jp/contests/abc171

/*
 * けんちょん氏やhamayanhamayan氏の解説と同じ。
 *   https://drken1215.hatenablog.com/entry/2020/06/22/122500
 *   https://blog.hamayanhamayan.com/entry/2020/06/23/193239
 * 
 * 全a[i]についてxorを取ると、「自分の数」を全てxorしたものになる。これをSとする。
 * Sから自分の数を求めるには、S^a[i]で求まる。
 * 
 * [ACまでの思考の流れ]
 * ・とりあえずN=4のときにどう求めるかを考える。
 * ・こういうのは全要素(a[i])についてxorしてみたい。
 *   やってみると「自分の数」を全てxorしたものになった。 ⇒ これをSとする
 * ・ならS^a[i]がi番目に求めたい数となりそう。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	int S = 0;  // 全aのxor
	for(auto &e : a) { S^=e; }
	for(auto &e : a) { cout << (S^e) << " "; }
	cout << endl;
	
	return 0;
}
