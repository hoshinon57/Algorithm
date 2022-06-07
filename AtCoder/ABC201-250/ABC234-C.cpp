#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC234 https://atcoder.jp/contests/abc234

/*
 * Kを2進数で表現したときに、1を2として表示すればよい。
 * Kを2で割った余りが2進数表記での最下位桁になるので、
 * 2で割りながら配列に追加していき、最後に逆順に表示する。
 */
int main(void)
{
	ll K;
	cin >> K;
	vector<int> a;

	while(K != 0)
	{
		if(K%2 == 0)
		{
			a.push_back(0);
		}
		else
		{
			a.push_back(2);
		}
		K/=2;
	}

	for(int i = a.size()-1; i >= 0; i--)
	{
		cout << a[i];
	}
	cout << endl;

	return 0;
}
