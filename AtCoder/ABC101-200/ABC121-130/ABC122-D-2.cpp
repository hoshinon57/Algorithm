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

#include <atcoder/modint>
using namespace atcoder;
using mint = modint1000000007;

// ABC122 https://atcoder.jp/contests/abc122

/*
 * 最初に考えた方針。
 * DPの添え字に文字列で持つのではなく、数値にエンコードした値でやる方針。
 * 
 * 直近4文字を見る、DP配列には3文字を持たせるのは同じ。
 * A:0,G:1,C:2,T:3とすると4文字の組み合わせは 4^4=256 通りで表現できるので、forループで256回回す。
 * AOJ-2879.cpp(ごちうさ数)が近い。
 * 
 * ※ただし処理時間はほとんど変わらなかった。
 * 
 * 後は実装を参照。
**/

// vector<int>の配列を1つの値にエンコード/デコードする関数
// 配列の値が[0,base), 要素数がpのとき、組み合わせの状態数は base^p となる。これを1つの値にエンコードする
// エンコードした値をDPの添え字に使う想定
// ★★ENC_VECTONUM_BASE_DEFAULTに値を設定すること★★
// [verify]ABC380-F,ABC322-E
const int ENC_VECTONUM_BASE_DEFAULT = 4;	// enc_VecToNum(), dec_ValToVec()で指定する基数のデフォルト 問題内で基数が変わることも無さそうなので、これで指定すれば呼び出し時の指定を省ける
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

// 値nをAGCT文字列に変換したとき、合法ならtrue
bool check(int n)
{
	// AGCがあればtrue(=NG)
	auto ng = [](vector<int> vv) -> bool
	{
		if(vv[0] == 0 && vv[1] == 1 && vv[2] == 2) return true;
		if(vv[1] == 0 && vv[2] == 1 && vv[3] == 2) return true;
		return false;
	};

	auto v = dec_ValToVec(n, 4);
	if(ng(v)) return false;  // 素の状態でAGCがあるか

	// swapしてAGCになるか
	for(int i = 0; i <= 2; i++)
	{
		auto tmp = v;
		swap(v[i], v[i+1]);
		if(ng(v)) return false;
		v = tmp;
	}

	return true;
}

int main(void)
{
	int N; cin >> N;
	int i, j;
	vector<mint> dp(64);  // 4^3  AGCTをそれぞれ0,1,2,3に割り当てる
	dp[63] = 1;  // "TTT"

	for(i = 0; i < N; i++)
	{
		vector<mint> ndp(64);
		for(j = 0; j < 256; j++)  // 4^4
		{
			if(!check(j)) continue;
			int v = j/4;  // 上位3つ
			int nv = j%64;  // 下位3つ
			ndp[nv] += dp[v];
		}
		swap(ndp, dp);
	}

	mint ans = 0;
	for(auto &e : dp) ans += e;
	cout << ans.val() << endl;

	return 0;
}
