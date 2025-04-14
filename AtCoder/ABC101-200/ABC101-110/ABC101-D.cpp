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

// ABC101 https://atcoder.jp/contests/abc101

/*
 * 自力で解けず、解説を見た。
 * 実験で範囲を狭めてから精査していく。
 *   https://drken1215.hatenablog.com/entry/2018/06/24/010600
 *   https://ikatakos.com/pot/programming_algorithm/contest_history/atcoder/2018/0623_arc099
 * 
 * まず、10^15よりもずっと小さな数で実験してみると、
 * すぬけ数は末尾に9が連続するっぽい、ということが分かる。
 * 例えばn=198として、
 *   198/19 と 199/20
 * を比較すると、198->199の倍率より19->20の倍率の方が大きいので、198/19 > 199/20であり、198はすぬけ数ではない。
 * 
 * 一方でn=1499999(桁和は50)を考えると、
 *   1499999/50 と 1599999/51
 * との比較で、1499999->1599999の倍率が十分に大きくなってしまい、 1499999/50 < 1599999/51.
 * 1499999はすぬけ数である。
 * 
 * 実験すると、どうも nnn99999..9 がすぬけ数の候補っぽいが、
 * nnnをどこまで(最大999)見ればよいのか、またnnnの3桁で良いのかが分からない。
 * nnnの部分をpとし、S(p)=bとすると、
 *   (p*10^n-1)/b > ((p+1)*10^n-1)/(b+1)
 * のとき、p*10^n-1 はすぬけ数ではない。よってpを概算でも求めたい。
 * 整理していくと
 *   p*10^n-1 >b*10^n
 * であり、おおよそ
 *   p>b
 * となる。
 * bの最大値は15*9=135なので、pは140ぐらいまで見れば良さそう、となる。
 * 
 * 後はpを1～140まで、それに対し末尾が9になるような値を全列挙し、n/S(n)が小さい順にソートする。
 * 先頭から見ていき、今見ているnに対し、それまでnより大きいものが存在すれば、nはすぬけ数ではない。
 * (この辺りは実装を参照)
**/

// 桁和を返す
ll s(ll n)
{
	ll ret = 0;
	while(n > 0)
	{
		ret += n%10;
		n /= 10;
	}
	return ret;
}

struct dat{
	ll n;
	ll sn;  // 桁和
};

bool comp(const dat &a, const dat &b)
{
	ll tl = a.n * b.sn;
	ll tr = b.n * a.sn;
	if(tl == tr) { return a.n < b.n; }
	else { return tl < tr; }
}

// 実験
void prac(void)
{
	ll i;
	const ll MX = 2000000;
	vector<dat> snk;
	for(i = 1; i < MX; i++)
	{
		snk.push_back({i, s(i)});
	}
	sort(snk.begin(), snk.end(), comp);
	vector<ll> ans;
	ll mx_n = -1;
	for(auto &e : snk)
	{
		if(e.n < mx_n) continue;
		ans.push_back(e.n);
		chmax(mx_n, e.n);
	}
	return;
}

int main(void)
{
	// prac();
	ll K; cin >> K;
	vector<ll> n_kouho;
	ll i, p;
	for(p = 1; p <= 140; p++)
	{
		ll base = 1;
		while(true)
		{
			ll tmp = p*base-1;
			base *= 10;
			if(tmp == 0) continue;
			if(tmp >= 1000000000000000) break;  // 1e15
			n_kouho.push_back(tmp);
		}
	}
	// 元データをソートし、重複を削除する
	sort(n_kouho.begin(), n_kouho.end());
	n_kouho.erase(unique(n_kouho.begin(), n_kouho.end()), n_kouho.end());

	// 候補を元に {n,S(n)} をsnk[]に登録し、 n/S(n) の小さい順にソート
	vector<dat> snk;
	for(auto &e : n_kouho)
	{
		snk.push_back({e, s(e)});
	}
	sort(snk.begin(), snk.end(), comp);
	vector<ll> ans;

	// 先頭から見ていき、nの最大値MX_Nを保持しておく
	// nを見ているときにn<MX_Nなら、n以降でn/S(n)より小さいものが存在するので、NG
	ll mx_n = -1;
	for(auto &e : snk)
	{
		if(e.n < mx_n) continue;
		ans.push_back(e.n);
		chmax(mx_n, e.n);
	}

	for(i = 0; i < K; i++)
	{
		cout << ans[i] << endl;
	}

	return 0;
}
