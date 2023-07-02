
// 幅優先探索のメモ

/*
 * キューを使って解いていくことが多い。
 * テンプレートとしてgraph.cppを参照。
 * 
 * graph.cppのテンプレート以外では、迷路の問題を幅優先探索で解くことがある。(こちらもキューを使う)
 * ARC005-C.cppやアルゴリズム本の13章末問題13.4(problem13-4.cpp)を参照。
 * 
 * 主に、重み無しグラフ(移動コストが0)の場合に、キューを用いた単純なBFSで解ける問題となる。
 * 迷路を解く問題や、2頂点が連結しているかを判定する問題など。
 * 各辺のコストが異なる場合は、ベルマン・フォード法やダイクストラ法を使うことになる。
 * ※これらの問題も単純なBFSで解くことはできるが、無駄な探索が増えてTLEになりやすい。
 *   例：ABC237のE問題について、単純なBFSで解いたらTLEになり(ABC237-E.cpp)、
 *       ダイクストラ法で解いたらACとなった(ABC237-E-2.cpp)
 * 移動コストが0 or 1のケースは、0-1 BFSも使える。
 * 
 * [関連する問題]
 * ABC308-D
 * ABC299-E
 * ABC292-D
 * ABC289-E 移動するものが2つ
 * ABC285-D サイクル検出, 連想配列
 * ABC277-C
 * ABC272-D
 * ABC269-D
 * ABC259-D
 * ABC257-D
 * ABC232-D
 * ABC231-D サイクル検出
 * ABC226-E
 * ABC132-E
 * ABC277-E 0-1 BFS
 * ARC005-C 0-1 BFS
 * 
 * 0-1 BFSについて：
 * 重み付きグラフの辺の重みが0 or 1、または移動コストが0 or 1のときに使える手法。
 * 両端キューを使い、重みが0であればキューの先頭に、1であれば末尾に追加する。
 * 以下URLを参考。
 *   https://betrue12.hateblo.jp/entry/2018/12/08/000020
 *   https://drken1215.hatenablog.com/entry/2021/07/30/024800
 * 
 */
