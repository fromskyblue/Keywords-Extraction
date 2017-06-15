#include <iostream>
#include <vector>
#include <cmath>
#include <map>
using namespace std;


class Term{

public: 
	string word; //需要的
	int frequency; //需要的
	double average_distance;
	vector<int> t_loc; //需要的
	vector<int> d_list; //需要的
	double ED; 
	double EDgeo;
	double EDnor; //需要的
	
	vector<int> IN_d_list;
	double HI;

	vector<int> EX_d_list;
	double HE;



public: 
	void Set_Term(string a,vector<int> b,int c,double d){
			word=a;
			t_loc=b;
			frequency=c;
			average_distance=d/frequency;
		}

		~Term(){};

		void divide_Mode(){
		
			int i;

			for (i=0;i<d_list.size();i++)
			{
				if(d_list[i]>average_distance)
					EX_d_list.push_back(d_list[i]);
				if(d_list[i]<=average_distance)
					IN_d_list.push_back(d_list[i]);
			}
		}

		void Cal_Distance(int sum){

			int num=t_loc.size();
			if(num==1)
				d_list.push_back(sum); //只出现了一次的词

			for(int i=0;i<num-1;i++)
				d_list.push_back(t_loc[i+1]-t_loc[i]);

			d_list.push_back(t_loc[0]+sum-t_loc[num-1]);		
		}


		void Cal_Entropy_MAX(){
			map<int,double> infrequency,exfrequency;
			int i;
			double size;
			double sum=0;
			

			for(i=0;i<IN_d_list.size();i++)
				++infrequency[IN_d_list[i]];

			size=infrequency.size();
			HI=log10(size)/log10(2.0);
			
		 for(i=0;i<EX_d_list.size();i++)
				++exfrequency[EX_d_list[i]];

		 size=exfrequency.size();
			HE=log10(size)/log10(2.0);
			

			ED=pow(HI,2)-pow(HE,2);

		}


		void Cal_Entropy(){
			map<int,double> infrequency,exfrequency;
			int i,num;
			double sum=0;
			double temp;

			num=IN_d_list.size();
			for(i=0;i<IN_d_list.size();i++)
				++infrequency[IN_d_list[i]];
			map<int,double>::iterator map_it=infrequency.begin();
			while(map_it!=infrequency.end()){
				temp=(map_it->second)/num;
				sum=sum+temp*(log10(temp)/log10(2.0));

				++map_it;
			}
		
			HI=-sum;
			

			sum=0;
			num=EX_d_list.size();
		   for(i=0;i<EX_d_list.size();i++)
				++exfrequency[EX_d_list[i]];

			map<int,double>::iterator map_it1=exfrequency.begin();
			while(map_it1!=exfrequency.end()){
				temp=(map_it1->second)/num;
				sum=sum+temp*(log10(temp)/log10(2.0));

				++map_it1;
			}

			HE=-sum;

			ED=pow(HI,2)-pow(HE,2);

		}


		void CAL_geo(int sum){
			double temp,temp1;;
			double PI,PE,HIgeo,HEgeo;
			PI=PE=HIgeo=HEgeo=0.0;
			int i;
			temp=frequency*1.0/sum;
	        int New_mean=floor(average_distance);

			for(i=1;i<New_mean;i++)
				PI=PI+temp*pow(1-temp,i-1);
			
			PE=1-PI;
			
			for(i=1;i<New_mean;i++){
				temp1=temp*pow(1-temp,i-1)/PI;
				HIgeo=HIgeo+temp1*(log10(temp1)/log10(2.0));
			}

			for(i=New_mean;i<=sum;i++){
				temp1=temp*pow(1-temp,i-1)/PE;
				HEgeo=HEgeo+temp1*(log10(temp1)/log10(2.0));
			}

			HEgeo=pow(-HEgeo,2);
			HIgeo=pow(-HIgeo,2);

			EDgeo=HIgeo-HEgeo;

		}

		void Cal_EDnor(){
			if(EDgeo==0)
				EDnor=-1000;
			else
			{EDnor=ED/fabs(EDgeo);}

}

};