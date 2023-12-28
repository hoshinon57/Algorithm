#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

int main(void)
{
	int i;
	int N, K; cin >> N >> K;
	vector<double> p(N);
	for(i = 0; i < N; i++)
	{
		int p_; cin >> p_;
		p[i] = (double)(p_+1)/2;
	}
	double sum = 0;
	for(i = 0; i < K; i++) sum += p[i];  // 先頭K個を先に加算
	double max = sum;
	for(i = K; i < N; i++)  // i番目を追加し、i-K番目を除外する
	{
		sum += p[i];
		sum -= p[i-K];
		chmax(max, sum);
	}
	cout << std::fixed << std::setprecision(15);
	cout << max << endl;

	return 0;
}
