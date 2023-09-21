#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1e9+7;
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC173 https://atcoder.jp/contests/abc173

/*
 * コーナーケースに気を付けながら実装する。
 * 公式解説の解法2が近い。
 * https://img.atcoder.jp/abc173/editorial.pdf
 * 
 * 積を正にしたいが、どうやっても負になるケースが存在する。
 * これをコーナーケースとし、洗いだすと以下2パターン存在する。
 * (1)N=KかつAi<0が奇数個：
 *   Aを全てかける必要があるが、その結果が負になるケース。
 *   単純に全かけすればよい。
 * (2)Aiが全て負で、Kが奇数個：
 *   負しか選べないので、奇数個かけると負になるケース。
 *   絶対値が小さい方からK個かければよい。
 * 
 * 上記2つだけ先に処理すれば、後は必ず積を正にできる。
 * まずはpo_a, ne_aを以下のように準備する。
 *   po_a：A>=0となる要素 降順 (例：9, 6, 3, 0)
 *   ne_a：A<0 となる要素 昇順 (例：-7, -5, -3, -1)
 * po_aから2つ、ne_aから2つ選んだものの積はどちらも正になるので、
 * これが大きい方を採用していく。
 * Kが奇数のときはpo_aから先に1つ取り、Kを偶数にしてスタートさせる。
 * 
 * [ACまでの思考の流れ]
 * ・積を正にしたいはず。
 * ・負の要素は2つかければ正になるので、2個ずつ取っていくのが良いか。
 *   Kが奇数の場合はまず1つ取り、Kを偶数にしよう。
 * ・どうやっても積が負になるケースがありそう。
 *   これはコーナーケースとして別処理しよう。
 *   どういうときに積が負になるかを洗い出そう ⇒ 大変だった
 */

int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	vector<ll> po_a, ne_a;
	for(i = 0; i < N; i++)
	{
		ll tmp; cin >> tmp;
		if(tmp >= 0) po_a.push_back(tmp);
		else ne_a.push_back(tmp);
	}
	sort(po_a.begin(), po_a.end(), greater<ll>());  // A>=0の要素は降順
	sort(ne_a.begin(), ne_a.end());  // A<0の要素は昇順

	ll ans = 1;
	// まずはコーナーケース処理
	if(K%2==1 && po_a.size()==0)   // Kが奇数でAが全て負
	{
		// 絶対値が小さい方からK個かける
		reverse(ne_a.begin(), ne_a.end());
		for(i = 0; i < K; i++)
		{
			ans *= ne_a[i];
			ans %= MOD;
		}
		ans += MOD;  // 負になっているので0～1e9+6の範囲に移す
		cout << ans << endl;
		return 0;
	}
	if(N==K && ne_a.size()%2==1) // N=KかつA<0が奇数個、つまり全てかけて負になるケース
	{
		// 正負ともに全かけ
		for(auto &e : po_a)
		{
			ans *= e;
			ans %= MOD;
		}
		for(auto &e : ne_a)
		{
			ans *= e;
			ans %= MOD;
		}
		ans += MOD;  // 負になっているので0～1e9+6の範囲に移す
		cout << ans << endl;
		return 0;
	}

	// 以下、積が正になるケース
	// 正側、負側それぞれから2つずつ採用していく
	size_t po_idx = 0, ne_idx = 0;
	if(K%2 == 1)  // 奇数の場合、先に正側から1つ選んでおく
	{
		ans = po_a[po_idx++];
		K--;
	}
	while(K > 0)
	{
		if(po_idx+2 > po_a.size())  // 正側が残っていないので、負側を使う
		{
			ans *= (ne_a[ne_idx] * ne_a[ne_idx+1] % MOD);
			ans %= MOD;
			ne_idx += 2;
		}
		else if(ne_idx+2 > ne_a.size())  // 負側が残っていないので、正側を使う
		{
			ans *= (po_a[po_idx] * po_a[po_idx+1] % MOD);
			ans %= MOD;
			po_idx += 2;
		}
		else  // 両方残っている
		{
			ll tmp1 = po_a[po_idx] * po_a[po_idx+1];
			ll tmp2 = ne_a[ne_idx] * ne_a[ne_idx+1];
			// 正側の積と負側の積、大きい方を採用する
			if(tmp1 > tmp2)
			{
				ans *= tmp1 % MOD;
				ans %= MOD;
				po_idx += 2;
			}
			else
			{
				ans *= tmp2 % MOD;
				ans %= MOD;
				ne_idx += 2;
			}
		}
		K -= 2;
	}
	cout << ans << endl;

	return 0;
}
