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

// ABC326 https://atcoder.jp/contests/abc326

/*
 * 半分全列挙で解く。どちらかというと考察より実装問題かな(復元部分)。
 * 以下、0-indexedで記載する。
 * 参考にした実装：https://atcoder.jp/contests/abc326/submissions/47130466
 * 
 * Y,Yは独立して処理することができる。
 * 偶数回目(0-indexed)の移動量をまとめて、それの正負を自由に決めたときの総和をYにできるか？を考える。
 * ⇒要素数の最大が40であるため、単純なbit全探索では間に合わない。
 *   またA,Yの値も大きく、DPでは間に合わない。
 * 要素40を前半後半に分けて半分全列挙で解く。
 * 詳しくはmake()を参照。
 * 
 * 出力において、正負だけではなく前回の移動結果も使ってL,Rが決まる点が厄介。
 * 簡単な実装としては、
 * Y方向の移動：前回(X方向)'+'で今回'-', または前回'-'で今回'+' のときR
 * X方向の移動：前回(Y方向)'+'で今回'+', または前回'-'で今回'-' のときR
 * となるので、前回の移動を覚えておき、xorで決めることができる。
 * 
 * 初回AC時からの、実装の改善ポイント：
 * ・Nが4未満はA=0を追加して半分全列挙の実装を簡易に 最後に足した分だけ消せばよい
 * ・次にLRどちらになるかを、前回の方向とxorして計算
 */

// 以下、bは0-indexed
bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)
void setbit(ll &x, int b) { x |= ((ll)1<<b); }  // xのbビット目を立てる
void unbit(ll &x, int b) { x &= (~((ll)1<<b)); }  // xのbビット目を落とす

using pll = pair<ll,ll>;
// dx[]の各要素を加減算してXを作れればtrue
// resのbit i:dx[i]を減算して使うなら0, 加算で使うなら1
bool make(vector<ll> dx, ll x, ll &res)
{
	ll i, j;
	vector<ll> dx_l(dx.begin(), dx.begin()+dx.size()/2);  // dxの前半後半
	vector<ll> dx_r(dx.begin()+dx.size()/2, dx.end());
	vector<pll> cal_l;  // {sum,bit}
	for(i = 0; i < 1<<(int)dx_l.size(); i++)
	{
		ll sum = 0;
		for(j = 0; j < (int)dx_l.size(); j++)
		{
			if( ((i>>j)&1) == 0) sum -= dx_l[j];
			else sum += dx_l[j];
		}
		cal_l.push_back({sum, i});
	}
	sort(cal_l.begin(), cal_l.end());

	for(i = 0; i < 1<<(int)dx_r.size(); i++)
	{
		ll sum = 0;
		for(j = 0; j < (int)dx_r.size(); j++)
		{
			if( ((i>>j)&1) == 0) sum -= dx_r[j];
			else sum += dx_r[j];
		}
		// x-sumを探す
		int idx = lower_bound(cal_l.begin(), cal_l.end(), make_pair(x-sum,0LL))- cal_l.begin();
		if(sum + cal_l[idx].first == x)
		{
			// 下位側をcal_l, 上位側をcal_r
			res = i;
			res <<= (int)dx_l.size();
			res |= cal_l[idx].second;
			return true;
		}
	}

	return false;
}

int main(void)
{
	ll i;
	ll N, X, Y; cin >> N >> X >> Y;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	int add = 0;
	while(N < 4)
	{
		a.push_back(0);
		N++; add++;
	}
	vector<ll> dx, dy;
	for(i = 0; i < N; i++)
	{
		if(i%2 == 0) dy.push_back(a[i]);  // 最初はY方向へ動く
		else dx.push_back(a[i]);
	}
	ll res_y, res_x;
	if(!make(dy, Y, res_y) || !make(dx, X, res_x))
	{
		cout << "No" << endl;
		return 0;
	}

	// 次Y方向:pre1,now0はR, pre0,now1はR
	// 次X方向:pre1,now1はR, pre0,now0はR
	cout << "Yes" << endl;
	string ans;
	int pre = 1;  // 前回進んだ軸が正負どちらか 正なら1
	for(i = 0; i < N; i++)
	{
		int now;
		if(i%2 == 0)  // Y方向へ移動
		{
			now = isbiton(res_y, i/2) ? 1 : 0;
			if((pre^now) == 1) ans += 'R';
			else ans += 'L';
		}
		else
		{
			now = isbiton(res_x, i/2) ? 1 : 0;
			if((pre^now) == 0) ans += 'R';
			else ans += 'L';
		}
		pre = now;
	}
	for(i = 0; i < add; i++) ans.pop_back();  // 最初に番兵として足した分だけ消す
	cout << ans << endl;

	return 0;
}
