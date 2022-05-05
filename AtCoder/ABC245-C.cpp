#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC245 https://atcoder.jp/contests/abc245

/*
 * 条件を満たすX(i+1)が存在するかは、X(i)のみによって決まる。
 * X(i)=A(i) or B(i)なので、
 * A(i)がX(i)としての条件を満たすとき：
 *   |A(i)-A(i+1)|<=K でればA(i+1)は条件を満たす
 *   |A(i)-B(i+1)|<=K でればB(i+1)は条件を満たす
 * B(i)がX(i)としての条件を満たすとき：
 *   |B(i)-A(i+1)|<=K でればA(i+1)は条件を満たす
 *   |B(i)-B(i+1)|<=K でればB(i+1)は条件を満たす
 * として先頭から順に判定していけばよい。
 */
int main(void)
{
	ll i;
	ll N, K;
	cin >> N >> K;
	vector<ll> a(N), b(N);  // 0-indexed
	for(i = 0; i < N; i++) cin >> a[i];
	for(i = 0; i < N; i++) cin >> b[i];
	vector<bool> a_OK(N, false), b_OK(N, false);
	a_OK[0] = b_OK[0] = true;

	for(i = 0; i < N-1; i++)
	{
		if(a_OK[i])
		{
			if(abs(a[i]-a[i+1]) <= K) a_OK[i+1] = true;
			if(abs(a[i]-b[i+1]) <= K) b_OK[i+1] = true;
		}
		if(b_OK[i])
		{
			if(abs(b[i]-a[i+1]) <= K) a_OK[i+1] = true;
			if(abs(b[i]-b[i+1]) <= K) b_OK[i+1] = true;
		}
	}

	if(a_OK[N-1] || b_OK[N-1])
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}
