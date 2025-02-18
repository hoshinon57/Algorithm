#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

#include <atcoder/modint>
using namespace atcoder;
using mint = modint1000000007;

// ABC122 https://atcoder.jp/contests/abc122

/*
 * ほぼhamayanhamayan氏の解法。
 *   https://blog.hamayanhamayan.com/entry/2019/03/25/232610
 * 
 * キーをstringにしてDPで解く。
 * 隣接文字をswapする操作があるため、直近4文字の情報がほしい。
 * そのためDPのキーとして直近(4-1)文字を持ってDPする。(欲しい情報数-1でDPする典型)
 * 
 * dp[str]=valとして持つ。
 * strは末尾の3文字、valはそのときの合法数。
 * 最終的に、dp[*]の総和が答。
 * 
 * DP遷移は、dpの各要素に"ACGT"の文字を付与し、
 *   それが合法かチェック
 *   付与した文字列の末尾3文字を取り出し、ndp[ns] += dp[s]
 * とする。
 * 
 * 計算量を考えてみる。
 *   文字数:N=100
 *   状態数:文字種が4, 文字数が3なので4*4*4=64
 *   遷移数:4
 * 以上の積が25600, これにmapのlogとチェック関数があるが、mapの状態数は少ないし十分に間に合う。
 * 間に合わなければチェック関数をメモ化にする形か。
**/

// 合法ならtrue
bool check(string s)
{
	string agc = "AGC";
	// そのままの状態でAGCがあるかチェック
	if(s.substr(0, 3) == agc) return false;
	if(s.substr(1, 3) == agc) return false;

	// 順にswapさせてAGCがあるかチェック
	int i;
	for(i = 0; i <= 2; i++)
	{
		string tmp = s;
		swap(tmp[i], tmp[i+1]);
		if(tmp.find(agc) != string::npos) return false;
	}

	return true;
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	map<string, mint> dp;
	dp["TTT"] = 1;

	string acgt = "ACGT";
	for(i = 0; i < N; i++)
	{
		map<string, mint> ndp;
		for(auto [s,val] : dp)  // s="AGC"といった文字列
		{
			for(j = 0; j < (int)acgt.size(); j++)
			{
				string tmp = s+acgt[j];
				if(!check(tmp)) continue;
				string ns = tmp.substr(1, 3);  // 後半3文字
				ndp[ns] += dp[s];
			}
		}
		swap(ndp, dp);
	}

	mint ans;
	for(auto [key,val] : dp)
	{
		ans += val;
	}
	cout << ans.val() << endl;

	return 0;
}
