#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC123 https://atcoder.jp/contests/abc123

/*
 * A～Eの順番および大小関係にかかわらず、
 * この中の最小値（乗れる人数が最も少ないもの）がボトルネックとなる。
 * 
 * Aを最小値として、N=7, A=2のとき4分かかり、最速の1分から3分伸びる。
 * 都市1～6までの最速5分に、この伸びる3分を足した8分が回答となる。
 */
int main(void)
{
	ll N, a, b, c, d, e;
	cin >> N >> a >> b >> c >> d >> e;
	ll sizeMin;
	sizeMin = min(min(min(a, b), min(c, d)), e);

	ll waitTime;  // 最速の場合(1分)から伸びる時間
	if((N % sizeMin) == 0)
	{
		waitTime = (N/sizeMin)  -1;
	}
	else
	{
		waitTime = (N/sizeMin)+1-1;
	}

	cout << waitTime+5 << endl;

	return 0;
}
