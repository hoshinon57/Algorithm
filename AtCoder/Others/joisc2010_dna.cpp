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

// https://atcoder.jp/contests/joisc2010/tasks/joisc2010_dna

/*
 * Trie木で解いた。
 * 以下、0-indexedで記載する。
 * 
 * 先頭からi文字目まで作れるとする。
 * ＞素DNA鎖の長さは20以下である．
 * という制約より、i文字目から20文字分探索して、ヒットする文字列長をTrie木で探す。
 * これをmlenとすると、[0,i]から[0,i+(mlen-1)]まではいずれも作れる、としてよい。文字列の合成があるので。
 * 合成したい長さは最大で1.5*10^5, 各DNAの長さは最大20なので、間に合う。
 * 
 * 上記を実装するため、dp[i]を以下のように定義する。
 *   [0,i] まで作るのに必要な操作回数
 *   作れない場合はINF.
 * 初期値としてdp[0]=0で良い。
 * 「必ず合成できる」的な制約より、dp[0]も適切に更新される。
 * 
 * メモリ制限が厳しく、当初MLE.
 * Trie<26, 'A'> tr; ⇒ Trie<20, 'A'> tr; に変更してAC.
 * ※出現する文字はATGCの4種類なので、最も後ろの文字までにしたという意図。
 *   これでもダメだったら、ATDC⇒ABDCとかに変換する形か。
**/

// https://algo-logic.info/trie-tree/
// Trie<26, 'A'> trie; のように定義する
// (1)insert(word) : 文字列wordをトライ木に追加する
// (2)search(word, prefix=true/false) : 文字列がトライ木に追加されているか検索
// (3)word_count() : 追加した単語数を返す
// (4)size() : 頂点の総数
template <int char_size, int base>  // 文字の種類と、0に対応する文字
struct Trie {
private:
	struct Node {
		vector<int> next;  // next[ch]:文字chである子の頂点番号 存在しなければ-1
		vector<int> accept;  // 末端がこの頂点である文字列のidたち
		int c;  // この頂点が持つ文字を、base基点で数値で表したもの 'A'なら0, 'B'なら1, など
		int depth;  // 根からの距離
		int common;  // この頂点を共有している文字列の個数
		Node(int c_, int dep_) : c(c_), depth(dep_), common(0) {
			next.assign(char_size, -1);
		}
	};
	vector<Node> nodes;  // トライ木本体
	int root;  // 根 (コンストラクタでしか使わないが、明示的に)

	// 単語wordをid番目として追加
	void insert(const string &word, int id)	{
		int node_id = 0;  // node_id=0は根(文字が入っていない)であることに注意
		for(int i = 0; i < (int)word.size(); i++) {
			// 以下、node_idから文字cへたどった先がnext_idになる
			int c = (int)(word[i] - base);
			int &next_id = nodes[node_id].next[c];
			if(next_id == -1) {  // 文字cが木に存在しなければ追加
				next_id = (int)nodes.size();
				nodes.push_back(Node(c, nodes[node_id].depth+1));
			}
			nodes[node_id].common++;
			node_id = next_id;
		}
		// word末尾の文字は、このタイミングでのnode_idである点に注意
		nodes[node_id].common++;
		nodes[node_id].accept.push_back(id);
	}

public:
	Trie() : root(0) {
		nodes.push_back(Node(root, 0));  // 始めは根のみ
	}
	// wordをトライ木に追加
	void insert(const string &word) {
		insert(word, nodes[0].common);
	}
	// wordの探索
	// prefix=false : wordがTrie木に存在すればtrue
	// prefix=true : wordそのものでなくとも、wordを接頭辞(prefix)として持つ単語が存在すればtrue
	bool search(const string &word, int &mlen, bool prefix = false) {
		int node_id = 0;
		int i;
		for(i = 0; i < (int)word.size(); i++) {
			int c = (int)(word[i] - base);

			// 本問用にadd
			if(nodes[node_id].accept.size() > 0)
			{
				chmax(mlen, i);
			}

			int &next_id = nodes[node_id].next[c];
			if(next_id == -1) {
				return false;  // 次の頂点が存在しない
			}
			node_id = next_id;
		}
		// word末尾の文字は、このタイミングでのnode_idである点に注意
		// 本問用にadd
		if(nodes[node_id].accept.size() > 0)
		{
			chmax(mlen, i);
		}

		if(prefix) {
			return true;  // 接頭辞であれば、ここまで来た時点で確実に存在する
		}
		else {
			return nodes[node_id].accept.size() > 0;  // 検索終端が末端となるような頂点があるか
		}
	}
	// 追加した単語数
	int word_count() const {
		return nodes[0].common;
	}
	// 頂点の総数 (根を含むので文字数+1になる)
	int size(void) const {
		return (int)nodes.size();
	}
};

int main(void)
{
	int i, j;
	int N; cin >> N;
	string td; cin >> td;
	int tdlen = (int)td.size();
//	Trie<26, 'A'> tr;
	Trie<20, 'A'> tr;
	for(i = 0; i < N; i++)
	{
		string s; cin >> s;
		tr.insert(s);
	}

	vector<int> dp(tdlen+1, INF32);
	dp[0] = 0;
	// 配るDP
	for(i = 0; i < tdlen; i++)
	{
		auto subs = td.substr(i, 21);  // tdの末尾を超える分はカットしてくれる
		int mlen = 0;
		tr.search(subs, mlen);
		for(j = 0; j < mlen; j++)
		{
			chmin(dp[i+j], dp[i]+1);
		}
	}
	cout << dp[tdlen-1] << endl;

	return 0;
}
