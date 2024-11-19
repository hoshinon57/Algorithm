#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <unordered_map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC380 https://atcoder.jp/contests/abc380

/*
 * ゲームのDP.
 * S=N+M+L(<=12)として、全S枚のカードの状態を
 *   0:高橋くんが持つ
 *   1:青木くんが持つ
 *   2:場にある
 * とすると、状態数は最大で3^12(約5*10^5)となり、管理できる。
 * 
 * ただ通常のbitDPと違い、状態を1つの変数にまとめられない（まとめづらい）のが厄介。
 * DP値をvector<int>で持ち、
 *   {1枚目の状態, 2枚目, ..., S枚目の状態, どちらのターンか(0 or 1)}
 * とし、map<vector<int>, bool>にて管理する。
 * 
 * DP遷移は以下。
 * ・ベースケースとして、自分の手札が無ければ負け。
 * ・手札から1枚選んで場に出す。
 *     場から取らないケース (これ必要かどうかは分からないが)
 *     場から1枚とるケース
 *   それぞれについて処理し、相手ターンへ。
 *   相手の負けが1つでもあれば、自分の勝ち。そうでなければ自分の負け。
 * 
 * 相手ターンの負けがあった時点で枝刈りをいくつか行っている。
 * 枝刈りが全くなしだとTLEだった。mapを用いているからと思われる。
 */

// {1枚目の状態, 2枚目, ..., S枚目の状態, どちらのターンか(0 or 1)} として状態を持つ
// カードの状態は、0:高橋,1:青木,2:場
map<vector<int>, bool> mp;  // 状態, 現在のターンの人が勝つか で管理

vector<ll> num;  // num[i]:i番目のカード値
int N, M, L;
int nml;  // N+M+L

// dt[]にて管理する状態で、現在のターンの人が勝つならtrue
bool dp(vector<int> &dt)
{
	if(mp.count(dt)) return mp[dt];
	bool &d = mp[dt];

	int i, j;
	int turn = dt.back();
	// ベースケース
	{
		bool ok = false;
		for(i = 0; i < nml; i++)
		{
			if(dt[i] == turn)
			{
				ok = true;  // 自分の手札が残っている
				break;
			}
		}
		if(!ok) return (d = false);
	}

	bool win = false;
	auto dt_bk = dt;
	for(i = 0; i < nml; i++)
	{
		if(dt[i] != turn) continue;
		// i番目のカードを場に出す
		dt[i] = 2;
		dt.back() = 1-turn;
		if(!dp(dt))  // カードをもらわないケース
		{
			win = true;
			dt = dt_bk;  // 元に戻す
			break;
		}
		dt = dt_bk;  // 元に戻す

		for(j = 0; j < nml; j++)
		{
			if(dt[j] == 2 && num[i] > num[j])
			{
				// i番目のカードを出し、j番目のカードをもらう
				dt[i] = 2;
				dt[j] = turn;
				dt.back() = 1-turn;
				if(!dp(dt))
				{
					win = true;
					dt = dt_bk;
					break;
				}
				dt = dt_bk;
			}
		}
	}
	return (d = win);
}

int main(void)
{
	int i;
	cin >> N >> M >> L;
	nml = N+M+L;
	num.resize(nml);
	vector<int> dt(nml+1);  // {i番目のカードの持ち主, 現在のターン}  0:Takahashi, 1:Aoki, 2:場
	for(i = 0; i < N; i++)
	{
		cin >> num[i];
		dt[i] = 0;
	}
	for(i = N; i < N+M; i++)
	{
		cin >> num[i];
		dt[i] = 1;
	}
	for(i = N+M; i < N+M+L; i++)
	{
		cin >> num[i];
		dt[i] = 2;
	}
	dt[nml] = 0;

	string ans;
	if(dp(dt)) ans = "Takahashi";
	else ans = "Aoki";
	cout << ans << endl;

	return 0;
}
