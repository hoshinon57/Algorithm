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

// ABC116 https://atcoder.jp/contests/abc116

/*
 * いやーーーーー難しかった。色々解説を見た。
 * この解説が最も近いかな。
 *   https://scrapbox.io/procon-kirokuyou/ABC116_D_(400)
 * たぶん公式解説も同じ方針っぽい。
 * 
 * ざっくり以下の流れ。
 * ・まず貪欲においしさが大きいものをK個選ぶ
 * ・選んだものと、選んでないネタと交換していく。
 *   選ぶ種類数を増やしていくイメージ。
 * 
 * [ACまでの思考の流れ]
 * ・貪欲においしさが大きいものをまずK個選ぶことを考えてみる。
 * ・ここから交換して最善のものを探していくときに、
 *   交換してもおいしさは増えない、という事実がある。
 *   ⇒つまり、1つも選んでいないネタのやつと交換するしかない、となる。
 *     また、あるネタで1つしか選んでいないものは、交換できない、ともなる。
 * 
 * なので、
 * 2つ以上選んでいるネタで、おいしさが小さい方から、
 * 1つも選んでないネタのうち、おいしさが大きい方から、
 * それぞれ交換していく。
 * すると交換ごとにネタ数は1つずつ増えていく。
 */

int main(void)
{
	ll i, j;
	ll N, K; cin >> N >> K;
	vector<vector<ll>> ta(N);  // ta[i]={9,9,6,3,...} ネタiの寿司のおいしさ一覧 降順ソート
	vector<pair<ll,ll>> tot;  // {おいしさ, ネタ} で全まとめ おいしさで昇順ソート
	vector<ll> ch(N);  // ch[i]:ネタiは何個選んだか
	for(i = 0; i < N; i++)
	{
		ll t, d; cin >> t >> d;
		t--;
		ta[t].push_back(d);
		tot.push_back({d, t});
	}
	for(auto &e1 : ta)
	{
		sort(e1.begin(), e1.end(), greater<ll>());
	}
	sort(tot.begin(), tot.end(), greater<pair<ll,ll>>());

	ll sum = 0, ans = 0;
	ll ki = 0;
	// まず、おいしい方から貪欲にK個選ぶ
	for(i = 0; i < K; i++)
	{
		auto [d,t] = tot[i];  // おいしさd, ネタt
		if(ch[t] == 0) ki++;
		ch[t]++;
		sum += d;
	}
	ans = sum + ki*ki;

	// 2個以上選んでいるネタのうち、各ネタで最もおいしさが大きいもの以外をピックアップしたものを、おいしさが小さい方から順に、
	// 1つも選んでいないネタのうち、各ネタで最もおいしさが大きいものだけをピックアップしたものを、大きい方から順に、
	// 交換していく
	// 選ぶネタの種類を1つずつ増やしていくイメージ。
	priority_queue<ll, vector<ll>, greater<ll>> sel2;  // 2個以上選んでいるネタについて、2個目以降の選んでいるおいしさのリスト 小さい方から
	priority_queue<ll> nosel;  // 選んでいないネタで、もっともおいしい寿司のリスト 大きい方から
	for(i = 0; i < N; i++)
	{
		if(ch[i] >= 2)
		{
			for(j = 1; j < ch[i]; j++)
			{
				sel2.push(ta[i][j]);
			}
		}
		if(ch[i] == 0 && ta[i].size() > 0)
		{
			nosel.push(ta[i][0]);
		}
	}

	// sum, ki
	while(!sel2.empty() && !nosel.empty())
	{
		ll d1 = sel2.top();
		sel2.pop();

		ll d2 = nosel.top();
		nosel.pop();

		// sel2をnoselと入れ替え kiは1つ増える
		sum = sum-d1+d2;
		ki++;
		ll tmp = sum + ki*ki;
		chmax(ans, tmp);
	}
	cout << ans << endl;
	return 0;
}
