//**************************************************
//海量图数据的管理和挖掘  第一次作业
//信息科学技术学院 计算机研究所  张晓德  1601214529
//zhangxiaode@pku.edu.cn
//2016.11.1
//功能：实现了论文Lei Zou, Yansheng Lu, Huaming Zhang, Rong Hu: PrefixTreeESpan: A Pattern Growth Algorithm for Mining Embedded Subtrees. WISE 2006: 499-505.
//中的PrefixTreeEspan算法来挖掘embedded substructure
//*************************************************
#include<iostream>
#include<list>
#include<vector>
#include<string>
#include<stdlib.h>
#include <algorithm>
#include<map>
#include <time.h>
using namespace std;
int TFP=0;        //total frequent pattern
struct TreeNode   //struct tree node ,include key and its ancestors
{
	int key;
	vector<int> Ancestor;
	TreeNode()
	{

	}
};

struct GE      //struct growth element,include key and ancestor
{
	 bool operator <(const GE& other) const
     {
         if (key < other.key)        
         {
             return true;
         }
         else if (key == other.key)  
         {
             return ancestor < other.ancestor;
         }
         
         return false;
     }

	 bool operator == (const GE & G)
	 {
		 return G.ancestor==ancestor&&G.key==key;
	 }

	int key;
	int ancestor;
};
vector<string> CutString(string source,string separator)
	//split the source string to several sub strings accoring to the separator
{
	vector<string> result;
    string cut;
	int pos=source.find(separator);
	while(pos!=-1)
	{
		cut=source.substr(0,pos);
		source=source.substr(pos+separator.length());
		pos=source.find(separator);
		result.push_back(cut);  
	}
	result.push_back(source); 
	return result;
}
void FindFrequentGEs(list<vector<TreeNode>> & Database,int minsup,vector<GE> & FreGEs)
	//find the frequent growth elements
{
	map<GE,int> CntFP;  //count the frequent of growth element
	CntFP.clear();
	for(list<vector<TreeNode>> ::iterator Lite=Database.begin();Lite!=Database.end();Lite++)
	{
		//vector<GE> ItemGEType;
		//ItemGEType.clear();
		GE temp;
		for(int i=0;i<(*Lite).size();i++)
		{
			if((*Lite)[i].key!=-1)
			{
				for(int j=0;j<(*Lite)[i].Ancestor.size();j++)
				{
					temp.key=(*Lite)[i].key;
					temp.ancestor=(*Lite)[i].Ancestor[j];
					//vector<GE>::iterator res=find(ItemGEType.begin(),ItemGEType.end(),temp);
					//if(res==ItemGEType.end())
					{
						//ItemGEType.push_back(temp);
						map<GE,int>::iterator MapF;
						MapF=CntFP.find(temp);
						if(MapF==CntFP.end())
						{
							CntFP.insert(pair<GE,int>(temp,1));
						}
						else
							MapF->second++;
					}
				}
			}
		}
	}

	for(map<GE,int>::iterator M=CntFP.begin();M!=CntFP.end();M++)
	{
		if(M->second>=minsup)
			FreGEs.push_back(M->first);
	}

}
void RecursiveMineFreqpattern(vector<GE> S,int n,list<vector<TreeNode>> Database,int minsup)
{
	//scan projected database once to find all frequent Ges b
	vector<GE> b;
	FindFrequentGEs(Database,minsup,b);

	list<vector<TreeNode>> ProjectedDatabase;
	for(vector<GE>::iterator I=b.begin();I!=b.end();I++)
    //for each GE b
	{
		ProjectedDatabase.clear();
		S.push_back(*I);
		for(int i=0;i<S.size();i++)
		{
			printf("%d  ",S[i].key,-1);
		}
		TFP++;
		printf("\n");
		
		//find all occurrence of b in projected database and construct new projected database
		for(list<vector<TreeNode>> ::iterator Lite=Database.begin();Lite!=Database.end();Lite++)
		{
			bool flag=false;
			int i=0;
			vector<TreeNode> TItem;
			for(;i<(*Lite).size();i++)
			{
				if((*Lite)[i].key==(*I).key)
				{
					//-------------------------------
					
					TItem.clear();
					int k=i+1;
					i++;
					int sum=0;
					if(k<(*Lite).size())
					{
						if((*Lite)[k].key==-1)
							sum+=-1;
						else
							sum+=1;
						while(sum!=-1)
						{
							TreeNode T;
							T.key=(*Lite)[k].key;
							if(T.key!=-1)
							{
							for(int x=0;x<(*Lite)[k].Ancestor.size();x++)
								T.Ancestor.push_back((*Lite)[k].Ancestor[x]);
							T.Ancestor.push_back(n+1);
							}
							TItem.push_back(T);
							k++;
							i++;
							if((*Lite)[k].key==-1)
								sum+=-1;
							else
								sum+=1;
						}
					}
					if(TItem.size()!=0)
					{
						ProjectedDatabase.push_back(TItem);
						//flag=true;
						//break;
					}
					i--;
					
					//break;
					//-------------------------------
				}
			}
			//if(flag)
			//for(;i<(*Lite).size();)
			//{
			//	while(i<(*Lite).size()&&(*Lite)[i].key==-1)
			//		i++;
			//	if(i>=(*Lite).size())
			//		break;
			////	vector<TreeNode> TItem;
			////	TItem.clear();
			//	TreeNode T;
			//	int sum=0;
			//	do
			//	{
			//		T.key=(*Lite)[i].key;
			//		if(T.key!=-1)
			//		{
			//		for(int x=0;x<(*Lite)[i].Ancestor.size();x++)
			//			T.Ancestor.push_back((*Lite)[i].Ancestor[x]);
			//		}
			//		TItem.push_back(T);
			//		if((*Lite)[i].key==-1)
			//			sum+=-1;
			//		else
			//			sum+=1;
			//		i++;
			//	}while(i<(*Lite).size()&&sum!=0);

			//	
			//}
			//if(TItem.size()!=0)
			//{
			//		for(int x=0;x<TItem.size();x++)
			//		{
			//			if(TItem[x].key==(*I).key)
			//			{
			//				int k=x+1;
			//				x++;
			//				int sum=0;
			//				if(k<TItem.size())
			//				{
			//					if(TItem[k].key==-1)
			//						sum+=-1;
			//					else
			//						sum+=1;
			//					while(sum!=-1)
			//					{
			//						TItem[k].Ancestor.push_back(n+1);
			//						k++;
			//						x++;
			//						if(TItem[k].key==-1)
			//							sum+=-1;
			//						else
			//							sum+=1;
			//					}
			//				}
			//				x--;
			//			}
			//			/*if(TItem[x].key==-1)
			//				sum+=-1;
			//			else
			//				sum+=1;*/
			//		}
			//		/*if(sum==0)
			//			ProjectedDatabase.push_back(TItem);
			//		else
			//			printf("==========================================\n");*/
			//		ProjectedDatabase.push_back(TItem);
			//}
			
		}
		RecursiveMineFreqpattern(S,n+1,ProjectedDatabase,minsup);
	}
}
void MineFreqpattern(list<vector<TreeNode>> & Database,int minsup)
{
	vector<GE> b;     //frequent pattern
	FindFrequentGEs(Database,minsup,b);

	list<vector<TreeNode>> ProjectedDatabase;
	for(vector<GE>::iterator I=b.begin();I!=b.end();I++)
	{
		printf("%d %d\n",(*I).key,-1);
		TFP++;
		ProjectedDatabase.clear();

		//construct the projected database
		for(list<vector<TreeNode>> ::iterator Lite=Database.begin();Lite!=Database.end();Lite++)
		{
			for(int i=0;i<(*Lite).size();i++)
			{
				if((*Lite)[i].key==(*I).key)
				{
					vector<TreeNode> TItem;
					TItem.clear();
					int k=i+1;
					i++;
					int sum=0;
					if(k<(*Lite).size())
					{
						if((*Lite)[k].key==-1)
							sum+=-1;
						else
							sum+=1;
						while(sum!=-1)
						{
							TreeNode T;
							T.key=(*Lite)[k].key;
							for(int x=0;x<(*Lite)[k].Ancestor.size();x++)
								T.Ancestor.push_back((*Lite)[k].Ancestor[x]);
							TItem.push_back(T);
							k++;
							i++;
							if((*Lite)[k].key==-1)
								sum+=-1;
							else
								sum+=1;
						}
					}
					if(TItem.size()!=0)
						ProjectedDatabase.push_back(TItem);
					i--;
				}
			}
			
		}
		//call RecursiveMineFreqpattern
		vector<GE> S;
		S.push_back(*I);
		RecursiveMineFreqpattern(S,1,ProjectedDatabase,minsup);
		printf("\n\n");
		/*printf("\n\n");
		for(list<vector<TreeNode>> ::iterator Lite=ProjectedDatabase.begin();Lite!=ProjectedDatabase.end();Lite++)
		{
			int sum=0;
			for(int j=0;j<(*Lite).size();j++)
			{
				if((*Lite)[j].key==-1)
							sum+=-1;
						else
							sum+=1;
			}
			printf("%d\n",sum);
		}*/
	}

}
int main()
{
	//int count=0;
	TFP=0;   
	//freopen("D10.txt","r",stdin);  
	freopen("F5.txt","r",stdin);  
	//freopen("CSlog.txt","r",stdin);  //2328663
	//freopen("T1M.txt","r",stdin);  //2328663
    //freopen("out.txt","w",stdout);  
	
	int min_sup=3800;  

	list<vector<TreeNode>> Database;
	string str;
	while(getline(cin,str))
	{
		vector<string> substring=CutString(str," ");
		vector<TreeNode> Tree;
		Tree.clear();
		for(int i=0;i<substring.size();i++)
		{
			int temp=atoi( substring[i].c_str());
			//if(temp!=-1)
			//	count++;
			TreeNode TNtemp;
			TNtemp.key=temp;
			TNtemp.Ancestor.push_back(1);
			Tree.push_back(TNtemp);
		}
		Database.push_back(Tree);
	}
	//cout<<count<<endl;
	clock_t start = clock();
	MineFreqpattern(Database,min_sup);
	//cout<<Database.size()<<endl;

	//list<vector<TreeNode>> ::iterator Lite=Database.begin();
	//for(;Lite!=Database.end();Lite++)
	//{
	//	for(int i=0;i<(*Lite).size();i++)
	//		cout<<(*Lite)[i].key<<endl;
	//}
	clock_t ends = clock(); 
    
	printf("There are %d frequent embeded subtrees!\n\n",TFP);
	cout <<"Running Time : "<<(double)(ends - start)/ CLOCKS_PER_SEC << endl;
	return 0;
}