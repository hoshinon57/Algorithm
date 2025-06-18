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

// ABC408 https://atcoder.jp/contests/abc408

/*
 * 期待値DP&bitDP的な。
 * 
 * dp[i][j]を以下のように定義する。
 *   i: 問題ごとにbitで表し、bitごとに未ACなら1
 *   残金がj
 *   のときの、そこからのスコア最大値
 * 
 * 「どの問題に提出するかを決めることができる」ということは、
 * 各問題に提出してそのMAXを採用する、ということ。
 * 
 * 問題ごと、WAとACの遷移先の和を計算し、その最大値を採用する。
 * 詳細は実装を参照。
**/

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

vector<vector<double>> dp;
vector<vector<bool>> memo;

ll N, X;
vector<ll> s, c;
vector<double> p;

// dp[b][m]を返す
double f(ll b, ll m)
{
	if(memo[b][m]) return dp[b][m];
	if(b == 0 || m <= 0) return 0.0;

	ll i;
	auto &d = dp[b][m];
	for(i = 0; i < N; i++)  // 問題i
	{
		if(!isbiton(b, i)) continue;
		if(m-c[i] < 0) continue;

		// WA
		auto tmp1 = f(b, m-c[i]) * (1.0-p[i]);

		// AC
		auto bb = b;
		unbit(bb, i);
		auto tmp2 = (f(bb, m-c[i])+s[i]) * p[i];

		chmax(d, tmp1+tmp2);
	}

	memo[b][m] = true;
	return d;
}

int main(void)
{
	ll i;
	cin >> N >> X;
	
	s.resize(N);
	c.resize(N);
	p.resize(N);
	for(i = 0; i < N; i++) {cin >> s[i] >> c[i] >> p[i]; p[i]/=100.0;}
	dp.resize(1<<N, vector<double>(X+1, 0.0));
	memo.resize(1<<N, vector<bool>(X+1, false));

	cout << std::fixed << std::setprecision(20);
	cout << f((1<<N)-1, X) << endl;

	return 0;
}
