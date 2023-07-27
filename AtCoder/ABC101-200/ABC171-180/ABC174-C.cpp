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

// ABC174 https://atcoder.jp/contests/abc174

/*
 * 数列について、ある数Xの次は X*10+7 で表すことができる。
 * これをKで割った余りが0になったときが答。
 * 一方で余りが「以前にも登場した値」であればそこでループしているため、Kの倍数は存在しない。
 */
int main(void)
{
	int K;
	cin >> K;
	vector<bool> m(K, false);  // m[i]:余りがiになる数が登場したらtrue

	int mod = 0;
	int count = 1;
	while(true)  // 最大でもK回のループ
	{
		mod = mod*10 + 7;
		mod %= K;  // mod K で処理してよい
		if(mod == 0)
		{
			cout << count << endl;
			break;
		}

		if(m[mod])  // 余りがmodになるような値は過去に登場した よってKの倍数になることは無い
		{
			cout << -1 << endl;
			break;
		}
		m[mod] = true;

		count++;
	}

	return 0;
}
