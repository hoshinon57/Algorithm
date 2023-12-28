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

// ABC162 https://atcoder.jp/contests/abc162

/*
 * 制約よりO(N^2)が通りそう。
 * i<j<kのうちi,jの組み合わせを全探索し、条件を満たすkを求める方針とした。
 * 
 * まずs[i]=s[j]ならNG.
 * s[k]になるべき値xを求める。今回はMEXを用いた。
 * [j+1,N)の範囲でxの個数を求め、答に加算する。これはR,G,Bそれぞれの累積和を使った。
 * ただしまだ条件2の i-j!=k-j を加味していないので、
 * i-j=k-j となるkが条件1を満たしているかを判定し、満たしていたら除外する。
 */

// a[]の要素に含まれない最小の非負整数を返す
// 呼び出し元で mex({0,2,3}); のようにinitializer_listを用いることを想定しているため、aは参照渡しにしていない
// a[]の要素数をNとして、計算量はO(N^2).
int mex(vector<int> a)
{
	int i, j;
	int a_size = (int)a.size();
	// iがMEXになるか判定
	// 要素数を超える値はMEXになりえないため、上限は要素数未満まで
	// (入力が {0,1,2} のような場合、i=3でループを抜けるため正しく判定される)
	for(i = 0; i < a_size; i++)
	{
		bool check = false;
		for(j = 0; j < a_size; j++)  // a[]にiが含まれているか
		{
			if(a[j] == i) check = true;
		}
		if(!check) break;
	}
	return i;
}

int main(void)
{
	int i, j;
	int N; cin >> N;
	string s_; cin >> s_;
	vector<int> s(N);
	for(i = 0; i < N; i++)
	{
		if(s_[i] == 'R') s[i] = 0;
		else if(s_[i] == 'G') s[i] = 1;
		else s[i] = 2;
	}
	vector<vector<int>> sum(3, vector<int>(N+1, 0));  // sum[3][N+1]  R,G,Bそれぞれの個数の累積和
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < 3; j++)
		{
			sum[j][i+1] = sum[j][i];
			if(s[i] == j) sum[j][i+1]++;
		}
	}

	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N-1; j++)
		{
			if(s[i] == s[j]) continue;
			int x = mex({s[i], s[j]});  // s[k]になるべき値
			ans += sum[x][N] - sum[x][j+1];  // [j+1,N)の範囲でxの個数を答に加算

			// i-j = k-j となるkが条件を満たしているなら、除外するため-1
			int k = j+(j-i);
			if(k < N && s[k] == x) ans--;
		}
	}
	cout << ans << endl;

	return 0;
}
