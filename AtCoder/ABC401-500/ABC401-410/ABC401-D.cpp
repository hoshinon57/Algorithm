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

// ABC401 https://atcoder.jp/contests/abc401

/*
 * 公式解説にある通り、丁寧な場合分け問題。
 * 以下のように分類していった。
 * Sに含まれる'o'の数をocとする。
 * 
 * oc=Kのとき：
 * これ以上oは増えないので、?をxにして終了。
 * 
 * oの隣をxにしておく。
 * 
 * ?が連続する区間について、左から貪欲にoにしていく。ここでランレングス圧縮を用いた。
 * ocは (?の連続数)/2 の切り上げだけ増加する。
 * 
 * oc>Kのとき：
 * ?は一意に定まらない。現時点のを出力して終了。
 * 
 * oc=Kのとき：
 * ?の連続する区間ごと、?が奇数個なら"oxoxo"のように一意に決まる。
 * 偶数個なら決まらない。
 * 
 * 補足、oc<Kのとき：
 * このとき合法な文字列は作れない。
 * 制約の「Xは空集合ではない」に反するのでこのケースは気にしなくてよい。
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
	int i;
	int N, K; cin >> N >> K;
	string s; cin >> s;

	int oc = count(s.begin(), s.end(), 'o');
	if(oc == K)
	{
		for(auto &e : s) if(e == '?') e = '.';
		cout << s << endl;
		return 0;
	}

	for(i = 0; i < N; i++)
	{
		if(s[i] == 'o')
		{
			if(i-1 >= 0) s[i-1] = '.';
			if(i+1 < N) s[i+1] = '.';
		}
	}

	auto enc = rle<char>(s);
	for(auto [c,len] : enc)
	{
		if(c != '?') continue;
		oc += (len+1)/2;
	}

	if(oc > K)  // ?は定まらない
	{
		cout << s << endl;
	}
	else  // ?は奇数個なら定まる
	{
		for(auto [c,len] : enc)
		{
			if(c == 'o' || c == '.' || (c == '?' && (len%2 == 0)) )
			{
				for(i = 0; i < len; i++) cout << c;
			}
			else  // ?が奇数個
			{
				for(i = 0; i < len; i++)
				{
					if(i%2 == 0) cout << 'o';
					else cout << '.';
				}
			}
		}
		cout << endl;
	}

	return 0;
}
