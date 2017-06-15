# Keyword Extraction Based On Entropy Difference

### Method design

We have mainly calculated the three methods:
+ clean_words: standardization document: Removing punctuation,all letters lower case
+ remove_stop: Remove stop word
+ get_keyword: The entropy of each word is calculated according to this method, and the result dictionary is returned after ordering


### Usage

``` python
from Keyword_Extraction import get_keyword, clean_words, remove_stop
text = ''' Arbitrary text '''
get_keyword(remove_stop(clean_words(text)))
```
