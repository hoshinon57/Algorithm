#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題97より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc150/tasks/abc150_d

/*
 * まず X=a*(p+0.5) を書き換えたい。
 * aは偶数であることから、 a=2*b と置くと X=b*(2p+1) となる。
 * 各a(ないしb)ごとにpの値は異なることから、p(i)とすると(便乗してb(i)とすると)
 *   X = b(1)*(2p(1)+1) = b(2)*(2p(2)+1) = ... = b(N)*(2p(N)+1)
 * となる。
 * pは整数であることから、2p(N)+1は奇数。
 * つまり各bに対して何らか奇数倍したものが半公倍数になる。
 * 
 * よって、、、
 * (1)
 * 各bごとの最小公倍数を求めて、それをsemi_lcmとする。
 * ただしこの時点では、これが半公倍数になるかはまだ分からない。
 * 
 * (2)
 * 次に各bに対して semi_lcm/b が奇数になるか判定する。
 * 全て奇数倍であれば、semi_lcmは最小の半公倍数となる。
 * 偶数倍が1つでもあれば、そもそも半公倍数が存在しない。
 * ★偶数倍に何をかけても、奇数倍にはならないため。
 * 
 * (3)
 * 最小の半公倍数が求まったので、この次に小さい半公倍数を求める。
 * semi_lcm*2は前述の 2p(N)+1 が偶数になるためNG.
 * semi_lcm*3, semi_lcm*5, ... と奇数倍していったものが以降の半公倍数になる。
 * つまり半公倍数の間隔は semi_lcm*2 となる。
 * 
 * よって最後に
 *   (M-semi_lcm) / (semi_lcm*2) + 1
 * でM以下の半公倍数の個数が計算できるので、これを出力すればよい。
 */

ll gcd(ll a, ll b)
{
	if(a%b == 0) return b;
	
	return gcd(b, a%b);
}

// 最小公倍数
ll lcm(ll a, ll b)
{
	return a*b/gcd(a,b);
}

bool is_odd(ll a)
{
	return (a%2==1);
}

int main(void)
{
	int i;
	ll N, M;
	cin >> N >> M;
	vector<ll> a(N);
	vector<ll> half_a(N);  // 入力Aの半分の値(Aは偶数)
	ll semi_lcm = 1;  // 半公倍数の最小値、いわゆる最小"半"公倍数
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		half_a[i] = a[i]/2;

		// Aの半分の値に対して、最小公倍数を求める。
		// 半公倍数が存在する場合、このsemi_lcmが最小"半"公倍数となる。
		semi_lcm = lcm(semi_lcm, half_a[i]);
		
		// M以下の個数を求めるので、Mを超えたら0出力
		// (このまま計算し続けると、long longをオーバーフローする(1WA))
		if(semi_lcm > M)
		{
			cout << 0 << endl;
			return 0;
		}
	}

	// half_a[]の各値eに対して、最小"半"公倍数/e は奇数でなくてはならない
	for(i = 0; i < N; i++)
	{
		if(is_odd(semi_lcm / half_a[i]))
		{
			;  // 奇数倍ならOK
		}
		else
		{
			// 偶数倍ならNG 半公倍数は存在しない
			cout << 0 << endl;
			return 0;
		}
	}
	
	// 最小"半"公倍数 semi_lcm に対して、
	// 以降の半公倍数は semi_lcm*3, semi_lcm*5, semi_lcm*7, ... とsemi_lcm*2増加ずつとなる。 
	// また、途中の if(semi_lcm > M) により、この時点で semi_lcm <= M が保証される。
	cout << (M-semi_lcm) / (semi_lcm*2) + 1 << endl;

	return 0;
}
