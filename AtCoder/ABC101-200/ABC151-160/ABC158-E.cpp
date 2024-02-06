#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC158 https://atcoder.jp/contests/abc158

/*
 * 自力で解けず、解説を見た。
 * 合同式や累積和といった考え方で解く。
 *   https://drken1215.hatenablog.com/entry/2020/03/08/020200
 *   https://algo-logic.info/abc158e/
 *   https://blog.hamayanhamayan.com/entry/2020/03/08/100748
 * 
 * i>jとし、右側からi桁、j桁を取り出した値をni,njとしてみる。
 * 区間i～(j-1)桁がPで割り切れるならば、
 *   (ni-nj) / (10^j) ≡ 0 (mod P)  -> (1)
 * となる。
 * ここでPと10が互いに素であれば、10^jの逆元が存在するので、
 *   ni-nj ≡ 0 (mod P)   -> (2)
 * となるi,jを求めることで、結果的に(1)が求められる。
 * 
 * (2)は ni ≡ nj なので、
 *   cnt[i]:数値をPで割った余りがiになる個数
 * として、文字列Sの右側から順に計算していけば求められる。
 * 
 * ただし以上はPと10が互いに素である場合。
 * Pが2,5のときは(2)から(1)が成り立たない。
 * これはコーナーケースとして個別に処理する。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・ある区間の値がPで割り切れる、は
 *   「右から累積和っぽく見たときに、Pで割った余りが等しい」と読み替えてみる。
 *   またこの場合、数値を左からではなく右から見た方が処理しやすいかも。
 */

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

ll solve1(ll N, ll P, string &s)
{
	int i;
	ll ans = 0;
	reverse(s.begin(), s.end());
	for(i = 0; i < N; i++)  // 下からi桁目
	{
		ll d = s[i] - '0';
		if(d % P == 0)
		{
			ans += N-i;  // i桁目を右端としたとき、左側は何桁あっても常にPで割り切れる
		}
	}
	return ans;
}

ll solve2(ll N, ll P, string &s)
{
	int i;
	vector<ll> cnt(P);
	cnt[0] = 1;
	ll ans = 0;
	reverse(s.begin(), s.end());
	ll num_old = 0;
	for(i = 0; i < N; i++)  // 下からi桁目
	{
		ll num = 0;  // 下からi桁をPで割った余り
		ll d = s[i] - '0';
		if(i == 0)
		{
			num = d % P;
		}
		else
		{
			num = num_old + d * modpow(10, i, P);
			num %= P;
		}
		ans += cnt[num];  // これまでにPで割った余りが等しいものがあれば、その区間はPで割り切れる
		cnt[num]++;
		num_old = num;
	}
	return ans;
}

int main(void)
{
	ll N, P; cin >> N >> P;
	string s; cin >> s;

	ll ans = 0;
	if(P == 2 || P == 5) ans = solve1(N, P, s);
	else ans = solve2(N, P, s);
	cout << ans << endl;

	return 0;
}
