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

// ABC112 https://atcoder.jp/contests/abc112

/*
 * Mを約数列挙しておく。
 * M=a*bにできるとすると、b>=Nなら最大公約数をaにできる。
 * https://blog.hamayanhamayan.com/entry/2018/10/06/233701
 * https://drken1215.hatenablog.com/entry/2018/11/06/120600
**/

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
	ll N, M; cin >> N >> M;
	vector<ll> divi;
	func_divisor(M, divi);
	reverse(divi.begin(), divi.end());  // 降順ソートにしておく

	for(auto &e : divi)  // 答の候補e 約数の個数は少ない(典型)ので全探索
	{
		if(M/e >= N)
		{
			cout << e << endl;
			break;
		}
	}

	return 0;
}
