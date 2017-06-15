# Keyword Extraction Based On Entropy Difference

![](https://www.codeproject.com/script/Membership/ProfileImages/%7B26b5cac9-deec-40a5-ab60-a11b64d0fb00%7D.jpg)

Here ,we provide simple and practical keyword extraction software and dll for long text

- [Download Keyword_Extraction_Based_On_Entropy_Difference-noexe.zip - 8.8 MB](https://www.codeproject.com/KB/DLL/643619/Keyword_Extraction_Based_On_Entropy_Difference-noexe.zip)
- [Download Keyword_Extraction_Based_On_Entropy_Difference.zip - 8.9 MB](https://www.codeproject.com/KB/DLL/643619/Keyword_Extraction_Based_On_Entropy_Difference.zip)



## Introduction
In this software, we use a kind of entropy difference measure to extract the keywords in a text. It's a simple measure, without any a priori information and effectively extract the keywords in a single text. Here we provide the dll which is developed by C++ and C# language. We also provide keywords extraction software for you to use which is developed by C#. By using this software, the only thing you need to do is set the path of files to be done, and the software can help you finish the rest of work. For English text, you should ensure that the text is a standard format, or you can use the "pretreatment" function in the software to format the text. Then, you can select one of the methods-general entropy or Maximum Entropy to extract the keywords.

![Figure1: software interface ](https://lh5.googleusercontent.com/-ANXB5lR2E4g/UiyNC-opGsI/AAAAAAAAAAs/4wjFr4hsp6I/w735-h552-no/1.png "Figure1: software interface")



For Chinese text, in order to format the text, you should follow the next steps. First, remove the punctuation and charts in the text. Then, divide the sentences into a list of words. Two successive words are separated by a space. Here we provide the function to remove the punctuation and charts. You should ensure that the sentences in the text has been divide into a list of words. Last, you can select one of the methods----general entropy or Maximum Entropy to extract the keywords.

The **Standardization *** *text** will be given in the part of usage. With these things, you can easily complete text keyword extraction work!


## Usage

Now we will make a detailed description for the keyword extraction software and use of dll. Here are two samples that we will use in numerous examples to illustrate the performance of Enor metric, one is scientific book in English, the other is a news report in Chinese.


**Please Note:** To start the evaluation of the text, any punctuation symbols were removed from the text, all words were changed to lowercase and then a simple tokenization method based on whitespaces was applied. For the Chinese text, the extra chinese word segmentation would be done at first.


In the keyword extraction software, we also provide you with a text pre-processing function. After pre-processing, the standardized format of text as follows:


**Standardization text input:** can any body hear me oh am I talking to myself my mind is running empty In the search for someone else cause tonight I'm feeling like an astronaut......


### Using the software
First, please click the icon to start the keyword extraction software. According to the flow shown in the following figure, you can conduct a keyword extraction process.

 <p>
  <img src="https://lh5.googleusercontent.com/-c3GHj4uQkM0/UiyNCwn5r7I/AAAAAAAAAA0/Q6MK7dUsoFA/w409-h552-no/5.png" alt="">
### Using the dll
  <p> The using of the dll we provided here is fairly simple, as long as you are familiar with C++ or C# dll calling, you will be able to easily use it.<o:p>

  <p>
  <strong> Using the dll of C++<strong></strong>：</strong>
</p>
  <p>  Please note: Before using the dll of the C++ version, please set you Visual Studio (VS2010) as follows:<o:p>

  <p>Open your Visual Studio, click the menu project -&gt; Properties -&gt;Configuration Properties -&gt; C/C++ -&gt; Code Generation -&gt; Runtime library, select Multi-threaded (/MD)</p>
  <p> Here provides two versions of the C++ dll, release version of dll and debug version of dll, please select the corresponding dll for use. Such as, using the dll in the "release" folder if you want to compile your code with the solution configuration as “realse” method. We recommend to use the release version because it's faster than the debug version.</p>
  <p>
##### Step1:
</p>
  <p> In the unzipped folder find dll (in the c++ folder) and click the release folder. There will be three file in the folder,as show in the following picture.Then,copy these three files in your project and import the "Node.h" file in your project directory <o:p>
</o:p></p>
  <p>
  <img src="https://lh5.googleusercontent.com/-w5SjR9gxSe4/UiyNDwuARuI/AAAAAAAAABI/PIjR6aIS6Yk/w166-h63-no/7.png" alt="">
</p>

##### Step2:

 <p>  Add the head file “Node.h” in your code as follow，Please note：The order of variables in the structure in "Node.h" can not be changed!  Now,we introduce the structure NODE,as follow</p>


```
#include<vector>
using namespace std;
typedef struct
{
	string word;  //word
	double EDnor; //Entropy difference, the greater the value is, the more critical the word
	int frequency; //Frequency of the word
	vector<int> t_loc; //the position of the word appeared in the text
	vector<int> d_list; //the distance between two consecutive words
}Node;  //the structure which the dll returns contains key information

#pragma comment (lib,"Keyword_Extraction.lib")
//input: string text ,the text after pretreatment
//int &num ,return the size of the Node array
//return: Node* ,return the keyword array
extern Node* keyword_extra_entropy(string text,int&num);  // return the keyword array with the                                                           //general entropy method
extern Node* keyword_extra_entropy_MAX(string text,int&num);  // return the keyword array with                                                               //the Maximum entropy method
```

The dll encapsulates two functions: Node *keyword_extra_entropy (string text, int & num) and Node *keyword_extra_entropy_MAX (string text, int & num). Respectively, first function use general entropy method, while to second function use maximum entropy method to calculate the maximum entropy. Both of the functions have two inputs: string type - preprocessed text; int type - return the size of the array. output: Node* type - the array of type Node, the structure Node include the content which is introduced above.

##### Step3:
After following above steps, you can call the function to get keywords, such as the following code to showing TOP-10 Keywords:

```
int i;
int num;
Node *result;
result=keyword_extra_entropy_MAX(text,num);
for(i=0;i<10;i++)
	 cout<<endl<<result[i].word<<"==="<<result[i].EDnor<<"==="<<result[i].frequency;
```
##### Example:
Now, we select the book "Origin of Species" as an example, and demonstrate the whole process of using the dll:

```
#include<fstream>
#include<iostream>
#include<string>

#include"Node.h"

void main(){

	//read the whole txt file in string type variable
	filebuf *pbuf;  
	ifstream filestr;  
	long size;  
	char * buffer;  
	filestr.open ("D:\\test.txt",ios::binary);  //please change the file
	                                            //path according to your actual situation
	pbuf=filestr.rdbuf();  
	size=pbuf->pubseekoff
		(0,ios::end,ios::in);  
	pbuf->pubseekpos (0,ios::in);  
	buffer=newchar[size+1];  
	pbuf->sgetn (buffer,size);  
	buffer[size]='\0';
        filestr.close();  
	string text=buffer;

	//Call the function to
	//extract keywords in the dll
	int num;
	Node *result;
	result=keyword_extra_entropy_MAX(text,num);

	//output all keywords in the array,
	//here "num" is the size of the array.
		for(int i=0;i<num;i++)
		cout<<endl<<result[i].word<<"==="<<result[i].EDnor<<"==="<<result[i].frequency;

	system("pause");
}  
```
### Using the dll of C#
	The dll of C# version packages the entire class, so there contains more functions than the dll of C++ version(including preprocessing functions, etc.). Please refer to file "KEBOED interface documentation" to learn the usage of C# dll.

#### The results of  experiments：
For the English example, here we select "Origin of Species" by using our keyword extraction software, and select the "maximum entropy" keyword extraction method,<o:p /> 

![](https://lh3.googleusercontent.com/-cFXNhRRaZy8/UiyND4EYm4I/AAAAAAAAABE/6wZYvjPv4pA/w309-h187-no/8.png) 

For the Chinese sample, we have chosen a news report on the network, the title of this report is《让雷锋精神代代相传》. We use the keyword extraction software, and select the "maximum entropy" keyword extraction method and get the following result:  


![](https://lh3.googleusercontent.com/-FIE3d36TkAA/UiyNAXIvXfI/AAAAAAAAAAg/n4T51oRkbBc/w312-h186-no/9.png)

 This two samples text will also be given in the compressed package.

### Conclusion
  In summary, understanding the complexity of human written text requires an appropriate analysis of the statistical distribution of the words in texts. We find highly significant words tend to be modulated by the writing writer’s intension, while common words are essentially uniformly spread in a text. The ideas of this work can be applied to any natural language with words clearly identified, without requiring any previous knowledge about semantics or syntax.  
