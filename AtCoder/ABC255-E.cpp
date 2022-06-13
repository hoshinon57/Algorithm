#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC255 https://atcoder.jp/contests/abc255

/*
 * 1-indexedで処理する。
 * 参考：https://twitter.com/kyopro_friends/status/1535619298399756288
 * 
 * A1+A2=S1より、A2=S1-A1となる。
 * 同様に、
 *   A3=S2-A2=S2-S1+A1
 *   A4=S3-A3=S3-S2+S1-A1
 *   A5=S4-A4=S4-S3+S2-S1+A1
 * よってsum[n]=S1-S2+S3-...+(or -)Sn とおくと、
 * An= (nが偶数)   sum[n-1]-A1    -> A1=sum[n-1]-An
 *     (nが奇数) -(sum[n-1]-A1)   -> A1=sum[n-1]+An
 * となる。
 * つまりAnをラッキーナンバーXnにするには、上記 -> の右式より
 *   nが偶数：A1=sum[n-1]-Xn
 *   nが奇数：A1=sum[n-1]+Xn
 * のようにA1を決めれば良い。
 * 
 * すると、A1～AnとX1～Xmの組み合わせにおいて、
 * 「各AをラッキーナンバーにするためにA1を何にすればよいか」
 * の表を作ることができる。
 * この表で最も多く登場したA1の個数が、求めるラッキーナンバーの最大個数となる。
 * (これは連想配列を使う)
 */
int main(void)
{
	// 1-indexed
	int i, j;
	int N, M;
	cin >> N >> M;
	vector<ll> S(N);  // S[1]～S[N-1]
	vector<ll> sum(N);  // sum[i]=S[1]-S[2]+S[3]-S[4]+ ... +(or-)S[i]
	for(i = 1; i <= N-1; i++)
	{
		cin >> S[i];
		// sum[]の計算にて、sum[0]=0という前提で処理を簡略化
		if(i%2 == 1)
		{
			sum[i] = sum[i-1]+S[i];
		}
		else
		{
			sum[i] = sum[i-1]-S[i];
		}
	}
	vector<ll> X(M+1);  // X[1]～X[M]
	for(i = 1; i <= M; i++)
	{
		cin >> X[i];
	}

	// A1～AnとX1～Xmの組み合わせにおいて、
	// 「各AをラッキーナンバーにするためにA1を何にすればよいか」の表を作る
	map<ll, ll> answer;  // answer[xx]:A1をxxにしたときのラッキーナンバーの個数
	for(i = 1; i <= M; i++)  // X[i]
	{
		for(j = 1; j <= N; j++)  // A[j]
		{
			if(j == 1)
			{
				// A[j]つまりA[1]をラッキーナンバーにするには、A[1]=X[i]にすればよい
				answer[X[i]]++;
			}
			else
			{
				// A[j]をラッキーナンバーにするには、A[1]を以下にすればよい
				// jが偶数：sum[j-1]-X[i]
				// jが奇数：sum[j-1]+X[i]
				ll tmp;
				if(j%2 == 0)
				{
					tmp = sum[j-1] - X[i];
				}
				else
				{
					tmp = sum[j-1] + X[i];
				}
				answer[tmp]++;
			}
		}
	}

/*
	ll ans_max = -INF64;
	ll ans_max_no;
	for(auto itr = answer.begin(); itr != answer.end(); itr++)
	{
		if(itr->second > ans_max)
		{
			ans_max_no = itr->first;
			ans_max = itr->second;
		}
	}
	cout << ans_max_no << " " << ans_max << endl;
*/
	ll ans_max = -INF64;
	for(auto &e : answer)
	{
		ans_max = max(ans_max, e.second);
	}
	cout << ans_max << endl;

	return 0;
}
