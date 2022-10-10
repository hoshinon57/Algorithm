#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題4
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/pakencamp-2019-day3/tasks/pakencamp_2019_day3_c

/*
 * 全探索で解く。
 * 
 * ある2曲を選んだ時に、各人のスコア(1曲目と2曲目のうち高い方)を加算していくと、
 * その合計がグループの得点となる。
 * 
 * 2曲の選び方を2重forで全列挙し、
 * それぞれについて上記のグループの得点を計算すると、その最大値が答となる。
 * 
 * 計算量はO(NM^2).
 */
int main(void)
{
	int i, j, k;
	int N, M;
	cin >> N >> M;
	vector<vector<int>> score(N, vector<int>(M));
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < M; j++)
		{
			cin >> score[i][j];  // 人iが、曲jを歌うときのスコア
		}
	}

	ll max_score = 0;
	for(i = 0; i < M; i++)
	{
		for(j = i+1; j < M; j++)  // 曲iと曲jを歌う
		{
			ll tmp = 0;
			for(k = 0; k < N; k++)  // 各人kに対して、曲iと曲jのうち高い方のスコアを求める
			{
				tmp += max(score[k][i], score[k][j]);
			}
			max_score = max(max_score, tmp);
		}
	}
	cout << max_score << endl;

	return 0;
}
