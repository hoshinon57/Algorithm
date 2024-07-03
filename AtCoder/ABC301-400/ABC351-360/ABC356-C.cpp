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

// ABC356 https://atcoder.jp/contests/abc356

// 鍵が正しいかダミーかをbit全探索
// bit全探索の各ループにて、
//   テストごとに、使った鍵のうち正しい鍵の数をカウント
//   カウントがテスト成功/失敗に応じて矛盾があるかどうかチェック

int main(void)
{
	int i, j;
	int N, M, K; cin >> N >> M >> K;
	vector<int> test(M);  // test[i]:i番目のテストにて使った鍵をbitで表現
	vector<bool> res(M);
	for(i = 0; i < M; i++)
	{
		int c; cin >> c;
		for(j = 0; j < c; j++)
		{
			int a; cin >> a;
			a--;
			test[i] |= (1<<a);
		}
		char r; cin >> r;
		if(r == 'o') res[i] = true;
		else res[i] = false;
	}

	int ans = 0;
	for(i = 0; i < (1<<N); i++)  // 鍵をbitに割り当てる
	{
		bool ok = true;
		for(j = 0; j < M; j++)  // テストj
		{
			int cnt = __builtin_popcount(test[j] & i);
			if(res[j] && cnt >= K) {;}  // テスト成功、K本以上なら矛盾なし
			else if(!res[j] && cnt < K) {;}  // テスト失敗、K本未満なら矛盾なし
			else ok = false;  // 矛盾あり
		}
		if(ok) ans++;
	}
	cout << ans << endl;

	return 0;
}
