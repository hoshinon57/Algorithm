#include <iostream>
#include <vector>
#include <cassert>
using namespace std;
typedef long long ll;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

/*
 * トライ木のライブラリ
 * 基本的には、問題ごとにsearch()を書き換える必要がありそう
 * 
 * [ざっくり概要]
 * ・文字列の接頭辞(prefix)の共通部分を共有して保存することで、文字列の長さをMとしてO(M)での検索が可能。
 * ・1つの頂点が1つの文字を表す。
 * ・ルートには空の文字列が対応する。
 * 
 * [参考資料]
 *   https://algo-logic.info/trie-tree/
 *   https://ja.wikipedia.org/wiki/%E3%83%88%E3%83%A9%E3%82%A4_(%E3%83%87%E3%83%BC%E3%82%BF%E6%A7%8B%E9%80%A0)
 *   https://ei1333.github.io/luzhiled/snippets/structure/trie.html
 * 
 * [関連する問題 / verifyした問題]
 * ABC287-E search()にてcommon>=2となるうちの最も深い(depth)頂点を探索
 */

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
		vector<int> accept;  // 末端がこの頂点である文字列の頂点番号たち
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
		int node_id = 0;
		for(int i = 0; i < (int)word.size(); i++) {
			int c = (int)(word[i] - base);
			int &next_id = nodes[node_id].next[c];
			if(next_id == -1) {  // 文字cが木に存在しなければ追加
				next_id = (int)nodes.size();
				nodes.push_back(Node(c, nodes[node_id].depth+1));
			}
			nodes[node_id].common++;
			node_id = next_id;
		}
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
	bool search(const string &word, bool prefix = false) {
		int node_id = 0;
		for(int i = 0; i < (int)word.size(); i++) {
			int c = (int)(word[i] - base);
			int &next_id = nodes[node_id].next[c];
			if(next_id == -1) {
				return false;  // 次の頂点が存在しない
			}
			node_id = next_id;
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
	Trie<26, 'A'> trie;
	trie.insert("FIRE");
	assert( trie.search("FIRE"));
	assert(!trie.search("FI"));
	assert( trie.search("FI", true));  // "FI"をprefixに持つ単語は存在する
	assert(!trie.search("FIREMAN", true));
	trie.insert("FIREMAN");
	assert( trie.search("FIREMAN"));
	assert(!trie.search("FIREMA"));
	assert( trie.search("FIREMA", true));
	assert(trie.word_count() == 2);
	trie.insert("FILE");
	assert(trie.size() == 9+1);  // FIREMAN + LE + 1(root)
	return 0;
}
