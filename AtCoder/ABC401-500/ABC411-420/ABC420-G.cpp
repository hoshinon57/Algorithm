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

// ABC420 https://atcoder.jp/contests/abc420

/*
 * 数学問題。各種解説の通り。
 *   https://atcoder.jp/contests/abc420/editorial/13755
 *   https://atcoder.jp/contests/abc420/editorial/13733
 *   https://www.youtube.com/watch?v=5KHjGZysb68
 * 
 * kyopro_friends氏解説の以下が目安として分かりやすい。
 * ＞一般に、この手の問題は項の数が少なかったり、変数の次数が揃っていた方が嬉しくなります。
 * ＞そこで「平方完成」をすることで1次の項を消すことを考えます。
 * 
 * √(n^2+n+X) が整数なので、これをmとおくと、
 * n^2 +n +X = m^2
 * (n+1/2)^2 -1/4 + X = m^2
 * (2n+1)^2 - 4m^2 = 1-4X
 * (2n-2m+1)*(2n+2m+1) = 1-4X
 * 1-4Xの約数αに対し、β=(1/4X)/αとすると、
 *   2n-2m+1=α
 *   2n+2m+1=β
 * よりn=(α+β-2)/4 (ただしnは整数であること)
 * となる。
 * 
 * Xは±10^14なので、約数の列挙は可能。
 * 1-4Xは正負どちらもありえるので、約数を求めるライブラリにはabsをつけて渡す。
 * またαも正負どちらもありえるので、求めた約数に対して正負それぞれチェックする。
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
	ll i;
	ll X; cin >> X;
	X = 1-4*X;
	vector<ll> div;
	func_divisor(abs(X), div);
	vector<ll> ans;
	for(auto e : div)  // e:約数
	{
		for(int _ = 0; _ < 2; _++, e=-e)  // 約数の正負変えて2回
		{
			ll a = e;
			ll b = X/a;
			if((a+b-2)%4 != 0) continue;
			ans.push_back((a+b-2)/4);
		}
	}

	// 元データをソートし、重複を削除する
	sort(ans.begin(), ans.end());
	ans.erase(unique(ans.begin(), ans.end()), ans.end());  // unique()で隣り合う重複を削除し、erase()で末端までのゴミを削除する
	int sz_ = (int)ans.size();
	cout << sz_ << endl;
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}
