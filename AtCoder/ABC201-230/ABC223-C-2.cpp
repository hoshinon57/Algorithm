#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC223 https://atcoder.jp/contests/abc223

/*
 * 公式解説を参考に、シミュレーションで解く。
 *   https://atcoder.jp/contests/abc223/editorial/2773
 * 
 * まず、火が左側のみに着いた場合に、燃え尽きるまでの時間を計算する。
 * これを2で割った時間が、左右から着火したときに燃え尽きるまでの時間となる。
 * あとはこの時刻になるまで左側からシミュレーションすればよい。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<double> a(N), b(N);
	double t = 0;
	for(i = 0; i < N; i++)
	{
		cin >> a[i] >> b[i];
		t += a[i]/b[i];
	}
	t /= 2;  // これが、導火線が燃え尽きるまでの時間となる

	// 左側からシミュレーション
	double ans = 0;
	for(i = 0; i < N; i++)
	{
		if(a[i]/b[i] < t)
		{
			ans += a[i];
			t -= a[i]/b[i];
		}
		else
		{
			ans += t*b[i];
			break;
		}
	}
	cout << std::fixed << std::setprecision(15);
	cout << ans << endl;

	return 0;
}
