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

// 競プロ典型90問:51 https://atcoder.jp/contests/typical90/tasks/typical90_ay

/*
 * 半分全列挙で解く。
 * 
 * N要素をそれぞれ選ぶ/選ばないの通り数は2^Nであり(bit全探索)、N=20程度が時間的に限界。
 * 要素を2グループに分けることで、それぞれがO(N*2^(N/2))となり、N=40程度まで可能となる。
 * 
 * 前半グループについてbit全探索で値段の合計を求める。
 * n個選んだときの値段を、val[n]に入れていく。
 * ※後にupper_bound()するのでソートしておく (忘れてWA)
 * 
 * 後半グループも同様にbit全探索する。
 * n個選んだとき、前半グループからは(K-n)個選ぶことになる。
 * va[K-n]から(P-sum)以下の要素数を求めたい。これはupper_bound()で求められる。
 * 
 * [ACまでの思考の流れ]
 * ・N=40は半分全列挙を考えてみる。
 */

int main(void)
{
	ll i, j;
	ll N, K, P; cin >> N >> K >> P;
	ll na = N/2, nb = N-na;
	vector<ll> a(na); for(i = 0; i < na; i++) {cin >> a[i];}
	vector<ll> b(nb); for(i = 0; i < nb; i++) {cin >> b[i];}
	vector<vector<ll>> val(na+1);  // val[k]={v1,v2,v3,...}:na側の、k個選んだときの値段の合計を入れていく
	for(i = 0; i < (1<<na); i++)
	{
		ll sum = 0;
		for(j = 0; j < na; j++)
		{
			if(((i>>j)&1) == 0) continue;
			sum += a[j];
		}
		int n = __builtin_popcount(i);
		val[n].push_back(sum);
	}
	for(auto &e : val)
	{
		sort(e.begin(), e.end());
	}

	ll ans = 0;
	for(i = 0; i < (1<<nb); i++)
	{
		ll sum = 0;
		for(j = 0; j < nb; j++)
		{
			if(((i>>j)&1) == 0) continue;
			sum += b[j];
		}
		int n = __builtin_popcount(i);
		// val[K-n]から探す
		if(0<=K-n && K-n<=na) {;}
		else continue;
		// 和がP以下 ⇒ P-sumのupper_boundが境界
		ll c = upper_bound(val[K-n].begin(), val[K-n].end(), P-sum) - val[K-n].begin();
		ans += c;
	}
	cout << ans << endl;

	return 0;
}
