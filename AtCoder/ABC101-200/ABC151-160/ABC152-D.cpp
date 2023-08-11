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

// ABC152 https://atcoder.jp/contests/abc152

/*
 * cnt[x][y]:先頭の桁がx, 末尾の桁がyとなるN以下の数値の種類数
 * と定義し、事前計算しておく。
 * するとAについて全探索したときに、
 *   各A=iについて、iの先頭/末尾の桁をそれぞれhead,tailとすると
 *   条件を満たすBの種類数はcnt[tail][head]と等しい
 * が成り立つ。
 * これをA=1～Nまで加算していったものが答。
 * 
 * ※Aについて全探索するのではなく、
 *   cnt[head][tail] * cnt[tail][head]
 *   をhead,tailともに0～9まで2重ループさせたときの総和でも答となる。
 */

// 数値nの先頭の桁をheadに、末尾の桁をtailに入れる
void check(int n, int &head, int &tail)
{
	tail = n % 10;
	while(n >= 10) {n /= 10;}
	head = n;
}

// cnt[x][y]:先頭の桁がx, 末尾の桁がyとなるN以下の数値の種類数
int cnt[10][10] = {0};

int main(void)
{
	int i;
	int N; cin >> N;
	int head, tail;
	for(i = 1; i <= N; i++)
	{
		// 例えばi=4567であれば、cnt[4][7]を+1する
		check(i, head, tail);
		cnt[head][tail]++;
	}

	ll ans = 0;
	for(i = 1; i <= N; i++)
	{
		check(i, head, tail);
		ans += cnt[tail][head];  // A=iに対し、条件を満たすBの種類数はcnt[tail][head]となる
	}
	cout << ans << endl;

	return 0;
}
