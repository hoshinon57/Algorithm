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

// ABC356 https://atcoder.jp/contests/abc356

/*
 * コンテスト中に解けず、解説を見た。
 * いやーこれ難しい。方針は早期に立ったけれど、細かい部分の詰めがめっちゃ大変。
 * 累積和や調和級数の考え方で解く。
 * 
 * 分母を固定したときに、
 *   max/min=1 となる組み合わせの数
 *   max/min=2 となる組み合わせの数
 *   max/min=3...
 * を計算していくことを考える。
 * 分母を1～10^6まで変化させていったときに、これが調和級数の和になるのをイメージする。
 * 
 * cnt[i]をA=iとなる個数、sum[i]をその累積和として事前計算しておく。
 * 各iについて、それを分母の値としたときに、
 * jをiからiずつ加算していくと、以下の手順で計算できる。
 *   [j,j+i) の個数だけ、 max/min=j となる組み合わせが存在する。
 *   ただしΣの式より(j=i+1), 重複の除外が必要。
 *   これはN*Nの表をイメージして、j=iの初回では cnt[i]*(cnt[i]+1)/2 だけ除外してやれば良い。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・max/min=k となる組み合わせ数を求めたり、調和級数の考え方はコンテスト時に見えた。
 *   ただ重複部分などの細かい実装にハマってしまった。
 * ・二重シグマはN*Nの表をイメージする。
 */

int main(void)
{
	const ll MAXA = 1e6;
	ll i, j;
	ll N; cin >> N;
	vector<ll> cnt(MAXA+5), sum(MAXA+5);
	for(i = 0; i < N; i++)
	{
		ll a; cin >> a;
		cnt[a]++;
	}
	for(i = 0; i <= MAXA; i++) sum[i+1] = sum[i] + cnt[i];

	ll ans = 0;
	for(i = 1; i <= MAXA; i++)  // i=min(Ai,Aj)
	{
		if(cnt[i] == 0) continue;
		for(j = i; j <= MAXA; j+=i)  // [j,j+i) となるmax(Ai,Aj)について計算  max/min=j/i
		{
			ll r = min(j+i, MAXA+1);  // [j,j+i) -> 最大で[j,MAXA] -> [j,MAXA+1)
			ll tmp = cnt[i] * (sum[r] - sum[j]);
			if(j == i)  // 初回のみ重複除外が必要
			{
				tmp -= cnt[i] * (cnt[i]+1) / 2;
			}
			tmp *= j/i;
			ans += tmp;
		}
	}
	cout << ans << endl;

	return 0;
}
