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

// ABC118 https://atcoder.jp/contests/abc118

/*
 * DPおよびDP復元で解く。各種解説と同じ方針。
 *   公式解説
 *   https://scrapbox.io/procon-kirokuyou/ABC118_D_-_Match_Matching_(400)
 *   https://scrapbox.io/esaka-sandbox/ABC118_D_-_Match_Matching
 * 
 * 最大値は桁数が最も多いものだから、使うマッチ棒の数が最も少ない数字を延々と採用して…と思ったが、
 * 「ちょうどN本使う」という制約により、これだけではダメ。
 * 
 * とすると見た目がDPっぽくて、以下のように定義した。
 *   dp[i]:i本ちょうど使って作れる整数の、"桁数"の最大値 (作れない場合はdp=-1)
 * 整数そのものを管理するのは、制約より10^4桁とかの値になるので、C++では厳しい。
 * dp[N]の桁数が決まったら、値はDP復元にて求めていく。
 * 
 * 1)dp[N]の求め方
 * use[x]をマッチをx本使うときの、その数字の最大値とする。これの構築は実装を参照。
 * もらうDPで、use[j]が存在するなら chmax(dp[i], dp[i-j]+1) とすれば良い。
 * 
 * 2)DP復元
 * dp[N]から、そこから遷移できる先で dp[N]=dp[nxt]+1 となるnxtを求めていく。
 * nxt候補が複数ある場合、その数値が大きいものを採用する。
 * 
 * 「最後にソート必要かな？」とか考えたが、
 * 例えば後半で数値"9"を採用した場合、最初の時点で"9"を採用できているはず。
 * なので復元した時点で数値は降順にならんでいる。
 * 
 * [余談]
 * DPに桁数ではなく数値そのものを、文字列として持たせる解法をいくつか見た。
 * なるほど、10^4*10^4=10^8より、メモリや実行時間には入りそうなのか…
 * https://drken1215.hatenablog.com/entry/2019/02/16/224100
 * https://mikanbox55.hatenadiary.jp/entry/2019/04/13/073757
 * 
 * [典型メモ]
 * ・DPに保持する値は求める答そのものではない、そこからDP復元等で答を構築していく、という考え方。
**/

int main(void)
{
	ll i, j;
	ll N, M; cin >> N >> M;
	vector<ll> use(8, -1);  // use[x]:マッチをx本使うときの、その数字の最大値 -1はx本使えるものが無い
	vector<ll> ma = {-1, 2, 5, 5, 4, 5, 6, 3, 7, 6};
	for(i = 0; i < M; i++)
	{
		ll a; cin >> a;
		chmax(use[ma[a]], a);
	}

	vector<ll> dp(N+1, -1);  // dp[n]:n本ちょうどで作れる整数の"桁数"の最大値  dp=-1は作れない
	dp[0] = 0;
	for(i = 1; i <= N; i++)  // dp[i]を求める
	{
		for(j = 0; j < (int)use.size(); j++)
		{
			if(use[j] == -1) continue;
			if(i-j < 0) continue;
			chmax(dp[i], dp[i-j]+1);
		}
	}

	// DP復元
	string ans;
	ll now = N;
	while(now > 0)
	{
		ll mx = -1, idx = 0;
		for(j = 0; j < (int)use.size(); j++)  // now本からj本使う先があるか
		{
			if(use[j] == -1) continue;
			if(now-j < 0) continue;
			if(dp[now] == dp[now-j]+1)
			{
				if(chmax(mx, use[j])) idx = now-j;
			}
		}
		// dp[now]からの遷移先で、最も大きな数値を使う
		ans += to_string(mx);
		now = idx;
	}
	cout << ans << endl;

	return 0;
}
