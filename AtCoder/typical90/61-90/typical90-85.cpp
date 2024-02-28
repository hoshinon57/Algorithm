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

// 競プロ典型90問:85 https://atcoder.jp/contests/typical90/tasks/typical90_cg

/*
 * 約数列挙と全探索で解く。
 * 
 * a,b,cはいずれもKの約数となる。
 * また約数の個数は少ない。
 * よってKの約数を列挙し、a,bについて全探索して間に合う。
 * 
 * ただしa*bは最大で10^24程度になるため、オーバーフローに注意する必要がある。
 * 自分の実装では乗算を使わない方針としたが、
 * 公式解説では a*b<=K であることから、
 *   a*b>K になったらNG,
 *   b>K/a と同値
 * のチェックをした上でa*bを計算している。
 * 
 * [ACまでの思考の流れ]
 * ・約数の個数は少ない。
 *   10^9以下では最大1344個、10^18以下では最大103680個とのこと。
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
	ll K; cin >> K;
	vector<ll> div;
	func_divisor(K, div);
	ll a, b;
	ll ans = 0;
	for(a = 0; a < (int)div.size(); a++)  // div[a]
	{
		ll aa = div[a];
		for(b = a; b < (int)div.size(); b++)  // div[b]
		{
			ll bb = div[b];
			// a*bで割り切れて、かつc>=bならOK
			if(K/aa%bb==0 && K/aa/bb >= bb) ans++;
		}
	}
	cout << ans << endl;

	return 0;
}
