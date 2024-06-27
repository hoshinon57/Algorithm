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

// ABC300 https://atcoder.jp/contests/abc300

/*
 * 二分探索＆累積和で解く。
 * 
 * 答が連続X文字であるとき、連続(X-1)文字も達成可能。逆に(X+1)文字以上はいずれも達成不可能。
 * そこから以下の判定問題を考える。
 *   左端をi文字目としたときに、x文字以上連続して'o'にできるか？
 * この境界を二分探索で求め、そして各i [0,N) についてそれぞれ求めた値の最大値が答となる。
 * 
 * 判定問題は、xについて'x'の個数を求めてそれがK個以内ならOKとなる。
 * 長さNを1ループとして、何周するか？ とループしない端数部分の数を求めればO(1)で計算できる。
 * ここで累積和を用いた。いわゆる円環のため、2周させることで端数部分の実装を簡易にした。
 * 
 * ok,ngの初期値に注意。
 * 文字列は有限回の繰り返しであり、i文字目から取れる最大文字数は(NM-i)である。
 * よってngの初期値は(NM-i+1)にしておく必要がある。
 * このため左端iによってng初期値が異なることから、各iについて二分探索する必要がある。
 * (この辺りの検討ミスでWAを繰り返してしまった)
 */

int main(void)
{
	ll i;
	ll N, M, K; cin >> N >> M >> K;
	string s; cin >> s;
	vector<ll> sum(N*2+1);  // 実装を簡易にするため2周
	for(i = 0; i < 2*N; i++)
	{
		sum[i+1] = sum[i];
		if(s[i%N] == 'x') sum[i+1]++;
	}

	// st文字目を左端として、連続してx文字以上可能ならtrue
	auto check = [&](ll st, ll x) -> bool
	{
		// 'x'の個数を数えて、それがK個以内ならOKとする
		ll tot1loop = sum[N] - sum[0];  // Sの1ループにおける'x'の個数
		ll loop_sum = tot1loop * (x/N);  // Sを何周するかでかけることで、ループ部分の'x'の個数

		ll zan = x%N;  // ループしない端数部分
		ll tmp = sum[st+zan] - sum[st];  // [st,st+zan) における'x'の個数
		tmp += loop_sum;
		return (tmp <= K);
	};

	ll ans = 0;
	for(i = 0; i < N; i++)  // i文字目を左端として調べる
	{
		ll ok = 0, ng = N*M-i+1;  // i文字目から取れる最大文字数は(N*M-i). よってその+1はNG
		while(ng - ok > 1)
		{
			ll mid = (ok+ng)/2;
			if(check(i, mid)) ok = mid;
			else ng = mid;
		}
		chmax(ans, ok);
	}
	cout << ans << endl;

	return 0;
}
