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

// ABC388 https://atcoder.jp/contests/abc388

/*
 * いもす法をベースに解く。
 * (コンテスト時は苦戦して遅延セグメント木を持ち出してしまった)
 * 
 * 基本は公式解説の通り。
 * i番目の宇宙人が成人したら、その時点で「以降、だれに石を配るか」が確定する。
 * ここをいもす法で高速化する。
 * 
 * 処理の流れは以下の通り。先頭の宇宙人から順に以下を行う。
 * ・a[i]をいもすテーブルで更新する。
 * ・x=min(a[i], (以降成人する宇宙人の数)) とすると、
 *   xは人iが配る石の数となる。
 *   ★配り先は i+1～i+1+(x-1) の人となる。
 * ・a[i]-=x これで人iの石は確定。
 * ・いもすテーブルにて、 [i+1]を+1, [i+1+x]を-1する。
 *   これは [i+1,i+x] の区間を+1することに相当する。
 * ・いもすテーブルにて、 imos[i+1]+=imos[i] する。
 *   いもす法のシミュレートのフェーズを、人iごとに行っていくことになる。 ★ここが一般的ないもす法と異なる部分
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・区間に対しての加算はいもす法を考えてみる。(他に遅延セグメント木やBITもあるが)
 * ・本問については、「成人した人が各人からもらう」ではなく、「成人した人が、今後誰に渡すことになるか」で考えると考察しやすい。
 *   問題文の操作を別観点から考える形。
 */

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	vector<ll> imos(N+5);
	for(i = 0; i < N; i++)
	{
		a[i] += imos[i];
		ll x = min(a[i], N-i-1);  // 残り人数とのmin この数だけ配れる
		a[i] -= x;

		// [i+1,i+x] の区間を+1
		imos[i+1]++;
		imos[i+1+x]--;

		imos[i+1] += imos[i];
	}

	int sz_ = (int)a.size();
	for(i = 0; i < sz_; i++) {
		cout << a[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}
