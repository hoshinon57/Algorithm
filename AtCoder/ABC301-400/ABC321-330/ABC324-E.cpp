#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC324 https://atcoder.jp/contests/abc324

/*
 * まず、各Sについて、
 *   Sの部分列が、Tの先頭から最大で何文字持っているか
 *   Sの部分列が、Tの末尾から最大で何文字持っているか
 * を求める。(実装ではpre, aftとしている)
 * 
 * あるSiについてpreがxのとき、後ろの(|T|-x)文字が欲しいので、
 * aftが(|T|-x)以上である要素の数が、条件を満たす組み合わせとなる。
 * 
 * 実装ではpre[i]を Tの先頭からi文字持っているSの数 とした。aft[]も同様。
 * また前述の "aftが(|T|-x)以上である要素の数" は、累積和にて求めている。
 * 
 * [ACまでの思考の流れ]
 * ・組み合わせがN^2通りだが、これを愚直に調べるのは到底間に合わない。
 * ・Sの2つを組み合わせる ⇒ Tのうち前側と後ろ側を担当するイメージ。
 * ・各Sについて、Tの先頭/末尾それぞれから何文字一致するかを計算することで解けないか。
 */

const ll TMAX = 500000+5;
ll pre[TMAX], aft[TMAX], aft_sum[TMAX+1];

// sの部分列が、tの先頭から最大何文字一致するかを返す
int calc(string &s, string &t)
{
	int cnt = 0;
	int tlen = (int)t.size();
	for(auto &c : s)
	{
		if(cnt >= tlen) break;
		if(c == t[cnt]) cnt++;
	}
	return cnt;
}

int main(void)
{
	ll i;
	ll N;
	string T, Trev;
	cin >> N >> T;
	int tlen = (int)T.size();
	Trev = T; reverse(Trev.begin(), Trev.end());
	// 各Sについて、Sの部分列がTの先頭/末尾から最大で何文字持っているかを計算
	for(i = 0; i < N; i++)
	{
		string s; cin >> s;
		// 前から
		ll tmp = calc(s, T);
		pre[tmp]++;
		// 後ろから
		reverse(s.begin(), s.end());
		tmp = calc(s, Trev);
		aft[tmp]++;
	}

	for(i = 0; i <= tlen; i++)
		aft_sum[i+1] = aft_sum[i] + aft[i];
	
	ll ans = 0;
	for(i = 0; i <= tlen; i++)
	{
		// 前からi文字一致しているならば、
		// 後ろから(|T|-i)文字以上一致しているものと組み合わせ可能。
		ll a, b;
		a = pre[i];
		b = aft_sum[tlen+1] - aft_sum[tlen-i];  // [|T|-i, tlen)
		ans += a*b;
	}
	cout << ans << endl;

	return 0;
}
