Pruned Landmark Labeling
========================

## Usage

### From CUI Interface
    e.g:
    $ python ./src/centrality_cal.py ./maps/demo.txt
    $ make
    $ bin/construct_index ./maps/demo.txt index_file
    $ bin/query_distance index_file <<< "1 4"
    查询结果:2
* Execute `./src/centrality_cal.py` to generate a order for PLL based on one specific centrality.
* Execute `make` to build programs.
* Execute `bin/construct_index` to construct an index from a graph.
* Execute `bin/query_distance` and write pairs of vertices to STDIN to query distance between pairs of vertices.

## References
* Takuya Akiba, Yoichi Iwata, and Yuichi Yoshida, **[Fast Exact Shortest-Path Distance Queries on Large Networks by Pruned Landmark Labeling](http://www-imai.is.s.u-tokyo.ac.jp/~takiba/papers/sigmod13_pll.pdf)**.
In *SIGMOD 2013*, to appear.
