#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC242 https://atcoder.jp/contests/abc242
// 参考：https://kyo-pro.hatenablog.com/entry/ABC242D
//       https://qiita.com/u2dayo/items/465c9299755ed20ee4d3

/*
 * 再帰で解く。
 * まず、t, kともに0-indexedとする。
 * kの入力は1-indexedになっているので、-1しておく。
 * 
 * t回操作後のk, k+1文字目を考える。
 * これはt-1回後のk/2(端数切捨て)文字目からの遷移となる。
 * かつ、遷移前の文字に対して、k文字目は1つ、k+1文字目は2つ後ろにずれる。
 * よってt回後のk文字目は、t-1回後のk/2文字目＋1or2 となる。これを再帰する。
 * 
 * 次に0文字目を考える。
 * これはA->B->C->A->B->... と順々に遷移していく。
 * よってt回後の0文字目は、入力文字Sを使って (S[0]+t) mod 3 と表せる。
 * 
 * 再帰ごとにtは1しか減らない。
 * 操作tが最大10^18回なので、これだけの回数の再帰は不可能。
 * しかしkは再帰ごとに1/2になっていく。
 * kも最大10^18≒2^60であり、60回程度の再帰でk=0となる。
 * 前述の通り0文字目は1回の計算で出せるので、tが大きくなると大半はこっちで計算終了できる、というわけ。
 */
string S;
vector<int> s_vec;  // 0:'A', 1:'B', 2:'C' に置き換える

// t回操作後のk文字目を返す
int func(ll t, ll k)
{
	if(k == 0)  // func(t, 0)
	{
		// 0文字目は操作ごとにA->B->C->A->... と変化する
		return (s_vec[0] + t)%3;
	}
	if(t == 0)  // func(0, k)
	{
		// これはs_vec[]そのまま
		return s_vec[k];
	}

	// func(t-1, k/2)を返す
	// t-1回目から遷移するとき、kが偶数ならt回目は1文字ぶん後ろにずれ、奇数なら2文字ぶん後ろにずれる。
	int tmp = (k%2 == 0) ? 1 : 2;
	return (func(t-1, k/2) + tmp)%3;
}

int main(void)
{
	ll i;
	string S;
	cin >> S;
	s_vec.resize(S.size());
	for(i = 0; i < S.size(); i++)
	{
		s_vec[i] = S[i] - 'A';
	}

	int Q;
	cin >> Q;
	ll t, k;  // 0-indexed
	for(i = 0; i < Q; i++)
	{
		cin >> t >> k;
		k--;  // 0-indexedに変換
		int ans = func(t, k);  // t回操作してk文字目を返す
		cout << (char)(ans+'A') << endl;
	}

	return 0;
}
