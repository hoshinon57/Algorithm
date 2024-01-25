#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:06 https://atcoder.jp/contests/typical90/tasks/typical90_f

/*
 * 辞書順最小は前から貪欲、という典型。
 *   https://twitter.com/e869120/status/1379202843622576130/photo/1
 * 
 * 以下、0-indexedで記載する。
 * 事前にid[i][*]に、i番目のアルファベットについて、それがsの中に登場する位置を記録しておく。
 * 
 * ans[i]を求めるにあたり、
 *   [ans[i-1]に使った文字の位置+1, N-K+i]
 * の閉区間における最も若いアルファベットを採用すればよい。（同じアルファベットがあれば、左側を選ぶ）
 * これを探すには、前述のid[]を用いてlower_bound()を使えば良い。
 * 計算量はアルファベットの種類26をMとし、O(NM*log(N))かな。
 */

int main(void)
{
	int i, j;
	int N, K; cin >> N >> K;
	string s; cin >> s;
	vector<vector<int>> id(26);  // id[i]:i番目のアルファベットについて、それがsの中に登場する位置
	for(i = 0; i < N; i++)
	{
		id[s[i]-'a'].push_back(i);
	}
	string ans;
	int st = 0;
	for(i = 0; i < K; i++)  // ans[i]を求める
	{
		for(j = 0; j < 26; j++)  // j番目のアルファベット
		{
			// j番目のアルファベットが、[st,N-K+i]の範囲に存在するか
			auto idx = lower_bound(id[j].begin(), id[j].end(), st);
			if(idx == id[j].end()) continue;
			if(*idx <= N-K+i)
			{
				ans += (j+'a');
				st = *idx + 1;
				break;
			}
		}
	}
	cout << ans << endl;

	return 0;
}
