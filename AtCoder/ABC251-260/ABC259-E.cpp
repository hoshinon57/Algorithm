#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC259 https://atcoder.jp/contests/abc259

/*
 * 公式解説と同じ考え方。
 *   https://twitter.com/kyopro_friends/status/1545767183208546304
 *   https://atcoder.jp/contests/abc259/editorial/4271
 * 
 * 1に書き換えないときのLCMを、デフォルトLCMとする。
 * 素数pごとに次数eを考えたときに、最も大きい次数がただ1つだけ存在すれば、
 * その次数を持つAiを1にしたときの最小公倍数 != デフォルトLCMとなる。
 * 
 * 逆に、
 * ・最も大きいただ1つの次数になるようなeを持たない
 * ・最も大きい次数だが、複数のAiがそのeを持つ
 * ようなAiは、1にしてもデフォルトLCMから変わらない。
 * 
 * よってAiごとに、最も大きいただ1つの次数を持てば答を+1する。
 * そのような次数が無ければデフォルトLCMになるが、
 * これはAi全体を通して1回のみカウントするように注意する。
 * (入力によっては、デフォルトLCMが答に登場しないケースもある)
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	map<int,vector<int>> p_list;  // p_list[p]:p^eであるeを入れていく
	vector<vector<pair<int,int>>> pe(N);  // pe[N][]:i番目のp,eの一覧
	for(i = 0; i < N; i++)
	{
		int m; cin >> m;
		for(j = 0; j < m; j++)
		{
			int p, e; cin >> p >> e;
			pe[i].push_back({p,e});
			p_list[p].push_back(e);
		}
	}

	map<int, int> e_max;  // e_max[p]:p^eにおいて、eの最大値 (最大値が複数ある場合は-1にしておく)
	for(auto &e : p_list)
	{
		sort(e.second.rbegin(), e.second.rend());
		if( (e.second.size() >= 2) && (e.second[0] != e.second[1]) )  // eが2つ以上あり、最大値がただ1つ
			e_max[e.first] = e.second[0];
		else if(e.second.size() == 1)  // eが1つ
			e_max[e.first] = e.second[0];
		else
			e_max[e.first] = -1;
	}

	int ans = 0;
	bool max_flag = false;  // デフォルトLCMが答に登場したか
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < (int)pe[i].size(); j++)
		{
			if(pe[i][j].second == e_max[pe[i][j].first])
			{
				ans++;
				break;
			}
		}
		if(j == (int)pe[i].size()) max_flag = true;  // Aiを1にしたときのLCMは、デフォルトLCM
	}
	if(max_flag) ans++;
	cout << ans << endl;
	
	return 0;
}
