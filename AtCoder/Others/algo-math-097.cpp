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

// アルゴリズムと数学 演習問題集 097 https://atcoder.jp/contests/math-and-algorithm/tasks/math_and_algorithm_bt

// エラトステネスの区間篩を用いて解いた & ライブラリ化

// エラトステネスの区間篩
// 区間[L,R]の素数を列挙してprime_listに入れる
// L,R: 1<=L<=R よって[L,R]=[1,1]は制約として合法(答は0個)
// rootR:
//   √R"以上"の値 1以上√R以下の素数を列挙する際に、配列確保に用いる
//   kth_root_integer()を用いるなり、制約から決め打ちするなり
// Rが10^14ぐらいまで、またR-Lが10^7ぐらいまで(Rに対して区間が狭い的な)場合に有効
// 計算量は M=max(R-L,√R) としてO(MloglogM)とか何とか。
//   https://algo-method.com/tasks/332/editorial
//   https://atcoder.jp/contests/abc412/editorial/13387
void make_prime_list_LR(long long L, long long R, long long rootR, vector<long long> &plist) {
	long long i, j;
	long long dif = R-L;
	vector<bool> isp1(rootR+1, true);  // 1～√Rまでの素数列挙用
	vector<bool> isp2(dif+1, true);  // isp2[i-L]: iが素数ならtrue i-Lで参照する
	for(i = 2; i*i <= R; i++) {
		if(!isp1[i]) continue;
		for(j = i*2; j*j <= R; j+=i) {
			isp1[j] = false;
		}

		long long st = ((L+i-1)/i)*i;  // L以上でiの倍数のうち最小値
		for(j = st; j <= R; j+=i) {
			if(j == i) continue;  // コーナーケース的な このときjは√R以下で列挙された素数そのもの
			isp2[j-L] = false;
		}
	}
	for(i = max(L,2LL); i <= R; i++) {  // max(L,2) 1は素数ではない
		if(isp2[i-L]) plist.push_back(i);
	}
}

int main(void)
{
	ll L, R; cin >> L >> R;
	vector<ll> plist;
	make_prime_list_LR(L, R, 1e6, plist);
	cout << plist.size() << endl;

	return 0;
}
