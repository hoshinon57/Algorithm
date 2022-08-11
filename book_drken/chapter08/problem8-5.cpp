#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 章末問題8.5

/*
 * ハッシュテーブルunordered_setを用いて、a[0]～a[N-1]の値を登録しておく。
 * 次に各bについて、ハッシュテーブル内にbが存在するかどうかを判定し、
 * 存在すればa==bとなるようなa,bが存在することになる。これをカウンタで管理していけばよい。
 */
int main(void)
{
	int i;
	int N, M;
	cin >> N >> M;

	unordered_set<int> us;
	for(i = 0; i < N; i++)
	{
		int a;
		cin >> a;
		us.insert(a);
	}

	int answer = 0;
	for(i = 0; i < M; i++)
	{
		int b;
		cin >> b;
		if(us.count(b) == 1)  // a==bとなるような組み合わせがあるか
		{
			answer++;
		}
	}

	cout << answer << endl;

	return 0;
}
