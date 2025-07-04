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

// ABC393 https://atcoder.jp/contests/abc393

/*
 * 調和級数をフル活用して解く。
 * ※コンテスト時はTLEギリギリの別解法だったが、
 *   数か月後に再度考えたら以下の公式解説とほぼ同じ実装にできていた。
 *   https://atcoder.jp/contests/abc393/editorial/12243
 *   https://atcoder.jp/contests/abc393/submissions/62850805
 * 
 * 各Aについて素因数分解をして何やかんや…とやると、計算量で10^9とかになるのでTLE.
 * 別方向から考えて、「ある値xの倍数がAに何個含まれているか？」で考えてみる。
 * 
 * Aに登場する値の回数をカウントすると、それを用いて
 *   bcnt[i]: Aに含まれるiの倍数の個数
 * を調和級数の要領で求めることができる。
 * 
 * また調和級数の要領で、iを1～MAX Aまでの各値として
 *   bcnt[i]>=Kなら、Aの要素にiの倍数はK個以上ある。
 *   A=i, i*2, i*3, i*4, ... となるAにて、それを含むようにK個の要素を選んだときに
 *   GCD=iとなるよう選ぶことができる。
 * となる。
 * 
 * ※snuke氏の実装と同じはずなのに実行時間に大きな差が出ており、いくつか試してみたら
 *   coutにてendlを'\n'に変えるとめっちゃ改善された。endl(1400ms), '\n'(350ms) とか。
 *   endlはフラッシュするので出力行数が多いときは時間がかかるみたい。
 *   '\n'は単なる改行。
 *   (Twitterで"abc393 e 出力"と検索すると、いくつかヒットした)
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・「ある値xの倍数がAに何個含まれているか？」をAの取りうる領域全てで計算したい場合、
 *   調和級数の要領でO(MlogM)で計算できる。(M=Max A)
**/

int main(void)
{
	ll i, j;
	ll N, K; cin >> N >> K;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	const ll MA = 1e6;
	vector<ll> acnt(MA+5); for(auto &e : a) {acnt[e]++;}  // ca[i]:a=iの個数
	vector<ll> bcnt(MA+1);
	for(i = 1; i <= MA; i++)
	{
		for(j = i; j <= MA; j+=i)
		{
			bcnt[i] += acnt[j];
		}
	}
	// bcnt[i]: Aに含まれるiの倍数の個数

	vector<ll> ans(MA+1);
	for(i = 1; i <= MA; i++)
	{
		if(bcnt[i] < K) continue;
		for(j = i; j <= MA; j+=i)
		{
			ans[j] = i;  // ans[j]は上書きされるが、後優先でよい
		}
	}

	for(i = 0; i < N; i++)
	{
//		cout << ans[a[i]] << endl;
		cout << ans[a[i]] << '\n';  // endl(1400ms), '\n'(350ms) と大幅に変わった
	}

	return 0;
}
