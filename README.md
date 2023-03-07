Pruned Landmark Labeling
========================

## Usage

### From CUI Interface
    e.g:
    计算centrality：
    $python ./centrality_cal.py ../maps/test.txt
    打乱排序：
    注意： 打乱后需要跑PLL 并手动记录
    $python shuffle.py test.txt
    计算kt：
    $python kt_cal.py test.txt
    运行pll：
    $ g++ script.cpp -o script
    $ script
