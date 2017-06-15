// Keyword_Extraction.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Keyword_Extraction.h"



 void Swap(Node &p,Node &q)                          
{                                                      
	Node temp = p;
	p=q;
	q=temp;
} 

//Partition function
int Partition(Node ArrayInput[],int nLow,int nHigh)                
{                                                 

	double nTemp=ArrayInput[nHigh].EDnor;   
	int i = nLow, j=nLow-1;  
	for(; i<nHigh; i++)
	{
		if( ArrayInput[i].EDnor>=nTemp )
		{
			j++;
			if(i !=j )
			{
				Swap(ArrayInput[i], ArrayInput[j]);
			}
		}
	}

	Swap(ArrayInput[j+1],ArrayInput[nHigh]);

	return (j+1);                                        
}

//Quick sort
 void Quick_sort(Node ArrayInput[],int nLow,int nHigh)            
{                                                                                                       
	if(nLow < nHigh)                                        
	{                                                
		int nIndex=Partition(ArrayInput , nLow, nHigh);                         
		Quick_sort(ArrayInput , nLow, nIndex-1);                           
		Quick_sort(ArrayInput , nIndex+1, nHigh);                           
	}                                                 
}

 Keyword_API Node* keyword_extra_entropy_MAX(string text){

	 //算法部分
	 hash_map<string,int> word_frequency;
	 hash_map<string,vector<int>> word_loc;


	 stringstream q;
	 q.str(text);
	 int i=0;
	 while(q){

		 string asd,we;
		 q>>asd;

		 we.resize(asd.size());
		 transform(asd.begin(),asd.end(),we.begin(),tolower);

		 if(we.empty())
			 continue;


		 i++;
		 ++word_frequency[we];
		 word_loc[we].push_back(i);

	 }

	 int sum=i;
	 vector<Term> Term_list;
	 Term temp;
	 hash_map<string,vector<int>>::const_iterator map_it=word_loc.begin();

	 vector<int> r;
	 while(map_it!=word_loc.end()){	
		 Term temp;
		 temp.Set_Term(map_it->first,map_it->second,map_it->second.size(),sum);
		 Term_list.push_back(temp);
		 ++map_it;
	 }

	 vector<Node> node_temp;
	 Node p;


	 for(i=0;i<Term_list.size();i++){
		 Term_list[i].Cal_Distance(sum);
		 Term_list[i].divide_Mode();
		 Term_list[i].Cal_Entropy_MAX();
		 Term_list[i].CAL_geo(sum);
		 Term_list[i].Cal_EDnor();

		 if(_isnan(Term_list[i].EDnor))
			 continue;

		 p.EDnor=Term_list[i].EDnor;
		 p.d_list=Term_list[i].d_list;
		 p.frequency=Term_list[i].frequency;
		 p.t_loc=Term_list[i].t_loc;
		 p.word=Term_list[i].word;
		 node_temp.push_back(p);
	 }

	 Node *result;
	 result=new Node[node_temp.size()];

	 for(i=0;i<node_temp.size();i++){
		 result[i].frequency=node_temp[i].frequency;
		 result[i].word=node_temp[i].word;
		 result[i].EDnor=node_temp[i].EDnor;
		 result[i].t_loc=node_temp[i].t_loc;
		 result[i].d_list=node_temp[i].d_list;
	 }
	 Quick_sort(result,0,node_temp.size()-1);

	 return result;

 }




Keyword_API Node* keyword_extra_entropy(string text){

	//算法部分
	hash_map<string,int> word_frequency;
	hash_map<string,vector<int>> word_loc;


	stringstream q;
	q.str(text);
	int i=0;
	while(q){

		string asd,we;
		q>>asd;

		we.resize(asd.size());
		transform(asd.begin(),asd.end(),we.begin(),tolower);

		if(we.empty())
			continue;


		i++;
		++word_frequency[we];
		word_loc[we].push_back(i);

	}

	int sum=i;
	vector<Term> Term_list;
	Term temp;
	hash_map<string,vector<int>>::const_iterator map_it=word_loc.begin();

	vector<int> r;
	while(map_it!=word_loc.end()){	
		Term temp;
		temp.Set_Term(map_it->first,map_it->second,map_it->second.size(),sum);
		Term_list.push_back(temp);
		++map_it;
	}

	vector<Node> node_temp;
	Node p;


	for(i=0;i<Term_list.size();i++){
		Term_list[i].Cal_Distance(sum);
		Term_list[i].divide_Mode();
		Term_list[i].Cal_Entropy();
		Term_list[i].CAL_geo(sum);
		Term_list[i].Cal_EDnor();

		if(_isnan(Term_list[i].EDnor))
			continue;

        p.EDnor=Term_list[i].EDnor;
		p.d_list=Term_list[i].d_list;
		p.frequency=Term_list[i].frequency;
		p.t_loc=Term_list[i].t_loc;
		p.word=Term_list[i].word;
		node_temp.push_back(p);
	}

	Node *result;
	result=new Node[node_temp.size()];

	for(i=0;i<node_temp.size();i++){
	result[i].frequency=node_temp[i].frequency;
	result[i].word=node_temp[i].word;
	result[i].EDnor=node_temp[i].EDnor;
	result[i].t_loc=node_temp[i].t_loc;
	result[i].d_list=node_temp[i].d_list;
	}
	Quick_sort(result,0,node_temp.size()-1);

	return result;

}