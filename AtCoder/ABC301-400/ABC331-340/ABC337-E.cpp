#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC337 https://atcoder.jp/contests/abc337

/*
 * (たぶん有名な)毒入りワイン問題。
 *   https://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q14115376614
 *   https://ishtarvenus.hatenablog.com/entry/2018/04/12/190000
 * 
 * ワインの本数Nに対し、人数はM=(log2(N)の切り上げ) で足りる。
 * ワインにナンバリングをするイメージ。例えばN=4のとき
 *   1本目：00
 *   2本目：01
 *   3本目：10
 *   4本目：11
 * として、bit0が1のワインを1人目が、bit1が1のワインを2人目が飲む。
 * 上記だと1人目は2,4本目のワインを飲む。
 * すると、腹痛になった人とワインのナンバリングが1:1に対応付けられる。
 * 
 * [ACまでの思考の流れ]
 * ・これ有名な毒入りドリンク問題だ、というのを思い出した。
 *   「毒入り 飲み物 問題」とかでググって解法を得た。
 *   知らなかったら解けなかったかも。
 * ・Mの計算に手間取った、反省。
 */

// 2^M>=N となるような最小のMを求める
int m_(int n)
{
	int m = 0;
	int val = 1;  // 2^0=1
	while(val < n)
	{
		m++;
		val *= 2;
	}
	return m;
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	int M = m_(N);
	cout << M << endl;
	vector<vector<int>> p(N);  // p[i]:友人iに飲ませるドリンクの一覧
	for(i = 0; i < N; i++)  // ドリンクi
	{
		for(j = 0; j < M; j++)  // 友人j
		{
			if(i&(1<<j)) p[j].push_back(i);
		}
	}
	for(i = 0; i < M; i++)
	{
		cout << p[i].size() << " ";
		for(auto &e : p[i]) cout << e+1 << " ";
		cout << endl;
	}
	string s; cin >> s;
	int ans = 0;
#if 1
	for(i = 0; i < M; i++)
	{
		if(s[i]=='1') ans |= 1<<i;
	}
#else  // stoi()で2進数として変換する方法(本問だとreverseは事前に必要)
	reverse(s.begin(), s.end());
	ans = stoi(s, nullptr, 2);
#endif
	cout << ans+1 << endl;

	return 0;
}
