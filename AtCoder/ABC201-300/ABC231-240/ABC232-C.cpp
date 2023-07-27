#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
using Graph = vector<vector<int>>;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC232 https://atcoder.jp/contests/abc232

/*
 * 順列全探索で解く。
 * 
 * 2人のおもちゃをgraph_1, graph_2とし、
 * graph_1[i][j]:i,jの2つのボールが紐で結ばれていれば1 と定義する。
 * 
 * 問題文にもある数列Pを用意し、next_permutation()を用いて全数列を列挙する。
 * graph_2のボールの番号をP[]の内容に読み替えたときに、graph_1,graph_2が一致すれば同じおもちゃとなる。
 * 一致するか否かは、各ボール同士の結びつきが完全一致するかどうかで判定すれば良い。
 */

// 1-indexed
int N, M;
Graph graph_1, graph_2;  // graph_1[i][j]:i,jの2つのボールが紐で結ばれていれば1

// graph_2のボールの番号をp[]の内容に読み替えたときに、graph_1,graph_2が一致するかどうか
// 一致するか否かは、各ボール同士の結びつきが完全一致するかどうかで判定する
bool check(vector<int> &p)
{
	for(int i = 1; i <= N; i++)
	{
		for(int j = 1; j <= N; j++)
		{
			// p[]は0-indexedのため-1が必要
			if(graph_1[i][j] != graph_2[p[i-1]][p[j-1]]) return false;
		}
	}

	return true;
}

int main(void)
{
	// 1-indexed
	int i;
	cin >> N >> M;
	graph_1.resize(N+1);
	graph_2.resize(N+1);
	for(i = 1; i <= N; i++)
	{
		graph_1[i].resize(N+1);
		graph_2[i].resize(N+1);
	}

	for(i = 1; i <= M; i++)
	{
		int a, b;
		cin >> a >> b;
		graph_1[a][b] = graph_1[b][a] = 1;
	}
	for(i = 1; i <= M; i++)
	{
		int a, b;
		cin >> a >> b;
		graph_2[a][b] = graph_2[b][a] = 1;
	}
	
	vector<int> p(N);  // {1,2,3,...,N}  これは順列なので要素番号は0-indexed
	for(i = 0; i < N; i++)  // 初期値設定 昇順に並べる
	{
		p[i] = i+1;
	}

	bool answer = false;
	do
	{
		if(check(p))  // 作成した順列を用いて判定
		{
			answer = true;
			break;
		}
	} while (next_permutation(p.begin(), p.end()));

	if(answer) cout << "Yes" << endl;
	else cout << "No" << endl;

	return 0;
}
