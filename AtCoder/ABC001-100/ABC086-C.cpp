#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC086 https://atcoder.jp/contests/abc086
// AtCoder Beginners Selectionより

int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<int> t(N+1, 0), x(N+1, 0), y(N+1, 0);  // 時刻0に(0, 0)になるようにしておく
	for(i = 1; i <= N; i++)
	{
		cin >> t[i] >> x[i] >> y[i];
	}
	
	// 時刻t[0] 位置(x[0], y[0])からt[1] (x[1], y[1])に行けるか
	for(i = 0; i < N; i++)
	{
		int diffT = t[i+1] - t[i];
		int dist = abs(x[i+1]-x[i]) + abs(y[i+1]-y[i]);
		// まず、次の地点への距離に対して時間が足りなければNG.
		if(dist > diffT)
		{
			cout << "No" << endl;
			return 0;
		}

		// 次に、距離が足りていても、距離と時間差の偶奇が不一致ならばNG.
		// その場にとどまることはできないため。
		if( (dist+diffT)%2 != 0)
		{
			cout << "No" << endl;
			return 0;
		}
	}

	cout << "Yes" << endl;

	return 0;
}
