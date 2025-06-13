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

// ABC400 https://atcoder.jp/contests/abc400

/*
 * 各種解説の通り。
 * 2番目の制約より、Nが400numberなら√Nは整数、つまりNは平方数となる。
 * 
 * Aの制約より、√Aあたりまで全探索したいな、となる。
 * √Nの素因数も2つなので、エラトステネスの篩の要領で
 *   cnt[i]:値iが持つ素因数の数
 * を事前計算しておき、cnt[i]=2となるiが√Nとなる。
 * これを2乗したものをリストに持たせておき、クエリに対してupper_bound/lower_boundで求められる。
**/

// ソート済のvector<T>a に対して、(未満,以下,より大きい,以上)となるidxと要素数を返す
// {val以下の最右のidx, val以下の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_ika(vector<T> &a, T val)   {ll idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}

int main(void)
{
	ll i, j;
	const ll MX_RA = 1e6;
	vector<ll> cnt(MX_RA+1);  // cnt[i]:値iの素因数の数
	for(i = 2; i <= MX_RA; i++)
	{
		if(cnt[i] != 0) continue;
		for(j = i; j <= MX_RA; j+=i)
		{
			cnt[j]++;
		}
	}
	vector<ll> li;  // 400numberの一覧
	for(i = 0; i <= MX_RA; i++)
	{
		if(cnt[i] == 2) li.push_back(i*i);
	}

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		ll a; cin >> a;
		auto tmp = bi_idxnum_ika(li, a);
		cout << li[tmp.first] << endl;
	}

	return 0;
}
