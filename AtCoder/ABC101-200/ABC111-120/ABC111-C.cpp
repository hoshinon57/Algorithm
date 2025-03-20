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

// ABC111 https://atcoder.jp/contests/abc111

/*
 * 実装が面倒だったやつ。各種解説と同じ方針。
 *   公式解説
 *   https://drken1215.hatenablog.com/entry/2019/12/17/130900
 *   https://blog.hamayanhamayan.com/entry/2018/09/30/002021
 * 
 * 基本は
 *   偶数番目の要素について、最頻値を残して、他はその値に合わせる
 *   奇数番目について同様
 * で良い。
 * 
 * しかしコーナーケースが厄介。サンプル3にあるのが親切。
 * ＞数列に現れる数はちょうど2種類
 * より、偶数/奇数番目に残す値は異なる必要があるので、
 * 最頻値が同じなら2番目に頻出するものを使う必要がある。
 * これの実装が面倒だった。
**/

int N;
vector<int> v;

// li[idx][*]  {個数,数値}のpairを個数大きい方から
void f(int idx, vector<vector<pair<int,int>>> &li)
{
	int i;
	const int MX = 100000;
	vector<int> cnt(MX+1);
	li[idx].resize(MX+1);
	for(i = 0; i < N; i++)
	{
		if(i%2 != idx) continue;
		cnt[v[i]]++;
	}

	for(i = 0; i <= MX; i++)
	{
		li[idx][i] = {cnt[i], i};
	}
	sort(li[idx].begin(), li[idx].end());
	reverse(li[idx].begin(), li[idx].end());  // 降順
}

int main(void)
{
	int i;
	cin >> N;
	v.resize(N);
	for(i = 0; i < N; i++) cin >> v[i];

	vector<vector<pair<int,int>>> li(2);
	f(0, li);
	f(1, li);

	int t;  // 残す数
	if(li[0][0].second != li[1][0].second)
	{
		t = li[0][0].first + li[1][0].first;
	}
	else
	{
		// 「2番目に頻出の値」を偶数/奇数番目どちらの要素で使うかを両方試す
		t = li[0][0].first + li[1][1].first;
		chmax(t, li[0][1].first + li[1][0].first);
	}
	cout << N-t << endl;

	return 0;
}
