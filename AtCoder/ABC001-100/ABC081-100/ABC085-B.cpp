#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC085 https://atcoder.jp/contests/abc085/
// AtCoder Beginners Selectionより

int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<int> d(N);
	for(i = 0; i < N; i++) cin >> d[i];
	sort(d.begin(), d.end(), greater<int>());  // 降順

	int prev = -1;
	int answer = 0;
	for(i = 0; i < N; i++)
	{
		if(d[i] != prev)
		{
			prev = d[i];
			answer++;
		}
	}

	cout << answer << endl;

	return 0;
}
