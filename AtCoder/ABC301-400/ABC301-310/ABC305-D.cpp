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

// ABC305 https://atcoder.jp/contests/abc305

/*
 * 累積和で解く。
 * 以下、睡眠の開始～終了を「睡眠ブロック」と表現する。
 * 
 * sum[i]を、i回目の睡眠ブロックの終わりまでに、トータルでの睡眠時間と定義する。
 * (1-indexedで考える)
 * 
 * f(x)を、x分後までにトータルで寝ていた時間とすると、
 * x分後の時点が
 *   起床中：1つ前の睡眠ブロックまでの総和。sumで求められる。
 *   睡眠中：1つ前の睡眠ブロックまでの総和と、今の区間の睡眠時間の和
 * にてf(x)を求められる。
 * ※起床中か睡眠中かは、Aをlower_bound()で求められる。
 * 
 * クエリごとにf(r)-f(l)が答。
 */
int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<ll> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
	}
	vector<ll> sum = {0};  // 1-indexedにするため0を入れておく
	for(i = 1; i <= N/2; i++)
	{
		ll d = a[i*2] - a[i*2-1] + sum[i-1];  // 例えば2番目の睡眠ブロックは、a[3]～a[4]の区間
		sum.push_back(d);
	}

	int Q;
	cin >> Q;
	while(Q > 0)
	{
		ll l, r;
		cin >> l >> r;

		// num分目までの睡眠時間を返す
		auto calc = [&](ll num) -> ll
		{
			auto idx = lower_bound(a.begin(), a.end(), num) - a.begin();
			if(idx == 0) return 0;  // idx=0は個別処理

			if(idx%2 == 1)  // 起床中
			{
				return sum[idx/2];  // 1つ前の睡眠ブロックまでの、トータル睡眠時間
			}
			else  // 睡眠中
			{
				ll ret = sum[idx/2 - 1];  // 1つ前の睡眠ブロックまでの、トータル睡眠時間
				ret += num - a[idx-1];  // 今回の睡眠ブロックぶん
				return ret;
			}
		};
		cout << calc(r) - calc(l) << endl;

		Q--;
	}

	return 0;
}
