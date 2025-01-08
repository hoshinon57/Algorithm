#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <unordered_map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC208 https://atcoder.jp/contests/abc208

/*
 * 桁DPで解く。leading-zeroの練習。
 * hamayanhamayan氏の解説をかなり参考にした。
 *   https://blog.hamayanhamayan.com/entry/2021/07/05/013342
 * 
 * dp[i][sm][lz][mul]を以下のように定義する。
 *   i桁目まで見て、 (i=1～L)
 *   そこまでの値が、N未満ならsm=1, Nと一致ならsm=0で、
 *   leading-zero中なら1, leading-zeroを抜けたら0で、
 *   各桁の積がmul
 *   である正の整数の種類数。
 * mulは状態数が少ないが(※)、値としては大きくなるため、連想配列で持たせる。
 * (※)K=10^9としても、2だけで考えると2^30で30通りぐらいしか無い。
 *   1桁の素因数は2,3,5,7であり、30^4は超えなさそう。
 * 
 * DP遷移は実装、およびdp-digitDP.cppを参照。
 * [i]を直近2要素のみ持つことで、次元を削除している。
 * 
 * Nの桁数をLとして、dp[L][sm=0,1][lz=0][mul<=K]の総和が答。
 */

int main(void)
{
	string s; cin >> s;
	int L = (int)s.size();
	ll K; cin >> K;

	// dp[sm][lz][mul(map)]
	vector<vector<unordered_map<ll,ll>>> dp(2, vector<unordered_map<ll,ll>>(2));
	// DP初期値は「Nと等しく、leading-zero中で、積が1」というケース。
	dp[0][1][1] = 1;

	// 配るDP
	int i, sm, lz;
	for(i = 0; i < L; i++)
	{
		vector<vector<unordered_map<ll,ll>>> ndp(2, vector<unordered_map<ll,ll>>(2));
		for(sm = 0; sm < 2; sm++)
		{
			for(lz = 0; lz < 2; lz++)
			{
				for(auto &[mul,v] : dp[sm][lz])
				{
					const int D = s[i] - '0';  // 配る先の値
					int ed = (sm ? 9 : D);  // 配る元が"未満"なら9まで、まだ一致ならDまで
					for(int d = 0; d <= ed; d++)  // 次に使う値
					{
						// 配る元が"未満", もしくは配り先の値がD未満なら、smaller=1.
						int nsm = (sm || d < D);

						// nlzはlzを引き継ぐ。ただし次に使う値が0以外ならnlz=0.
						int nlz = lz;
						if(d != 0) nlz = 0;

						// leading-zero中であれば、積は1とする
						ll nmul = mul * d;
						if(nlz == 1) nmul = 1;

						// ndp[nsm][nlz][nmul] += dp[sm][lz][mul];
						ndp[nsm][nlz][nmul] += v;
					}
				}
			}
		}
		swap(dp, ndp);
	}

	ll ans = 0;
	// Nの桁数をLとして、dp[L][sm=0,1][lz=0][mul<=K]の総和が答
	for(sm = 0; sm < 2; sm++)
	{
		for(auto &[mul,v] : dp[sm][0])
		{
			if(mul > K) continue;
			ans += v;
		}
	}
	cout << ans << endl;

	return 0;
}
