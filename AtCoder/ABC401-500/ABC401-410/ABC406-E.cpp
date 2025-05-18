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

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

#include <atcoder/modint>
using namespace atcoder;
using mint = modint998244353;

// ABC406 https://atcoder.jp/contests/abc406

/*
 * コンテスト時に解けず、解説を見た。桁DPで解く。
 * 
 * 「bitごとに1になるケースが何個あるか？」を軸に考える。[数え上げの典型]
 * 2進数で考えて、上の桁(bit)から以下のDPを考える。
 *   dp[sm][pc]:
 *   N未満である:smaller=1, Nと等しい:smaller=0
 *   そこまでにbitが1である個数がpc
 *   のときの、{組み合わせ数, 総和}
 * DP初期値は{1,0}.
 * 
 * 配るDPで、次に使うbitが0か1かで場合分けする。
 * 0のとき：
 *   組み合わせ数は遷移前のものと同じ。
 *   総和も増えない。遷移前のものと同じ。
 * 1のとき：
 *   組み合わせ数は遷移前のものと同じ。
 *   総和は増える。遷移先がiビット目とすると、(遷移前の組み合わせ数)*2^i だけ増えることになる。
 * 
 * 最下位ビットまで見て、dp[0][K]+dp[1][K]が答。
 * 桁DPの細かい内容は実装を参照。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・桁ごとに考える問題では桁DPが有効なことが多い。
 *   ⇒今回はbitごとに考えるので、これもまた桁ごとと言える。
 * ・"N以下"という条件も、桁DPが有効なことが多い。
**/

void solve(void)
{
	ll N, K; cin >> N >> K;
	using plm = pair<ll,mint>;  // {組み合わせ数, 総和}
	vector<vector<plm>> dp(2, vector<plm>(65));
	dp[0][0] = {1, 0};

	ll i, sm, pc, d;
	// 配るDP
	for(i = 62; i >= 1; i--)
	{
		vector<vector<plm>> ndp(2, vector<plm>(65));
		for(sm = 0; sm < 2; sm++)
		{
			for(pc = 0; pc < 65-1; pc++)  // dp[sm][pc]から配る
			{
				const int D = (isbiton(N, i-1) ? 1 : 0);  // 配る先の値
				int ed = (sm ? 1 : D);
				for(d = 0; d <= ed; d++)  // 次に使う値
				{
					int nsm = (sm || d < D);
					int npc = ((d==1) ? pc+1 : pc);

					if(d == 0)  // 使わないケース
					{
						ndp[nsm][npc].first += dp[sm][pc].first;
						ndp[nsm][npc].second += dp[sm][pc].second;
					}
					else  // 使うケース
					{
						ndp[nsm][npc].second += dp[sm][pc].first * (1LL<<(i-1));
						ndp[nsm][npc].first += dp[sm][pc].first;
						ndp[nsm][npc].second += dp[sm][pc].second;
					}
				}
			}
		}
		swap(dp, ndp);
	}
	mint ans = dp[0][K].second + dp[1][K].second;
	cout << ans.val() << endl;
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
