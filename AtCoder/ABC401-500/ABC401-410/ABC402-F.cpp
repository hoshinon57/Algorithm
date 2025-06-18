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

// ABC402 https://atcoder.jp/contests/abc402

/*
 * 半分全列挙。
 * 類題はABC271-F(XOR on Grid Path).
 * 
 * 左上からDFSと右下からDFSを、対角線のマスで組み合わせる。
 * DFSはそれぞれ実装するのが楽だった。(1関数にまとめない)
 * 
 * 左上からDFSした結果の値は、事前に 10^(N-1) をかけておく。
 * これをeとしたとき、M-e未満の最大の値を、右下からDFSした結果より探す。
 * 無かった場合は、右下からDFSした結果の末尾の要素を使う。
**/

// ソート済のvector<T>a に対して、(未満,以下,より大きい,以上)となるidxと要素数を返す
// {val未満の最右のidx, val未満の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_miman(vector<T> &a, T val) {ll idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}
// {val以下の最右のidx, val以下の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_ika(vector<T> &a, T val)   {ll idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}
// {valより大きい最左のidx, valより大きい要素数} 要素が無ければidx=N
template<typename T> pair<ll,ll> bi_idxnum_koeru(vector<T> &a, T val) {ll idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx,(ll)a.size()-idx};}
// {val以上の最左のidx, val以上の要素数} 要素が無ければidx=N
template<typename T> pair<ll,ll> bi_idxnum_ijou(vector<T> &a, T val)  {ll idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx,(ll)a.size()-idx};}

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
	ll i, j;
	ll N, M; cin >> N >> M;
	vector<vector<ll>> a(N, vector<ll>(N, 0));  // a[N][N]
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++) cin >> a[i][j];
	}

	vector<vector<ll>> li_f(N);  // li[y]
	ll pp = modpow(10, N-1, M);
	auto dfs_front = [&](auto &self, ll y, ll x, ll n) -> void
	{
		n = n*10 + a[y][x];
		n %= M;
		if(y+x+1 == N)
		{
			n = n*pp%M;  // 後ろからDFSのぶん、桁を足しておく
			li_f[y].push_back(n);
			return;
		}
		self(self, y+1, x, n);
		self(self, y, x+1, n);
	};
	dfs_front(dfs_front, 0, 0, 0);

	vector<vector<ll>> li_b(N);  // li[y]
	auto dfs_back = [&](auto &self, ll y, ll x, ll n, ll p) -> void
	{
		if(y+x+1 == N)
		{
			li_b[y].push_back(n);
			return;
		}
		n = a[y][x]*p + n;
		n %= M;
		p = p*10%M;
		self(self, y-1, x, n, p);
		self(self, y, x-1, n, p);
	};
	dfs_back(dfs_back, N-1, N-1, 0, 1);

	for(auto &e : li_b) sort(e.begin(), e.end());
	ll ans = 0;
	for(ll y = 0; y < N; y++)
	{
		for(auto &e : li_f[y])
		{
			// M-e未満のMAXをli_b[y]から探す
			// なければli_b[y]の末尾を使う
			auto[idx,num] = bi_idxnum_miman(li_b[y], M-e);
			ll v = 0;
			if(idx != -1) v = li_b[y][idx];
			else v = li_b[y].back();
			chmax(ans, (e+v)%M);
		}
	}
	cout << ans << endl;

	return 0;
}
