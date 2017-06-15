# Zhen Yang-Keywords Extraction

### Keyword extraction by entropy difference between the intrinsic and extrinsic mode

We strive to propose a new metric to evaluate and rank the relevance of words in a text. The method uses the Shannon’s entropy difference between the intrinsic and extrinsic mode, which refers to the fact that relevant words significantly reflect the author’s writing intention, i.e., their occurrences are modulated by the author’s purpose, while the irrelevant words are distributed randomly in the text. By using The Origin of Species by Charles Darwin as a representative text sample, the performance of our detector is demonstrated and compared to previous proposals. Since a reference text ‘‘corpus’’ is all of an author’s writings, books, papers, etc. his collected works is not needed. Our approach is especially suitable for single documents of which there is no a priori information available.

### Project Members

*   Zhen Yang

### Highlights

*   We propose a new metric to evaluate and rank the relevance of words in a text.

*   The metric uses the Shannon’s entropy difference between the intrinsic and extrinsic mode.

*   We believe that this work is a new result in keyword extraction and ranking.

*   Our approach is especially suitable for single documents of which there is no a priori information available.

### Publication

*   [[2013] Yang Z, Lei J, Fan K, Lai Y. “Keyword Extraction by Entropy Difference Between the Intrinsic and Extrinsic Mode.” Physica A: Statistical Mechanics and its Applications 392(19): 4523-4531.](http://www.sciencedirect.com/science/article/pii/S0378437113004949)

### Code & Toolbox

*   Github page (Soon!)

*   [Codeproject page](https://www.codeproject.com/Articles/643619/Keyword-Extraction-Based-On-Entropy-Difference)

*   [SPROUT toolbox](http://ace.autotutor.org/downloads/sprout.1.0.0.0.zip), developed by [Prof. Zhiqiang Cai](http://yzlab.net/zcai.autotutor.org), which use our algorithm to extract keywords for target corpus, and then use the keywords to find extra articles on wiki to expand the corpus.

### Update

1.  张龙伯. 基于多尺度划分的关键词检测算法, 北京工业大学硕士学位论文，2014.
    张龙伯. 基于多尺度划分的关键词检测系统, 计算机软著(登记号: 2014SRBJ0226)，2014.

    1.  在YANG’ 13算法的基础上加入多尺度分析方法。

    2.  对文章进行多尺度划分的方法，综合考虑词语在各个粒度下的分布特性，计算词语的主题相关度，从而有效的检测出文本中的关键词。

    3.  对文章《物种起源》进行关键词检测，性能明显提升，得到top19准确率100%的性能。