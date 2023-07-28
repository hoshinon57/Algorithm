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

// ABC134 https://atcoder.jp/contests/abc134

/*
 * 大きい側から考える。
 * 
 * 数値iについて、
 *   iを除くiの倍数の箱に入っているボールの和、を2で割った余り
 * を求め、それとa[i]が不一致ならば箱iにはボールを入れる、一致するならば箱iにボールを入れない、とすればよい。
 * 
 * ※以下の解説にあった"パリティ"という表現がしっくりきた。
 *   https://blog.hamayanhamayan.com/entry/2019/07/20/233440
 * 
 * 調和級数になるので、計算量はO(NlogN)となり十分間に合う。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<int> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}
	vector<int> ball(N+1, 0);  // ball[i]:i番目の箱にボールを入れるなら1

	// 上から決めていく
	int cnt = 0;
	for(i = N; i > 0; i--)
	{
		int sum = 0;
		for(j = i+i; j <= N; j+=i)  // i自身を除くiの倍数の和
		{
			sum += ball[j];
		}
		sum %= 2;
		ball[i] = (sum+a[i])%2;
		if(ball[i] == 1) cnt++;
	}
	
	cout << cnt << endl;
	for(i = 1; i <= N; i++)
	{
		if(ball[i] == 1) cout << i << endl;
	}

	return 0;
}
