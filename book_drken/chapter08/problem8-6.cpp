#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題8.6

/*
 * ハッシュテーブルを用いた連想配列unordered_mapを使う。
 * まずa[0]～a[N-1]までの入力時に、それぞれの値が出た回数をカウントしておく。
 * 次に各bについて、連想配列内にbが存在するかどうかを判定し、存在すればその数だけanswerを増やす。
 * 最終的なanswerが求める個数となる。
 */
int main(void)
{
	int i;
	int N, M;
	cin >> N >> M;

	unordered_map<int,int> um;
	for(i = 0; i < N; i++)
	{
		int a;
		cin >> a;
		um[a]++;
	}

	int answer = 0;
	for(i = 0; i < M; i++)
	{
		int b;
		cin >> b;
		if(um.count(b) == 1)
		{
			answer += um[b];
		}
	}

	cout << answer << endl;

	return 0;
}
