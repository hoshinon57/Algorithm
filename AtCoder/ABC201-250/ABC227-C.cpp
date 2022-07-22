#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC227 https://atcoder.jp/contests/abc227

/*
 * Bについて全探索する。
 * 
 * まずB<=Cのため、B*B<=Nの範囲で探索すればよい。
 * 次に各BについてA<=Bとなる範囲でAを探索する。
 * A,Bが決まったときに、B<=CかつC<=N/(A*B)となるので、
 * このときのCの組み合わせの数は N/(A*B)-(B-1) となる。
 * (Cの最大値 N/(A*B) に対して、B<=CなのでB未満のものを除外するという考え)
 * 
 * 想定解法ではAについて全探索しており、そちらの方が計算量は少ないと思われるが、
 * 本解法でも実行時間には十分余裕があった(505ms)。
 */
int main(void)
{
	ll N;
	cin >> N;
	ll answer = 0;
	for(ll b = 1; b*b <= N; b++)   // B<=Cなので、B*B<=Nの範囲で探索
	{
		for(ll a = 1; a <= b; a++)
		{
			if(a*b*b > N) break;  // 今のA,Bに対して条件を満たすCが存在しない
			ll tmp;
			tmp = N/a/b - (b-1);
			answer += tmp;
		}
	}

	cout << answer << endl;

	return 0;
}
