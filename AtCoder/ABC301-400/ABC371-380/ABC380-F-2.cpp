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

// ABC380 https://atcoder.jp/contests/abc380

/*
 * mapを使わない実装。
 * vector<int>で表現したカードの状態を、1つの値にエンコードしてDPの添え字に使う。
 */

int nml;
vector<int> num;  // num[i]:i番目のカードの値

vector<vector<int>> dp;  // dp[S][t] 状態がS,今のターンがtのとき、勝つプレイヤーの番号
// S:全(N+M+L)枚のカードを 0:Takahashi, 1:Aoki, 2:場 とし、
//   各カードの状態を3進数としてみなして1つの値にエンコードしたもの

// enc_VecToNum(), dec_ValToVec()で指定する基数のデフォルト
// 問題内で基数が変わることも無さそうなので、これで指定すれば呼び出し時の指定を省ける
const int ENC_VECTONUM_BASE_DEFAULT = 3;

// 要素の値が[0,base)の範囲であるstateについて、[0]から順にbase進法としてみなして1つの整数値にまとめる(変換する)
// 例：state={0,3,2,1}, base=4であれば、0*4^0+3*4^1+2*4^2+1*4^3 を返す
// 戻り値をDPの添え字に使うことを想定しているため、int型
int enc_VecToNum(vector<int> &state, int base = ENC_VECTONUM_BASE_DEFAULT) {
	int ans = 0, p = 1;
	for(int i = 0; i < (int)state.size(); i++) {
		ans += p*state[i];
		p *= base;
	}
	return ans;
}

// enc_VecToNum()での戻り値からvector<int>に変換して返す
// siz:vector<int>の要素数
vector<int> dec_ValToVec(int n, int siz, int base = ENC_VECTONUM_BASE_DEFAULT) {
	vector<int> ret(siz);
	for(int i = 0; i < siz; i++) {
		ret[i] = n % base;
		n /= base;
	}
	return ret;
}

// dp[S][turn]を返す
int bitDP(int S, int turn)
{
	if(dp[S][turn] != -1) return dp[S][turn];
	int &d = dp[S][turn];

	int i, j;
	auto st = dec_ValToVec(S, nml);
	{  // ベースケース
		bool ok = false;
		// 自分の手札が1枚も無ければ相手の勝ち
		for(i = 0; i < nml; i++)
		{
			if(st[i] == turn) ok = true;
		}
		if(!ok)
		{
			return (d = 1-turn);  // 相手勝ち
		}
	}

	for(i = 0; i < nml; i++)
	{
		if(st[i] != turn) continue;
		// 以下、i番目は自分のカードで、それを場に出す
		st[i] = 2;
		// 場からカードを回収しないケース
		if(bitDP(enc_VecToNum(st), 1-turn) == turn)  // 相手負け
		{
			return (d = turn);  // 自分の勝ち
		}

		for(j = 0; j < nml; j++)
		{
			if(st[j] == 2 && num[i] > num[j])
			{
				st[j] = turn;
				if(bitDP(enc_VecToNum(st), 1-turn) == turn)  // 相手負け
				{
					return (d = turn);  // 自分の勝ち
				}
				st[j] = 2;  // 元に戻す
			}
		}
		st[i] = turn;  // 元に戻す
	}

	return (d = 1-turn);  // 相手勝ち
}

int main(void)
{
	int i;
	int N, M, L; cin >> N >> M >> L;
	nml = N+M+L;
	num.resize(nml);
	vector<int> st(nml);  // st[i]:i番目のカードの状態 0:Takahashi, 1:Aoki, 2:場
	dp.resize(540000, vector<int>(2, -1));  // 3^12=531441
	for(i = 0; i < N; i++)
	{
		cin >> num[i];
		st[i] = 0;
	}
	for(i = N; i < N+M; i++)
	{
		cin >> num[i];
		st[i] = 1;
	}
	for(i = N+M; i < N+M+L; i++)
	{
		cin >> num[i];
		st[i] = 2;
	}

	string ans;
	if(bitDP(enc_VecToNum(st), 0) == 0) ans = "Takahashi";
	else ans = "Aoki";
	cout << ans << endl;

	return 0;
}
