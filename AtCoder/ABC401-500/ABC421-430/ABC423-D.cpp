#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC423 https://atcoder.jp/contests/abc423

/*
 * コンテスト時はイベントソートっぽく解いたが、想定解法に比べてかなり重い実装をしてしまった。
 *   https://atcoder.jp/contests/abc423/submissions/69326620
 * 
 * シミュレーションで解く感じ。基本は公式解説の通り。
 * 各種解説を色々読んだ後に実装したら、公式解説とほぼ同じ意図の実装になっていた。
 *   https://atcoder.jp/contests/abc423/editorial/13871
 * 
 * 各グループが入店する時刻が答となっているので、これをans[i]とする。
 * グループiが行列の先頭に来るのをまず考え、これをAi'とすると、、
 * そもそもグループ(i-1)が入店しないと話にならないので、
 *   Ai' = max(ans[i-1], a[i])
 * となる。
 * 
 * Ai'のとき、c[i]人以上お店には入れればそのタイミング。
 * 入れなければ、退店時刻が早い順に、c[i]人以上は入れるようになるまで
 * 店内のグループをシミュレーションしていく。
 * これは優先度付きキューを用いればよい。
 * 
 * シミュレーション中に、さらに次のグループが到着することもありえるが(時刻の逆転)、
 * グループiが入店するまでグループ(i+1)は何もできないので、上記処理でOK.
 *   Ai' = max(ans[i-1], a[i])
 * を実施しているわけだし。
**/

int main(void)
{
	using pll = pair<ll,ll>;
	ll i;
	ll N, K; cin >> N >> K;
	vector<ll> ans(N);
	priority_queue<pll, vector<pll>, greater<pll>> que;
	ll ti = 0, cnt = 0;  // 現在時刻, 現在の店内の人数
	for(i = 0; i < N; i++)
	{
		ll a, b, c; cin >> a >> b >> c;
		chmax(ti, a);
		if(cnt+c <= K) {;} // 今入れる
		else  // 退店待ち
		{
			while(!que.empty() && cnt+c > K)  // 退店シミュレーション
			{
				auto[tt, cc] = que.top();
				que.pop();
				chmax(ti, tt);
				cnt -= cc;
			}
		}
		// 入店処理 退店時刻をキューに積む
		cnt += c;
		ans[i] = ti;
		que.push({ti+b, c});
	}

	for(auto &e : ans) cout << e << endl;

	return 0;
}
