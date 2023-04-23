#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC299 https://atcoder.jp/contests/abc299

/*
 * BFSで解く。
 * 
 * 条件より、各p[i]について、
 *   p[i]からの距離がd[i]未満の頂点のうち、黒で塗られているもの
 * が存在したらNGとなる。
 * 
 * よってBFSを用いて各p[i]から全頂点への距離を算出し、
 *   (1)黒で塗ってはいけない頂点(d[i]未満)
 *   (2)黒で塗る候補(d[i]と等しい)
 * をリストアップする。
 * (1)は全頂点で共通、(2)は頂点ごとに保持する。
 * 
 * 次に各p[i]について、
 *   (2)の候補のうち、(1)には存在しないもの
 * を探す。
 * これが無ければNGと判定できる。
 * 
 * 計算量はBFS1回あたりO(N+M)で、全体でO(K(N+M)).
 * K,N,Mいずれも2000以下のため、十分に間に合う。
 */

using Graph = vector<vector<int>>;

string S;  // 回答文字列

bool solve(void)
{
	// 0-indexed
	int i, j;
	int N, M;
	cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int u, v;
		cin >> u >> v;
		u--;  v--;
		g[u].push_back(v);
		g[v].push_back(u);
	}
	int K;
	cin >> K;
	vector<int> p(K), d(K);
	for(i = 0; i < K; i++)
	{
		cin >> p[i] >> d[i];
		p[i]--;
	}

	// K=0はコーナーケース扱いとする
	// 「黒は1つ以上」の条件があるため、"100...00" の文字列を返す
	if(K == 0)
	{
		S += '1';
		for(i = 1; i < N; i++) S += '0';
		return true;
	}

	set<int> NG_list;  // 黒で塗ると条件を満たさなくなってしまう頂点
	vector<vector<int>> black_list(K);  // black_list[K][]  // 頂点p[i]からの距離がd[i]である頂点
	for(i = 0; i < K; i++)  // p[i]からBFS
	{
		queue<int> que;
		que.push(p[i]);
		vector<bool> seen(N, false);  // seen[i]:頂点iが探索済みならtrue
		vector<int> dist(N, 0);
		seen[p[i]] = true;
		dist[p[i]] = 0;
		while(!que.empty())
		{
			int v = que.front();
			que.pop();

			for(auto &e : g[v])
			{
				if(seen[e]) continue;
				seen[e] = true;
				dist[e] = dist[v]+1;
				que.push(e);
			}
		}

		// p[i]から各頂点への距離を調べて(dist[*])、
		//   d[i]と等しければ「黒にする候補」
		//   d[i]より小さければ「黒にしてはいけない頂点」
		// として保持する
		for(j = 0; j < N; j++)
		{
			if(dist[j] == d[i])
			{
				black_list[i].push_back(j);  // 黒にする候補
			}
			else if(dist[j] < d[i])
			{
				NG_list.insert(j);  // 黒にしてはいけない頂点
			}
		}
	}

	// 各p[i],d[i]について、黒にする頂点を選ぶ
	// ⇒black_list[K]のうち、NG_listに存在しないものを探す
	//   無ければNG
	vector<bool> black(N, false);  // 黒に塗るならtrue
	for(i = 0; i < K; i++)
	{
		bool check = false;
		for(auto &e : black_list[i])
		{
			if(NG_list.count(e) > 0) continue;
			black[e] = true;
			check = true;
			break;
		}
		if(!check) return false;  // 黒で塗れる頂点が無かった
	}

	// 回答文字列Sを構築
	for(i = 0; i < N; i++)
	{
		if(black[i])
			S += '1';
		else
			S += '0';
	}
	
	return true;
}

int main(void)
{
	if(solve())
	{
		cout << "Yes" << endl << S << endl;		
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
