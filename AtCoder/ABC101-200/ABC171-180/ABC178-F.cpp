#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC178 https://atcoder.jp/contests/abc178

/*
 * この問題はもうやりたくないな… (実装が簡単な解法を選んだだけで、他の解法を学ぶ気が起きない…)
 * 自力で解けず、解説を見た。
 * 
 * まず、N+1回以上登場する数値があればNo回答。
 * それ以外はYes回答になる。
 * 
 * Bを反転させると、A=Bとなる区間は最大で1つとなる。
 * その区間を交換していく。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> cnt(N+1);
	vector<int> a(N), b(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		cnt[a[i]]++;
	}
	for(i = 0; i < N; i++)
	{
		cin >> b[i];
		cnt[b[i]]++;
		if(cnt[b[i]] == N+1)
		{
			cout << "No" << endl;
			return 0;
		}
	}
	reverse(b.begin(), b.end());

	int st = -1, ed = -1;
	for(i = 0; i < N; i++)
	{
		if(a[i] == b[i])
		{
			if(st == -1) st = i;
		}
		if(st != -1 && a[i] != b[i]) break;
	}
	ed = i-1;
	// [st,ed]の範囲でA=Bとなる

	if(st != -1)
	{
		int idx = 0;
		int val = b[st];
		for(i = st; i <= ed; i++)  // [st,ed]
		{
			for(; idx < N; idx++)
			{
				if(a[idx] != val && b[idx] != val)
				{
					swap(b[idx], b[i]);
					idx++;
					break;
				}
			}
		}
	}
	cout << "Yes" << endl;
	int sz_ = (int)b.size();
	for(i = 0; i < sz_; i++) {
		cout << b[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}
