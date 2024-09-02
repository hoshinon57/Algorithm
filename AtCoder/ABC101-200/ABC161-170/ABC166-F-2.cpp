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

// ABC166 https://atcoder.jp/contests/abc166

/*
 * ABC166-F.cppで書いた以下方針を実装した。
 * -------------------
 *   A+B+C=0はNG
 *   A+B+C=1は現在のクエリを愚直にやるしかない
 *   A+B+C>=2の場合、
 *     初手で(0,0)のNGは仕方ない
 *     (1,1)のとき、1つ先のクエリを読んで足し引きする方に加算
 *     それ以外は大小関係で決めればうまくいく
 * -------------------
 * 
 * また、A,B,Cの値をcnt[3]にしたり、
 * クエリABならx[que][*]={0,1}にしたりして、実装をA,B,Cで共通化を図ってみた。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> cnt(3);  // {A,B,C}の値
	cin >> cnt[0] >> cnt[1] >> cnt[2];
	string out = "ABC";
	vector<vector<int>> x(N, vector<int>(2));  // x[query][2] = "AB"なら{0,1}, "BC"なら{1,2}, "AC"なら{2,0}
	for(i = 0; i < N; i++)
	{
		string s; cin >> s;
		if(s == "AB")
		{
			x[i][0] = 0; x[i][1] = 1;
		}
		else if(s == "BC")
		{
			x[i][0] = 1; x[i][1] = 2;
		}
		else
		{
			x[i][0] = 2; x[i][1] = 0;
		}
	}

	string ans;
	// n0に相当する文字を足し、n1に相当する文字を減らす
	auto upd = [&](int n0, int n1) -> void
	{
		cnt[n0]++;
		cnt[n1]--;
		ans += out[n0];
	};
	for(i = 0; i < N; i++)
	{
		int x0 = x[i][0], x1 = x[i][1];
		if(cnt[x0] == 0 && cnt[x1] == 0)
		{
			ans = "No";
			break;
		}
		if(cnt[x0] == 0 && cnt[x1] == 1)  // {0,1}ケース x0側を足すのみ
		{
			upd(x0, x1);
		}
		else if(cnt[x0] == 1 && cnt[x1] == 0)  // {1,0}ケース
		{
			upd(x1, x0);
		}
		else if(cnt[x0] == 1 && cnt[x1] == 1 && i != N-1)  // {1,1}ケース 1手先読み
		{
			// 1手先のクエリで足し引きする方
			if(x0 == x[i+1][0] || x0 == x[i+1][1]) upd(x0, x1);
			else upd(x1, x0);
		}
		else
		{
			// 大きい方を減らす感じで適当に
			// {1,1}での最終ターンもこっち
			if(cnt[x0] < cnt[x1]) upd(x0, x1);
			else upd(x1, x0);
		}
	}

	if(ans != "No")
	{
		cout << "Yes" << endl;
		for(auto &c : ans) cout << c << endl;
	}
	else cout << ans << endl;

	return 0;
}
