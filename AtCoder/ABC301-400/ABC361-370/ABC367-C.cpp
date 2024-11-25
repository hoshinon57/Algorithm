#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC367 https://atcoder.jp/contests/abc367

// 再帰で条件を満たす数列を作っていくやつ

int N, K;
vector<int> r;
vector<vector<int>> ans;

void dfs(vector<int> &arr)
{
	int i;
	if((int)arr.size() == N)
	{
		if(reduce(arr.begin(), arr.end()) %K == 0)
		{
			ans.push_back(arr);
		}
		return;
	}

	int rr = r[arr.size()];
	for(i = 1; i <= rr; i++)
	{
		arr.push_back(i);
		dfs(arr);
		arr.pop_back();
	}
}

int main(void)
{
	int i;
	cin >> N >> K;
	r.resize(N);
	for(i = 0; i < N; i++) cin >> r[i];

	vector<int> arr;
	dfs(arr);

	for(auto &e1 : ans)
	{
		int sz_ = (int)e1.size();
		for(i = 0; i < sz_; i++) {
			cout << e1[i];
			if(i != sz_-1) cout << " ";
		}
		cout << endl;
	}

	return 0;
}
