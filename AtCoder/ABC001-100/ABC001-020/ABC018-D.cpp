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

// ABC018 https://atcoder.jp/contests/abc018

/*
 * 自力で解けず(反省)、公式解説や以下を見た。
 *   https://tsutaj.hatenablog.com/entry/2017/02/14/160504
 * 
 * 女子P人のグループを全探索する。next_permutation()を用いた。
 * 各組み合わせにて、各男子の幸福度の上位Q個を採用すればよい。
 * C(N,P)が最大で5*10^4程度なので、十分に間に合う。
 * 
 * bit全探索して、立っているbitがP個かチェックする方針でも解ける。
 * https://nullkara.jp/2021/02/09/atcoder_abc018/
 * bit全探索部分がO(2^N)で、そこから判定部分を追加すると間に合わないと思いこんでしまったが、
 * 「計算量がちゃんと見えますか」だった。反省。
 */

int main(void)
{
	ll i;
	ll N, M, P, Q; cin >> N >> M >> P >> Q;
	ll R; cin >> R;
	vector<ll> x(R), y(R), z(R); for(i = 0; i < R; i++) {cin >> x[i] >> y[i] >> z[i]; x[i]--; y[i]--;}

	ll ans = 0;
	vector<ll> co(N);
	for(i = 0; i < P; i++) co[i] = 1;
	sort(co.begin(), co.end());
	do  // 女子からP人選ぶ方法について全探索 選ぶ女子はco[x]=1
	{
		vector<ll> sc(M);  // sc[x]:その男子が持つスコア
		for(i = 0; i < R; i++)
		{
			if(co[x[i]] == 0) continue;  // その女子を選んでいない
			sc[y[i]] += z[i];
		}
		sort(sc.begin(), sc.end(), greater<ll>());

		ll tmp = 0;
		for(i = 0; i < Q; i++) tmp += sc[i];
		chmax(ans, tmp);
	} while (next_permutation(co.begin(), co.end()));
	cout << ans << endl;

	return 0;
}
