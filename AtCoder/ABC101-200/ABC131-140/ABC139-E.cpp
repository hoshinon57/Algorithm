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

// ABC139 https://atcoder.jp/contests/abc139

/*
 * シミュレーションで解く感じ。
 * 
 * 各タイミングで選手の対戦相手は1人しかいないため、
 * 選手i,jが試合できる状態であれば、その日に試合するのが最善。
 * 
 * ただし毎日全メンバーの試合可否をチェックするとO(N)であり、
 * 1日に1試合しかできないような入力例だと、全体でO(N^3)となりTLEしそう。
 * (実際、そういう入力例があるらしい。解説動画のチャットコメントで見かけた)
 * 
 * そこで、「翌日の対戦リスト」を作っていくこととする。
 * これは「本日対戦した人」のみが候補となるので、それだけをチェックすれば計算量を改善できる。
 * ※初日だけはO(N)で対戦リストを作る必要がある
 * 
 * 当日の対戦リストが空になるまでシミュレーションし、
 * 最後に各選手が最後まで試合できていればOK.
 * 
 * [ACまでの思考の流れ]
 * ・試合の順番はどのような感じになるか手計算してみる
 * ・毎日、全メンバーの試合可否をチェックするとO(N)
 *   ⇒これだとTLEしそう
 *   ⇒翌日対戦できるのは、本日対戦した人が関わっている必要がありそう。これで計算量改善できないか
 */

int main(void)
{
	// 0-indexed
	int i, j;
	int N; cin >> N;
	vector<vector<int>> li(N, vector<int>(N-1, 0));  // li[i]:選手iの相手リスト
	vector<int> idx(N);  // idx[i]:選手iが次に戦う相手
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N-1; j++)
		{
			cin >> li[i][j];
			li[i][j]--;
		}
	}
	for(i = 0; i < N; i++) li[i].push_back(-1);  // 番兵

	vector<pair<int,int>> tdy;  // 本日の対戦リスト
	// 初日分は事前計算しておく
	for(i = 0; i < N; i++)
	{
		j = li[i][0];
		if(i<j && li[j][0]==i)
		{
			tdy.push_back({i, j});
		}
	}

	ll ans = 0;
	while(!tdy.empty())  // 今日の試合が1つでもあれば続行
	{
		ans++;
		vector<pair<int,int>> nxt;  // 明日の対戦リスト
		for(auto &e : tdy)
		{
			i = e.first; j = e.second;  // (i,j)が対戦
			idx[i]++;
			idx[j]++;
			int a = li[i][idx[i]];  // 選手iの次の相手
			if(a != -1 && li[a][idx[a]] == i)  // 試合マッチングするか
			{
				nxt.push_back({i, a});
			}
			a = li[j][idx[j]];  // 選手jの次の相手
			if(a != -1 && li[a][idx[a]] == j)
			{
				nxt.push_back({j, a});
			}
		}
		tdy = nxt;
	}

	for(i = 0; i < N; i++)
	{
		if(idx[i] != N-1) ans = -1;  // 試合が終わっていない選手がいればNG
	}
	cout << ans << endl;

	return 0;
}
