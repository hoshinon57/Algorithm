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
using mint = modint998244353;

// ABC419 https://atcoder.jp/contests/abc419

/*
 * DPで解く。snuke氏の解説を参考にしている。
 *   https://www.youtube.com/live/sgvGfbE1QVE
 * 
 * dp[s][j]を以下のように定義する。
 *   i文字目まで見て、  (i=0～L)
 *   Nbit幅で、Sxを既に部分文字列に含んでいればsのx bit目を1にし、  (s=0～(2^N)-1)
 *   末尾の文字列(に割り当てられたID)がj
 *   のときの総数。
 * dp,ndp形式で実装し、最後にs[(2^N)-1][*]の総和が答。
 *   
 * 「末尾の文字列」について、各Sjの全prefixのみを考えればよい。
 * これは制約より、8*10=80通り程度となる。空文字列を別1つ追加はしておく。
 * IDは
 *   prefixである文字列⇒ID
 *   ID⇒prefixである文字列
 * への相互変換を用意しておく。実装ではそれぞれtoid, toprとなる。
 * 
 * 次に、あるprefixが登場したとき、dpのsにどう寄与するかを事前計算する。
 * S1="aab", S2="aa"のとき、"aab"が登場したらS1,S2ともに登場したことになるため。
 * これは実装のmask[]を参照。
 * 
 * また、あるIDに1文字追加したとき、どのIDに遷移するかも事前計算しておく。
 * to[id][c] として遷移先のIDを取得することになる。
 * これは実装のto[]を参照。
 * 
 * 以上をもって、配るDPで答を求めていく。
 * i文字目まで作ったとき、dp[m][j]からndpへは、次に追加する文字c(26種)を全探索して、
 *  nj = to[j][c]
 *  nm = m | mask[nj]
 * としてndp[nm][nj]へ遷移する。
**/

int main(void)
{
	ll i, j;
	ll N, L; cin >> N >> L;
	vector<string> s(N); for(i = 0; i < N; i++) {cin >> s[i];}

	// s[i]の全prefixをidに割り当てる
	map<string, ll> toid;  // toid[str] = id
	vector<string> topr;  // topr[id] = str
	ll mxid = 0;  // idの個数
	auto id_add = [&](string ss) -> void
	{
		if(toid.count(ss) > 0) return;  // 登録済み
		toid[ss] = mxid;
		topr.push_back(ss);
		mxid++;
	};
	id_add("");
	for(i = 0; i < N; i++)
	{
		for(j = 1; j <= (ll)s[i].size(); j++)  // s[i]の先頭からj文字
		{
			id_add(s[i].substr(0, j));
		}
	}

	// あるprefixが登場したとき、dpのSにどう寄与するか
	vector<ll> mask(mxid);
	for(i = 0; i < mxid; i++)
	{
		for(j = 0; j < N; j++)
		{
			if(topr[i].find(s[j]) != string::npos)
			{
				mask[i] |= (1LL<<j);
			}
		}
	}

	// あるidに1文字追加したとき、どのidに遷移するか
	vector<vector<ll>> to(mxid, vector<ll>(26, 0));  // to[mxid][26]
	for(i = 0; i < mxid; i++)
	{
		for(j = 0; j < 26; j++)  // to[i][jを文字換算]
		{
			string t = topr[i] + (char)(j+'a');
			// tがtoidから見つかるまで、先頭の1文字を削除
			while(toid.count(t) == 0) t.erase(t.begin());
			to[i][j] = toid[t];
		}
	}

	// 配るDP
	ll np = (1LL<<N);  // 2^N
	vector<vector<mint>> dp(np, vector<mint>(mxid, 0));  // dp[np][mxid]
	dp[0][0] = 1;
	for(i = 0; i < L; i++)
	{
		vector<vector<mint>> ndp(np, vector<mint>(mxid, 0));
		for(ll m = 0; m < np; m++)
		{
			for(j = 0; j < mxid; j++)  // dp[m][j] -> ndp
			{
				if(dp[m][j].val() == 0) continue;
				for(ll c = 0; c < 26; c++)  // 文字cを追加
				{
					ll nj = to[j][c];
					ll nm = (m | mask[nj]);
					ndp[nm][nj] += dp[m][j];
				}	
			}
		}
		swap(dp, ndp);
	}

	mint ans = 0;
	for(j = 0; j < mxid; j++)
	{
		ans += dp[np-1][j];
	}
	cout << ans.val() << endl;

	return 0;
}
