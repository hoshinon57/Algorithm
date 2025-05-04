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

// siを全探索で最初考えたがTLE. 計算量の見積もりがダメだった。
// 公式解説の通り、siの長さを全探索して、矛盾が発生しないかで実装する。

int K, N;
vector<string> v, w;
vector<int> len;  // len[i]:siの文字列長

// idx:次はs[idx]を作る
void rec(int idx)
{
	int i, j;
	if(idx == K)
	{
		vector<string> s(K);
		for(i = 0; i < N; i++)
		{
			int p = 0;
			for(auto &e : v[i])
			{
				j = e-'1';
				// pからlen[j]文字
				if(p >= (int)w[i].size()) return;  // 矛盾 当初これを入れずREだった
				string tmp = w[i].substr(p, len[j]);
				if(s[j] == "") s[j] = tmp;  // 初回
				else
				{
					if(s[j] != tmp) return;  // 矛盾
				}
				p += len[j];
			}
			if(p != (int)w[i].size()) return;  // 矛盾
		}
		// 成功
		for(auto &e : s) cout << e << endl;
		exit(0);
	}

	for(i = 1; i <= 3; i++)  // siの文字列長は1～3
	{
		len[idx] = i;
		rec(idx+1);
	}
}

int main(void)
{
	int i;
	cin >> K >> N;
	v.resize(N);
	w.resize(N);
	len.resize(K);
	for(i = 0; i < N; i++)
	{
		cin >> v[i] >> w[i];
	}
	rec(0);

	return 0;
}
