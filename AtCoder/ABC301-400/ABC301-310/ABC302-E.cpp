#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC302 https://atcoder.jp/contests/abc302

/*
 * 愚直な考え方で、
 * 各頂点ごとに、繋がっている頂点をsetで管理していく。
 * 
 * クエリ1：
 *   uのsetが空であれば、答を-1する。setにvを追加する。
 *   v側も同様。
 * クエリ2：
 *   (vのsetが空であれば、そもそも何もしない)
 *   vのsetにある要素それぞれについて、その要素のsetからvを削除し、空集合になれば答を+1する。
 *   またvのぶん、答を+1する。
 * 
 * クエリ2で1回あたり大量の辺を削除しようとしてTLEになりそうだが、
 * 辺の追加はクエリ1で1回あたり1本なので、
 * 辺の削除は全クエリで高々Q回しか行われず、実行時間制限に十分間に合う。
 */
int main(void)
{
	int N, Q; cin >> N >> Q;
	vector<set<int>> v_list(N);  // v[i]:頂点iと結ばれている頂点の集合(set)

	int answer = N;
	while(Q > 0)
	{
		int kind; cin >> kind;
		if(kind == 1)
		{
			int u, v; cin >> u >> v;
			u--; v--;

			if(v_list[u].size() == 0) answer--;  // 頂点uの次数が0であれば、答から1減らす
			v_list[u].insert(v);

			if(v_list[v].size() == 0) answer--;
			v_list[v].insert(u);
		}
		else
		{
			int v; cin >> v;
			v--;
			if(v_list[v].size() != 0)  // そもそも頂点vの次数が0であれば、何もしない
			{
				for(auto &e : v_list[v])  // 頂点vと繋がっている頂点それぞれ
				{
					// 頂点vとの接続を削除し、次数が0になれば答を+1
					v_list[e].erase(v);
					if(v_list[e].size() == 0) answer++;
				}				
				v_list[v].clear();
				answer++;
			}
		}

		cout << answer << endl;
		Q--;
	}

	return 0;
}
