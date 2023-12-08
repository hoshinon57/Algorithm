#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <cassert>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC198 https://atcoder.jp/contests/abc198

/*
 * 覆面算を頑張って解く。
 * 
 * 文字を数値への割り当てを全探索する。
 * 制約より文字数は最大10種であり、10!≒3*10^6 程度なので間に合う。
 * 
 * 以下、実装では入力にて出てきた文字の一覧をchlistに入れている。
 * 10文字未満は"*"で埋めることで、next_permutation()と組み合わせて
 * 「i番目の文字がc」⇒「文字cを数値iに割り当てる」とみなすことができる。
 * 文字数が10種より多ければ、問題文の条件によりNGとなる。
 * 
 * 文字列の先頭に0が付く場合、これまた条件よりNGとなるため
 * 文字列⇒数値への変換関数にて-1を返すようにしている。
 */

int main(void)
{
	int i;
	string s[3];
	string chlist;  // 使う文字のリスト
	for(i = 0; i < 3; i++)
	{
		cin >> s[i];
		for(auto &e : s[i])
		{
			if(chlist.find(e) == string::npos) chlist.push_back(e);
		}
	}
	for(i = (int)chlist.size(); i < 10; i++) chlist.push_back('*');  // 10文字未満は"*"で埋める
	if(chlist.size() > 10)  // 10文字より多い場合、問題文の条件によりNG
	{
		cout << "UNSOLVABLE" << endl;
		return 0;
	}
	sort(chlist.begin(), chlist.end());

	do
	{
		map<char, int> mp;  // mp[c]=i  文字cに数iを割り当てる
		for(i = 0; i < 10; i++)
		{
			mp[chlist[i]] = i;
		}
		// 文字列tmpを数値に変換する
		// 先頭に0が付くケースはNGなので-1を返す
		auto calc = [&](string tmp) -> ll
		{
			if(mp[tmp[0]] == 0) return -1;
			ll sum = 0;
			for(auto &e : tmp)
			{
				sum *= 10;
				sum += mp[e];
			}
			return sum;
		};
		ll n1, n2, n3;
		n1 = calc(s[0]);
		n2 = calc(s[1]);
		n3 = calc(s[2]);
		if(n1 < 0 || n2 < 0 || n3 < 0) continue;
		if(n1+n2 == n3)
		{
			cout << n1 << endl << n2 << endl << n3 << endl;
			return 0;
		}
	} while (next_permutation(chlist.begin(), chlist.end()));
	// ここまで来たら解無し
	cout << "UNSOLVABLE" << endl;

	return 0;
}
