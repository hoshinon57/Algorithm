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

// ABC389 https://atcoder.jp/contests/abc389

/*
 * コンテスト時に解けず、解説を見た。二分探索で解く。
 * 基本は公式解説の通り。
 *   https://atcoder.jp/contests/abc389/editorial/11933
 * 
 * ぱっと見で二分探索だが、「x個買えるか？」の判定問題だと解けない。
 * 「x円以下の品物を買い占めたとき、M円以下か？」の判定問題で解く。
 * ABC149-E(Handshake)が近いか。
 * 
 * Piのj番目だけを買う場合、(2j-1)*Pi円となる。
 * すると、x円以下の品物を全て買うとき、その個数は
 *   (2j-1)*P <= x を整理して
 *   j <= (x+P)/2P
 * だけ買うことができる。
 * このときの種類Piの総額が j^2*Pi.
 * 
 * 二分探索にて買い占めがx円までと分かったとき、(x+1)円の品物も買えるだけ買う必要がある。
 * これがj番目とすると
 *   (2j-1)*P = x+1 を整理して
 *   j = (x+1+P)/2P
 * となり、jが整数なら(x+1)円の品物が存在する。
 * ⇒ここ、もっと単純な実装があった。実装を参照。
 * 
 * 以上を実装すればよい…が、各所でオーバーフローに注意が必要。
 * 「オーバーフロー判定をdouble型でざっくり行うテクニック」や
 * 「加算ではなく減算で大小関係を判定」などを用いた。
 * 詳細は実装を参照。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・解説ACにてオーバーフローに苦戦したが、そもそも二分探索の方針に気づけないとスタートラインに立てない。
 * ・個数で二分探索ではなく、「x円以下を全て買ったときに条件を満たすか？」で二分探索する方針を思いつけるようにする。
 */

int main(void)
{
	ll i;
	ll N, M; cin >> N >> M;
	vector<ll> p(N); for(i = 0; i < N; i++) {cin >> p[i];}

	// x円以内を買い占めて、M円以内か？
	// sum:購入金額, num:購入した数
	auto check = [&](ll x, ll &sum, ll &num) -> bool
	{
		for(i = 0; i < N; i++)
		{
			ll c = (x+p[i])/(2*p[i]);  // この個数だけ買える

			// オーバーフロー判定をdouble型でざっくり行うテクニック
			if((double)c*c*p[i]+sum > INF64) return false;

			sum += c*c*p[i];
			num += c;
		}
		return (sum<=M);
	};

	ll ok = 0;
	ll ng = INF64;
	while(ng - ok > 1)
	{
		ll mid = (ok+ng)/2;  // mid円以下を買い占めて、M円以下か？
		ll dmy1 = 0, dmy2 = 0;
		if(check(mid, dmy1, dmy2)) ok = mid;
		else ng = mid;
	}

	// (ok+1)円の品物があれば、M円以内の範囲で買う
	ll sum = 0, num = 0;
	check(ok, sum, num);
#if 1
	// 最初は#elseの方で実装していたが、これで十分だった。(snuke氏の解説より)
	// ok円でOKで,(ok+1)円でNGになるのならば、(ok+1)の品物はM円オーバーするだけ存在するはずなので。
	num += (M-sum)/(ok+1);
#else
	for(i = 0; i < N; i++)
	{
		//if(sum+(ok+1) > M) break;
		if(sum > M-(ok+1)) break;

		if( ((ok+1)+p[i]) % (2*p[i]) == 0)  // (ok+1)円の品物があるならば、それは (ok+1+Pi)/(2Pi) 番目。これが整数であること。
		{
			sum += (ok+1);
			num++;
		}
	}
#endif
	cout << num << endl;

	return 0;
}
