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

// ABC233 https://atcoder.jp/contests/abc233

/*
 * 累積和で解く。
 * 
 * X=12345のとき、以下の加算を行うことになる。
 *    12345
 *     1234
 *      123
 *       12
 *   +    1
 *   ------
 * 
 * Xの各桁の値を、上位の桁から順にnum[0], num[1], ... とすると、
 * 上の桁から順に
 *   num[0]
 *   num[0]+num[1]
 *   num[0]+num[1]+num[2]
 *   num[0]+num[1]+num[2]+num[3] ...
 * となっている。
 * これは累積和を使って求めていけばよい。
 * 
 * 繰り上がりがあるため、累積和の計算の後に下の桁から計算していく。
 * Xの先頭に"0"を追加しておくと、最上位の桁の繰り上がりを気にする必要がなくなり、実装が簡単になる。
 */

int main(void)
{
	ll i;
	string X;
	cin >> X;
	X = "0" + X;  // 繰り上がりの実装を簡易にするため、先頭に0を付与しておく
	ll size = X.size();

	vector<int> num(size);  // num[i]:Xのi文字目の値
	for(i = 0; i < size; i++)
	{
		num[i] = (int)(X[i] - '0');
	}

	vector<ll> sum(size);  // sum[i]:num[0]～num[i]の累積和
	sum[0] = num[0];
	for(i = 1; i < size; i++)  // 上の桁から
	{
		sum[i] = sum[i-1] + num[i];
	}

	// 繰り上がりの計算
	// 10で割った値を上の桁へ、余りを今の桁へ
	for(i = size-1; i > 0; i--)  // 下の桁から
	{
		sum[i-1] += sum[i]/10;
		sum[i] %= 10;
	}
	// i=0は付与した"0"の桁であるため、繰り上がり計算は不要

	// 最上位の桁から1文字ずつ出力
	for(i = 0; i < size; i++)
	{
		if(i == 0 && sum[i] == 0) continue;  // 先頭が0なら出力しない
		cout << sum[i];
	}
	cout << endl;

	return 0;
}
