#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC161 https://atcoder.jp/contests/abc161

/*
 * 約数列挙で考える。
 * (★)N%Kと(N-K)%Kの値は同一となる。
 * 
 * (1)
 * Nの約数e(1を除く)について実際に操作を行っていく。
 * (★)より、操作は「除算を繰り返す」⇒「減算を繰り返す」となるので、
 * eで割れるだけ割ったあと、eで割った余りが1になればOK.
 * 
 * (2)
 * N-1の約数e(1を除く)はいずれも条件を満たす。
 * N%e=1であるため、(★)より減算を繰り返す形になる。
 * 
 * (1)と(2)の和が答。
 * 重複するかを考えたが、NとN-1は互いに素なので重複はしないだろうと判断した。
 * 
 * [ACまでの思考の流れ]
 * ・いくつかNを考えたときに、どういうKが条件を満たすかを考えていった。
 * ・どうもN-1の約数は常に条件を満たしそう。
 * ・Nの約数は条件を満たす/満たさないがケースバイケースっぽい。
 *   約数の個数は多くないことと、操作は除算繰り返し⇒減算繰り返しなので計算量は少ないと判断。
 *   愚直にやって問題なさそう。
 */

// Nの約数を列挙してdivisor[]に入れる
// 参考：https://qiita.com/drken/items/a14e9af0ca2d857dad23
void func_divisor(long long N, vector<long long> &divisor)
{
	for(long long i = 1; i*i <= N; i++)
	{
		if( N%i == 0 )  // iで割り切れるなら約数
		{
			divisor.push_back(i);
			if( i*i != N )
			{
				// N=16, j=2のとき、8も約数になる。それを追加する。
				// (これを行うことで、約数列挙の計算量がO(N)からO(√N)になる)
				// ただしj*j=N(例：4*4=16)といった場合を
				// 追加してしまうと重複してしまうので、それは省く。
				divisor.push_back(N/i);
			}
		}
	}

	// ソート有無は必要に応じて
	sort(divisor.begin(), divisor.end());

	return;
}

int main(void)
{
	ll N; cin >> N;
	ll ans = 0;

	// Nの約数について探す
	vector<ll> divisor;
	func_divisor(N, divisor);
	for(auto &e : divisor)
	{
		if(e == 1) continue;  // 1は除く
		ll NN = N;
		// 約数eで割れるだけ割った後、MOD e が1ならOK
		while(NN%e == 0) NN/=e;
		if(NN%e == 1) ans++;
	}

	// N-1の約数について探す
	divisor.clear();
	func_divisor(N-1, divisor);
	ans += (int)divisor.size() - 1;  // 1は除く

	cout << ans << endl;

	return 0;
}
