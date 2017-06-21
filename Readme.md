# Zhen Yang-Keywords Extraction

### Keyword extraction by entropy difference between the intrinsic and extrinsic mode

We strive to propose a new metric to evaluate and rank the relevance of words in a text. The method uses the Shannon’s entropy difference between the intrinsic and extrinsic mode, which refers to the fact that relevant words significantly reflect the author’s writing intention, i.e., their occurrences are modulated by the author’s purpose, while the irrelevant words are distributed randomly in the text. By using The Origin of Species by Charles Darwin as a representative text sample, the performance of our detector is demonstrated and compared to previous proposals. Since a reference text ‘‘corpus’’ is all of an author’s writings, books, papers, etc. his collected works is not needed. Our approach is especially suitable for single documents of which there is no a priori information available.

### Project Members

*   Zhen Yang
*   Chen Weitong
*   Li Hanchen
*   Li Chaoyang
*   E Youjun

### Background

One of the most significant different between human-written texts and monkeys typing is the general existence of meaningful topics in human written texts.keyword/relevant word extraction and ranking are the staring point for critical tasks like topic detection and tracking in written texts,and they are widely applied in information extraction,selection and retrieval.

![Figure 2: Intrinsic mode and extrinsic mode in positions of word-type occurrences in text.](https://lh4.googleusercontent.com/-xu5f5vSp6zY/UiyNC83FL1I/AAAAAAAAAA4/xBC-WA9gxPY/w798-h347-no/2.png)


Here's the brief introduction of the principle of the algorithm, it can help you understand and use the dll and software better. The idea of intrinsic-extrinsic mode is based on the general idea that highly significant words tend to be modulated by the writer's intension, while common words are essentially uniformly spread throughout a text. So the intrinsic mode denotes the statistical properties of the appearance of a relevant word within a topic, i.e., the statistical properties of clustering within each topic. Meanwhile, the extrinsic mode captures the statistical properties of the disappearance of a word clustering along a written text and it characterizes the relationship between word clustering occurrence within a topic and an author's written style. As shown in FIG. 2\. the distances between two words which is successive occurrences is defined as _d**i**_**= _t_**_i_+1 − _t**i**_**. _T_**_i_ is the position of the word in the text. The arrival time difference _d**i_belongs to the intrinsic mode if _d**i_<μ. In other words, a given occurrence of the word is a part of an intrinsic mode if its local separation is less than its mean waiting time. Let _d**I**_**= {_d_**_i_|_d**i**_**<μ} be the union set for all _d_**_i_<μas shown in the bottom-left figure in FIG. 2\. We found through experiments, that the keyword which appears in the article presents the characteristics of aggregates. so its intrinsic mode entropy is large while its extrinsic mode entropy is small; the general words are evenly distributed in the article, any two consecutive word spacing appears little change, so the entropy difference between the intrinsic and extrinsic mode is small. In this way, you can use the value E which is the entropy difference between the intrinsic and extrinsic mode to extract keywords. In practice, in order to eliminate the words randomly distributed and boundary conditions, we use the _C~c ~ _boundary conditions and the normalized entropy difference _E_~nor~ as the final indicators. If you want to learn more details of this algorithm, please click here (<http://dx.doi.org/10.1016/j.physa.2013.05.052>) to view the full paper.


### Highlights

*   We propose a new metric to evaluate and rank the relevance of words in a text.

*   The metric uses the Shannon’s entropy difference between the intrinsic and extrinsic mode.

*   We believe that this work is a new result in keyword extraction and ranking.

*   Our approach is especially suitable for single documents of which there is no a priori information available.

### Publication

*   [[2013] Yang Z, Lei J, Fan K, Lai Y. “Keyword Extraction by Entropy Difference Between the Intrinsic and Extrinsic Mode.” Physica A: Statistical Mechanics and its Applications 392(19): 4523-4531.](http://www.sciencedirect.com/science/article/pii/S0378437113004949)

### Please Note

If you use the keyword extraction software or dynamic link library (dll) in your program or research, please indicate that the part of paper and program cites the following paper.All the source code in the article can be fully copy.

l **Zhen YANG, Jianjun LEI, Kefeng FAN, Yingxu LAI. ****Keyword Extraction by Entropy Difference between the Intrinsic and Extrinsic ****Mode, Physica A: Statistical Mechanics and its Applications, 392 (2013),**** 4523-4531\. **<http://dx.doi.org/10.1016/j.physa.2013.05.052>

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
