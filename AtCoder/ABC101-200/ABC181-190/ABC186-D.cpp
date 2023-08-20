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

// ABC186 https://atcoder.jp/contests/abc186

/*
 * 累積和を使う。
 * 
 * 初めにAの総和sumを計算しておき、またAを昇順ソートしておく。
 * するとi<jについて|Ai-Aj|=Aj-Aiとなり、絶対値が外れる。
 * 
 * 各iについて (j:i+1～N)Σ|Ai-Aj| は、(A(i+1)～Anの総和)-(Ai*(N-1))となる。
 * これを答ansに加算し、(Ai～Anの総和)はsumから都度a[i]を引いていくことで求まる。
 * ※jをiスタートとしても結果は同じ。実装はそのようにした。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	ll sum = 0;  // Aの総和
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i]; sum += a[i];}
	sort(a.begin(), a.end());

	ll ans = 0;
	for(i = 0; i < N-1; i++)
	{
		ans += sum - a[i] * (N-i);
		sum -= a[i];
	}
	cout << ans << endl;

	return 0;
}
