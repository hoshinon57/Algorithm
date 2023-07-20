#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC287 https://atcoder.jp/contests/abc287

/*
 * 累積和的な考え方で解く。
 * 
 * head[i]:Sの先頭i文字とTの先頭i文字が一致すればtrue
 * tail[i]:Sの末尾i文字とTの末尾i文字が一致すればtrue
 * と定義する。
 * どちらもサイズは |T|+1 となる。
 * 
 * head[]については、S,Tの先頭から1文字ずつ判定していくことで、全体でO(|T|)で判定できる。
 * このとき、x文字目の判定で不一致になれば、(x+1)文字目以降は全て不一致になることに留意する。
 * tail[]についても、S,Tをreverse()させて同様に先頭から判定する。
 * ここまでが前準備。
 * 
 * x=0, 1, ..., |T| の各問題については、
 * head[x]とtail[|T|-x]の両方がtrue(一致)であればYes, それ以外はNo回答となる。
 */

int main(void)
{
	int i;
	string S, T; cin >> S >> T;
	vector<bool> head(T.size()+1, false), tail(T.size()+1, false);
	head[0] = true;
	for(i = 1; i <= (int)T.size(); i++)  // 先頭i文字
	{
		if(S[i-1] == T[i-1]) head[i] = true;
		else if(S[i-1] == '?' || T[i-1] == '?') head[i] = true;  // どちらかが'?'なら一致できる
		else break;  // いったん不一致になれば、今後もずっと不一致
	}

	tail[0] = true;
	reverse(S.begin(), S.end());
	reverse(T.begin(), T.end());
	for(i = 1; i <= (int)T.size(); i++)  // 末尾i文字
	{
		if(S[i-1] == T[i-1]) tail[i] = true;
		else if(S[i-1] == '?' || T[i-1] == '?') tail[i] = true;
		else break;  // いったん不一致になれば、今後もずっと不一致
	}
	
	for(i = 0; i <= (int)T.size(); i++)
	{
		string ans = "No";
		if(head[i] && tail[(int)T.size()-i]) ans = "Yes";
		cout << ans << endl;
	}

	return 0;
}
