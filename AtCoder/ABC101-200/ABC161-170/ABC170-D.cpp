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

// ABC170 https://atcoder.jp/contests/abc170

/*
 * エラトステネスの篩的な方針で解く。
 *   https://blog.hamayanhamayan.com/entry/2020/06/14/232009
 * 
 * ※最初、
 *   ・Aを昇順ソートしておく
 *   ・各Aについて約数列挙し、約数がAの中に存在すれば条件を満たさない
 *   という方針でACした。
 *   https://atcoder.jp/contests/abc170/submissions/47654434
 *   ただしこの方針は約数列挙にO(√A)かかるため、全体でO(N√A)となり、10^8オーダーでTLEの可能性がある。
 * 
 * 本ソースコードでは以下の方針で解いた。
 * ・Aを昇順ソートしておく。
 * ・a[i]に対してa[i]*2, a[i]*3, a[i]*4, ... となるようなa[j]は割り切れてしまい、条件を満たさない。
 * ・上記は「同じ値が2回以上出る」場合のケアができないので、
 *   Aの値の登場回数をカウントしておき、2回以上登場したらそのa[i]は条件を満たさないとする。
 * ★調和級数より、1/1+1/2+1/3+...+1/N=logN になる。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<int> a(N), a_cnt(1e6+5);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
	}
	sort(a.begin(), a.end());

	vector<bool> ok(1e6+5, true);
	for(i = 0; i < N; i++)
	{
		if(!ok[a[i]]) continue;
		if(++a_cnt[a[i]] >= 2)  // Aに同じ数が2回以上出たら、そのAは条件を満たさない
		{
			ok[a[i]] = false;
			continue;
		}
		for(j = a[i]*2; j <= 1e6+1; j += a[i])
		{
			ok[j] = false;
		}
	}

	int ans = 0;
	for(i = 0; i < N; i++) { if(ok[a[i]]) ans++; }
	cout << ans << endl;

	return 0;
}
