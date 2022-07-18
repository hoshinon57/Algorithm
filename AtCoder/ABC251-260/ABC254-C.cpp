#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC254 https://atcoder.jp/contests/abc254

/*
 * 参考：https://twitter.com/kyopro_friends/status/1533082318763569152
 * Kずつしか動かせないバブルソートにて、全体をソート可能かという問題。
 * 
 * 数列Aを、K単位で分割して別の配列に保持しておく。
 * これをそれぞれソートし、K単位で合成する。
 * 合成結果が数列Aのソート結果と一致するかしないか、でYes/Noが決まる。
 */
int main(void)
{
	ll i;
	ll N, K;
	cin >> N >> K;
	vector<ll> a(N);
	vector<vector<int>> b(K);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		b[i%K].push_back(a[i]);  // 数列AをK単位で分割
	}

	sort(a.begin(), a.end());
	// 分割した数列を、それぞれソート
	for(i = 0; i < K; i++)
	{
		sort(b[i].begin(), b[i].end());
	}

	for(i = 0; i < N; i++)
	{
		// 分割したのを合成し、それがA全体のソート結果と一致するか
		if(a[i] != b[i%K][i/K])
		{
			cout << "No" << endl;
			return 0;
		}
	}
	cout << "Yes" << endl;

	return 0;
}
