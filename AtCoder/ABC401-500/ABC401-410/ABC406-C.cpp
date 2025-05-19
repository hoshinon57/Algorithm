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

// ABC406 https://atcoder.jp/contests/abc406

/*
 * ランレングス圧縮を用いて解く。
 * ※コンテスト時の実装よりだいぶ簡潔になった
 * 
 * 基本は公式解説の通り。
 *   https://atcoder.jp/contests/abc406/editorial/13047
 * A[i]とA[i+1]の大小関係で長さ(N-1)の > or < の文字列を作り、ランレングス圧縮する。
 * チルダ型は <>< となるので、>に着目。
 * (1つ手前の'<'の長さ)*(1つ後ろの'<'の長さ) を答に加算する。
 * 
 * 要素が3つあるとき、真ん中の要素について全探索する典型。
**/

// 文字列strをランレングス圧縮して {文字,長さ} のpairの列挙で返す
// [補足]配列に対して扱いたい場合、引数をstring->vectorに変えるだけで良い
//       "長さ"は呼び出し元で乗算する可能性があるため、オーバーフロー防止でll型としている (ABC369-C)
// auto enc = rle<char>(s); といった形で呼び出す
// for(auto [c,len] : enc) が使い方の一例
// [参考]https://algo-logic.info/run-length/  [verify]ABC019-B,ABC380-B
template <typename T> vector<pair<T,ll>> rle(string &str) {
	vector<pair<T,ll>> ret;
	int n = str.size();
	for(int l = 0, r = 0; l < n; ) {  // 尺取り法っぽく [l,r)が条件を満たす
		while(r < n && str[l] == str[r]) r++;
		ret.push_back({str[l], r-l});
		l = r;
	}
	return ret;
}

int main(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	string s;
	for(i = 0; i < N-1; i++)
	{
		if(a[i] < a[i+1]) s.push_back('<');
		else s.push_back('>');
	}
	auto enc = rle<char>(s);

	ll ans = 0;
	for(i = 0; i < (int)enc.size(); i++)
	{
		auto [c, len] = enc[i];
		if(c == '<') continue;
		// '>'について処理
		ll l,r;

		if(i == 0) l = 0;
		else l = enc[i-1].second;

		if(i == (int)enc.size()-1) r = 0;
		else r = enc[i+1].second;
		
		ans += l*r;
	}
	cout << ans << endl;

	return 0;
}
