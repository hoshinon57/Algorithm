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
 * 公式解説の方針で解いた版。
 *   https://atcoder.jp/contests/abc408/editorial/13166
 * 
 * 基本的に解説の通り。
 * chmaxのタイミング、公式解説ではchminの後だったが、chminの前でもACする。
 * (というよりも、作り上げた数式的には"前"の方がしっくり来る気がする)
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・2つの要素、例えばlとrについて全組み合わせについて調べる必要があるとき、
 *   片方で全探索すればもう片方については最適な値がO(1)で求まることがある。
**/

void solve(void)
{
	ll i;
	ll N; cin >> N;
	string s; cin >> s;
	vector<ll> a(N+1), b(N+1);
	for(i = 1; i <= N; i++)
	{
		a[i] = a[i-1];
		if(s[i-1] == '0') a[i]++;
		b[i] = b[i-1];
		if(s[i-1] == '1') b[i]++;
	}

	ll ans = INF64;
	ll ma = 0;
	for(i = 0; i <= N; i++)
	{
		ll c = a[i] - b[i];
		chmax(ma, c);
		chmin(ans, b[N] + c - ma);
//		chmax(ma, c);  // こっちでも良い
	}
	cout << ans << endl;
}

int main(void)
{
	int T; cin >> T;
	while(T > 0)
	{
		T--;
		solve();
	}
	return 0;
}
