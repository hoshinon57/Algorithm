#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC361 https://atcoder.jp/contests/abc361

/*
 * BFSで解く。
 * 
 * 各種解説の通りで、盤面を文字列で持つ。
 * 遷移は2文字を空白位置と交換する形となる。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	string s, t; cin >> s >> t;
	s += "..";  // 空白文字を末尾に付与
	t += "..";

	map<string, int> dist;  // dist["str"] = d;
	dist[s] = 0;
	queue<string> que;
	que.push(s);

	while(!que.empty())
	{
		auto n = que.front();
		que.pop();
		int d = dist[n];

		int sp = n.find('.');  // ".."の左側
		for(i = 0; i < N+1; i++)  // n[]のi,i+1を".."へ移動
		{
			if(n[i] == '.' || n[i+1] == '.') continue;
			string tmp = n;
			swap(tmp[i], tmp[sp]);
			swap(tmp[i+1], tmp[sp+1]);
			if(dist.count(tmp) != 0) continue;
			que.push(tmp);
			dist[tmp] = d+1;
		}
	}
	int ans;
	if(dist.count(t) == 0) ans = -1;
	else ans = dist[t];
	cout << ans << endl;

	return 0;
}
