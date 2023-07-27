#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC270 https://atcoder.jp/contests/abc270

/*
 * 二分探索で解く。
 * 
 * x周してかご1に戻ってくるまでに食べたリンゴの数をeat(x)と定義すると、
 *   eat(x-1) < K
 *   eat(x) >= K
 * となるxが存在する。これを二分探索で求める。
 * eat(x)の計算量はO(N)であるので、全体の計算量はO(NlogK)となる。
 * 
 * 上記のxを求めたら、あとはいわゆる手計算で求める。
 * ・x-1周した時点の食べた個数と、各かごのりんごの数を計算
 * ・x周めについて、かご1つ1つ計算
 * を実装する。
 */

// x周してかご1に戻ってきたときに、それまでに食べた数を返す
ll eat_count(ll x, vector<ll> &a)
{
	ll N = a.size();
	ll count = 0;

	for(ll i = 0; i < N; i++)
	{
		// x周してりんごがまだ残っているケースと、りんごを食べきるケース、のどちらか
		count += min(a[i], x);
	}
	return count;
}

int main(void)
{
	// 0-indexed
	ll i;
	ll N, K;
	cin >> N >> K;
	vector<ll> a(N);
	for(i = 0; i < N; i++) cin >> a[i];

	ll l, r;
	l = 0;  // l周してかご1に戻ってきたときに、食べたリンゴの数がK個未満
	r = 1e12+1;  // 同じく、K個以上
	// (rの初期値はちゃんとやるならKだが、サボってKの最大値+1とした)
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		if(eat_count(mid, a) < K)
		{
			l = mid;
		}
		else
		{
			r = mid;
		}
	}

	// r周めの途中で、K個食べることになる
	// まずr-1(=l)周した時点の食べた個数と、各かごのりんごの数を計算する
	ll eat = 0;  // 食べた個数
	for(i = 0; i < N; i++)
	{
		ll tmp = min(a[i], l);  // l周したときに、かごiから食べる個数
		eat += tmp;
		a[i] -= tmp;
	}

	// r周めについて、かご1つ1つ計算していく
	for(i = 0; i < N; i++)
	{
		if(eat == K) break;
		if(a[i] > 0)  // りんごが残っていたら、1つ食べる
		{
			a[i]--;
			eat++;
		}
	}

	// 結果出力
	for(auto &e : a)
	{
		cout << e << " ";
	}
	cout << endl;

	return 0;
}
