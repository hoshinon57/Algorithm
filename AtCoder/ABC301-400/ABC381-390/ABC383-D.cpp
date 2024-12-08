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

// ABC383 https://atcoder.jp/contests/abc383

/*
 * いやーコンテスト時苦戦した。
 * 素数列挙、二分探索などを用いて解く。
 * 
 * 「約数が9個」がどういう値かを考える。
 * ある値Nを素因数分解したとき、N=a1^p1*a2^p2*...*ai^pi とすると、
 * Nの約数の個数は (a1+1)(a2+1)...(ai+1) となる。[数学典型]
 * 
 * 9=3*3または9*1であるため、
 *   (1)a1^2*a2^2
 *   (2)a1^8
 * のどちらかで表せる値が条件を満たす。これを別手段でそれぞれ求める。
 * 
 * (1)
 * a1^2 と2乗しているため、a1<=√N を満たす。
 * よって2*10^6以下にて素数を列挙する。
 * (素数の2乗)を昇順にリストアップしておくと、要素内の2要素e1,e2(e1<e2)にて、e1について全探索する。
 * e1<e2<=(N/e1, の切り捨て) を満たすe2を二分探索で求めて、それを加算していけばよい。
 * 
 * (2)
 * (素数^8)<=N なる値を単純に求めればよい。
 * Nを超えた時点でforループから抜ければ、オーバーフローを気にする必要もない。
 * 
 * (1)について、公式解説にあった尺取り法っぽい実装も試してみた。
 * コードの#if 0側の部分。ただし実行時間は変わらなかった。
 */

// 素数列挙(エラトステネスの篩)
// N以下の素数を列挙してprime_listに入れる
// 計算量はO(NloglogN)とのこと。以下2つの記事を参考。
//   https://algo-method.com/descriptions/64
//   https://manabitimes.jp/math/992
void make_prime_list(long long N, vector<long long> &prime_list)
{
	vector<bool> isPrime(N+1, true);
	isPrime[0] = isPrime[1] = false;
	for(long long i = 2; i <= N; i++)
	{
		if(!isPrime[i]) continue;
		// iは素数
		prime_list.push_back(i);
		// iの倍数を素数から外す
		for(long long j = i+i; j <= N; j += i)
		{
			isPrime[j] = false;
		}
	}
	return;
}

// ソート済のvector<T>a に対して、(未満,以下,より大きい,以上)となるidxと要素数を返す
// {val未満の最右のidx, val未満の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_miman(vector<T> &a, T val) {ll idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}
// {val以下の最右のidx, val以下の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_ika(vector<T> &a, T val)   {ll idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}
// {valより大きい最左のidx, valより大きい要素数} 要素が無ければidx=N
template<typename T> pair<ll,ll> bi_idxnum_koeru(vector<T> &a, T val) {ll idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx,(ll)a.size()-idx};}
// {val以上の最左のidx, val以上の要素数} 要素が無ければidx=N
template<typename T> pair<ll,ll> bi_idxnum_ijou(vector<T> &a, T val)  {ll idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx,(ll)a.size()-idx};}

// Nが素数かどうか判定
bool isPrime(long long N)
{
	if(N < 2) return false;

	// √Nを上限とすれば十分
	for(long long i = 2; i*i <= N; i++)
	{
		if(N%i == 0) return false;
	}

	return true;
}

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// ※modは素数といった制約は無い
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	// a %= mod;  // a*aにてオーバーフローしそうな場合に有効化
	long long ans = 1;
	while(n > 0)
	{
		if((n&1) != 0)  // nの最下位ビットが立っているなら、答に掛ける
			ans = (ans * a) % mod;
		a = (a * a) % mod;
		n >>= 1;
	}
	return ans;
}

int main(void)
{
	ll N; cin >> N;
	ll MX_P = 2*1e6;  // √4*10^12
	vector<ll> pl;
	make_prime_list(MX_P, pl);
	for(auto &e : pl) e=e*e;  // 素数の2乗を昇順にリストアップ

	ll ans = 0;
#if 1
	for(ll i = 0; i < (ll)pl.size(); i++)
	{
		auto e1 = pl[i];  // e1 < e2 <= (N/e1)(切り捨て) でe1*e2<=N なるe2を求める
		auto[dmy1, num1] = bi_idxnum_ika(pl, N/e1);
		ll tmp = num1-(i+1);
		chmax(tmp, 0LL);  // e1>=e2になってしまうケース 0で下限クリップ
		ans += tmp;
	}
#else
	ll idx_e2 = (ll)pl.size()-1;
	for(ll i = 0; i < (ll)pl.size(); i++)  // idxがi,idx_e2の2つについて尺取り法っぽく探索 (i<idx_e2)
	{
		while(idx_e2 >= 0 && pl[i]*pl[idx_e2] > N)
		{
			idx_e2--;
		}
		if(i >= idx_e2) break;  // e1<e2 を満たさなくなった
		ans += idx_e2-i;
	}
#endif
	for(ll i = 2; i < 50; i++)  // i^8を調べる
	{
		if(!isPrime(i)) continue;
		if(modpow(i, 8, INF64) > N) break;
		ans++;
	}
	cout << ans << endl;

	return 0;
}
