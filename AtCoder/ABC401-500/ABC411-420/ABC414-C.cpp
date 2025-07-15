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

// ABC414 https://atcoder.jp/contests/abc414

/*
 * 回文となる値を列挙することを考える。
 * 前半部分を反転させると回転数になることから、チェックするのは「桁数の半分」、つまり√Nまでで良い。
 * √Nの最大値は10^6程度であることから、全探索できる。
 * 
 * 数値iから10進の回文を作るとき、
 *   123 -> 123321
 *   123 -> 12321
 * の2パターンがある。
 * それぞれの回文数を作り、それを8進数に直したときも回文数であるかチェックしていく。
 * 
 * [ACまでの思考の流れ]
 * ・回文数は値の前半部分を決めると後半が自動で決まる。
 *   よって前半部分を全探索すれば十分。上限をNとしてO(√N).
**/

#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

// [verify]ABC336-C, ABC234-C, ABC372-B
// nをbase進法で表したときの値を返す (n=0が変換後も0に対応する)
// 戻り値は、一番下の桁から順に[0],[1],... と格納される
vector<int> chg_base(ll n, int base)
{
	if(n == 0) return {0};  // 0のときだけ特別に処理
	vector<int> a;
	while(n > 0) {
		a.push_back(n%base);
		n /= base;
	}
	return a;
}

int main(void)
{
	ll i;
	ll A, N; cin >> A >> N;
	
	// valを8進数で表したときに回文数ならvalを返す
	auto chk = [&](ll val) -> ll
	{
		auto v = chg_base(val, A);
		ll l=0, r = (ll)v.size()-1;
		while(l<=r)
		{
			if(v[l] == v[r])
			{
				l++;
				r--;
			}
			else return 0;  // 非 回文数
		}
		return val;
	};

	ll ans = 0;
	// 数値iから10進の回文を作る
	for(i = 1; i <= 1e6+5; i++)
	{
		string s1 = to_string(i);
		string s2 = s1;
		reverse(s2.begin(), s2.end());

		// 123 -> 123321
		auto tmp = stoll(s1+s2);
		if(tmp <= N) ans += chk(tmp);

		// 123 -> 12321
		s1.pop_back();
		tmp = stoll(s1+s2);
		if(tmp <= N) ans += chk(tmp);
	}
	cout << ans << endl;

	return 0;
}
