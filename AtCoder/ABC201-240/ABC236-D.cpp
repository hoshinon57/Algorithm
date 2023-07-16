#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC236 https://atcoder.jp/contests/abc236

/*
 * 再帰を用いた深さ優先探索で解く。
 * 
 * ペアの1人目を、まだ選ばれていない人のうち、最も若い番号の人とする。(1通り)
 * ペアの2人目を、まだ選ばれていない人のうちから1人選ぶ。
 * すると組み合わせの数はN=8の最大時に 15*13*11*...*3*1 となる。
 * またこのとき、「ペアの1人目の番号」<「ペアの2人目の番号」が成り立つ。
 */

int N;
vector<vector<int>> pairScore;
int maxScore = -1;

void func(vector<int> &a)
{
	int i;

	if(a.size() == N*2)  // ペアを全て作成した
	{
		int score = 0;
		for(i = 0; i < a.size(); i+=2)
		{
			score = score ^ pairScore[a[i]][a[i+1]];
		}
		maxScore = max(maxScore,score);
		return;
	}

	vector<bool> notUse(N*2, true);  // まだ選ばれていないならtrue
	for(auto &e : a) notUse[e] = false;

	// ペアの1人目として、まだ選ばれておらず、かつ最も若い番号を選ぶ
	for(i = 0; i < N*2; i++)
	{
		if(notUse[i])
		{
			notUse[i] = false;
			a.push_back(i);
			break;
		}
	}

	// ペアの2人目は、まだ選ばれていない人から選ぶ
	for(i = 0; i < N*2; i++)
	{
		if(!notUse[i]) continue;
		a.push_back(i);
		func(a);
		a.pop_back();
	}

	// ペアの1人目の削除
	a.pop_back();

	return;
}

int main(void)
{
	int i, j;
	cin >> N;
	pairScore.resize(N*2);
	for(int i = 0; i < N*2; i++)	pairScore[i].resize(N*2);

	for(i = 0; i < N*2; i++)
	{
		for(j = i+1; j < N*2; j++)	cin >> pairScore[i][j];
	}

	vector<int> a;
	func(a);

	cout << maxScore << endl;

	return 0;
}
