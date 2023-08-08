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

// ABC192 https://atcoder.jp/contests/abc192

/*
 * 二分探索で解く。
 * 
 * まず自分がハマったコーナーケースとして、Xが1桁のケースがある。
 * このとき、何進法など関係なく、みなせる値は1種類である。
 * これがM以下か否かで、答が 1 or 0 となる。まずこれを処理してしまう。
 * 
 * 以降はXが2桁以降のケース。
 * これは何進法にするかによって、みなせる値が変化していく。
 * 
 * n進法表記とするn'を増やしていったときに、
 *   n'進のとき、X<=M
 *   (n'+1)進のとき、X>M
 * となるn'が存在する。これを二分探索で求める。
 * このときXを10進に変換するのはオーバーフロー判定が大変そうだったので、M(10進)をn'進に変換することとした。
 * X,Mをn'進法で揃えたら、まずは桁数の大小で、桁数が同じなら上位の桁から見ていくことで、大小関係を判定できる。
 */

int main(void)
{
	int i;
	string X; cin >> X;  // [0]が最上位の桁
	ll M; cin >> M;

	// Xが1桁ならば、何進数など関係なく値は1種類
	if(X.size() == 1)
	{
		int ans = 0;
		if((int)(X[0]-'0') <= M) ans = 1;  // M以下なら1種類、そうでなければ0種類
		cout << ans << endl;
		return 0;
	}

	int d = 0;
	for(auto &e : X)
	{
		d = max(d, (int)(e-'0'));
	}

	ll l = d;
	ll r = INF64;
	while(r - l > 1)
	{
		// Xをn進法とみなし、Mをn進法に変換したときに、X<=Mであればtrue
		auto check = [&](ll n) -> bool
		{
			vector<ll> dig;  // Mをn進法に変換したときに、各桁の値を入れていく
			ll M_bk = M;
			while(M > 0)
			{
				dig.push_back(M % n);
				M /= n;
			}
			M = M_bk;
			reverse(dig.begin(), dig.end());  // dig[0]を最上位の桁とする

			// 桁数が異なればそれで大小は判別できる
			if(X.size() > dig.size()) return false;
			if(X.size() < dig.size()) return true;

			// 桁数が同じ場合は、上の桁から順に大小判定
			for(i = 0; i < (int)X.size(); i++)
			{
				ll nX = (ll)(X[i] - '0');
				ll nM = dig[i];
				if(nX != nM) return (nX<=nM);
			}
			// X=Mの場合はtrue
			return true;
		};
		ll mid = (l+r)/2;
		if(check(mid)) l = mid;
		else r = mid;
	}
	cout << l - d << endl;

	return 0;
}
