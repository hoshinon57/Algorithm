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

// ABC160 https://atcoder.jp/contests/abc160

/*
 * 公式解説での解法。i,jについて全探索する。
 * 
 * i<jとなる2頂点(i,j)の距離は、
 *   X->Yを使わないパターン
 *   X->Yを使うパターン
 * のどちらかとなるので、それぞれを計算して小さい方を採用する。
 * ※i->X->Y->j, i->Y->X->jの2通りを計算したが、後者は不要だったと考える。
 *   制約よりX<Yであるため。
 */
int main(void)
{
	int i, j;
	int N, X, Y; cin >> N >> X >> Y;
	X--; Y--;
	vector<int> ans(N);  // ans[i]:最短距離がiとなるような頂点の組の数
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)  // 頂点i,j間の距離  i<j
		{
			// (X,Y)間の経路を使わないパターンと、使うパターン
			int d1 = j - i;
			int d2 = abs(X-i) + 1 + abs(j-Y);  // i->X->Y->j
			int d3 = abs(Y-i) + 1 + abs(j-X);  // i->Y->X->j
			ans[min({d1, d2, d3})]++;
		}
	}
	
	for(i = 1; i <= N-1; i++)
	{
		cout << ans[i] << endl;
	}

	return 0;
}
