#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC128 https://atcoder.jp/contests/abc128

/*
 * 全探索する。
 * ※公式解説やhamayanhamayan氏の考え方と同じだった。
 *   https://img.atcoder.jp/abc128/editorial.pdf
 *   https://blog.hamayanhamayan.com/entry/2019/06/03/203333
 * 
 * 取る宝石の数(0～min(K,N)個)で全探索する。ここで、正負は問わない。
 * これをkとすると、左側/右側から何個ずつ取るかでk+1パターンある。ここでも全探索する。
 * 取った宝石には負のものも含まれるが、これを(K-k)回の範囲で返す。
 * 
 * 以上の操作を各全探索について行い、最大値が答となる。
 */
int main(void)
{
	int i, j, k;
	int N, K; cin >> N >> K;
	vector<int> v(N); for(i = 0; i < N; i++) {cin >> v[i];}

	int ans = 0;
	for(k = 0; k <= min(K, N); k++)  // k個取る
	{
		for(i = 0; i <= k; i++)  // 左側からi個、右側から(k-i)個取る   全k+1パターン
		{
			int sum = 0;
			set<int> minus;  // 負の宝石のリスト
			for(j = 0; j < i; j++)  // 左側からi個取る
			{
				sum += v[j];
				if(v[j] < 0) minus.insert(v[j]);
			}
			for(j = N-1; j > (N-1)-(k-i); j--)  // 右側から(k-i)個取る
			{
				sum += v[j];
				if(v[j] < 0) minus.insert(v[j]);
			}
			// 返せる数だけ負の宝石を返す
			for(j = 0; j < K-k; j++)
			{
				if(minus.size() == 0) break;
				int c = *minus.begin();
				if(c >= 0) break;  // 負の宝石が無くなった
				sum -= c;  // 価値の総和を更新
				minus.erase(c);
			}
			ans = max(ans, sum);
		}
	}
	cout << ans << endl;

	return 0;
}
