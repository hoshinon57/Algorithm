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

// ABC392 https://atcoder.jp/contests/abc392

/*
 * 全探索で、ぱっと見では O(N^2*max(K)) になりそうなのが、
 * 実は O(N*ΣK) になってて十分に間に合うやつ。
 * 
 * 入力のAと、
 * サイコロごとの値の登場回数 acnt[i][j] を用意する。（サイコロiで値jがある個数）
 * 
 * サイコロ2つについて全探索する。これをi,jとする。
 * サイコロiの各値について、サイコロjで何回出ているかはacnt[j][*]により、
 * 各値ごとにO(1), サイコロi全体でO(Ki)で求められる。
 * 上記acntの総和をKi*Kjで割ったものが、サイコロi,jを選んだときの解となる。
 * この最大値が答。
 * 
 * 全体の計算量について、実装におけるiとhのループに着目すると O(ΣK) になるっぽい。
 * これとjのループを合わせて、全体で O(NΣK) になる模様。
 * 
 * [ACまでの思考の流れ]
 * ・コンテスト時、「これ計算量的にアウトだよな…」と悩んで時間を費やしてしまった。
 * ・いわゆる「ならし」なのかは分からないが、
 *   2つのfor文の計算量が全体で見るとO(N)とかになっている、というやつ。
 */

int main(void)
{
	ll i, j, h;
	ll N; cin >> N;
	vector<ll> k(N);
	const ll MXA = 1e5;  // Aの最大
	vector<vector<ll>> a(N);
	vector<vector<ll>> acnt(N, vector<ll>(MXA+1, 0));  // acnt[i][j]: サイコロiにて値jがある個数
	for(i = 0; i < N; i++)
	{
		cin >> k[i];
		a[i].resize(k[i]);
		for(j = 0; j < k[i]; j++)
		{
			cin >> a[i][j];
			acnt[i][a[i][j]]++;
		}
	}

	double ans = 0.0;
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)  // サイコロiとj
		{
			ll cnt = 0;  // 値が一致する組み合わせの数
			for(h = 0; h < k[i]; h++)
			{
				cnt += acnt[j][a[i][h]];
			}
			double tmp = (double)cnt/(k[i]*k[j]);
			chmax(ans, tmp);
		}
	}
	cout << std::fixed << std::setprecision(20);
	cout << ans << endl;

	return 0;
}
