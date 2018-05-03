#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include<string>
#define dim  3//输入特征x的维数
#define K    2//分类的个数
#define MAX_NUM 100//读取输入数据的最大数目
using namespace std;

//定义类，封装输入向量x,及它要分到哪个类别
class InputFeature{
public:	
	double x[dim];//保存输入数据的向量坐标
	int cluster;//标志分类的哪一类(0,1,...,K)
	InputFeature(double x[dim])//构造函数
	{       memcpy(this->x,x,sizeof(double)*dim);
	cluster=-1;	}	//输入函数	
	void print()	{
		cout<<"cluster="<<cluster<<"   x=[";
		for (int i=0;i<dim;i++)
		{
			cout<<x[i]<<" ";
		}
		cout<<"]"<<endl;	
	}	//计算该特征向量与输入向量的2范数(欧式距离)
	double distanceOf(double u[dim])
	{        
		double distance=0;
		for (int i=0;i<dim;i++)
		{			
			distance+=(u[i]-x[i])*(u[i]-x[i]);
		}		
		distance=sqrt(distance);
		return distance;
}};
vector<InputFeature> InputVector;//保存输入特征向量
vector<int> split(string inputstring, string c) {
	vector<int> result;
	vector<string> temp_map;
	string::size_type pos1 = 0, pos2 = 0;
	string temp;
	while (pos2 < inputstring.size()) {
		pos2 = inputstring.find(c, pos2);
		if (pos2 == -1) {
			while (pos1 <inputstring.size()) {
				if (isalnum(inputstring[pos1]))
					temp += tolower(inputstring[pos1]);
				pos1++;
			}
			if (temp.size())
				temp_map.push_back(temp);
			break;
		}
		while (pos1 <pos2) {
			if (isalnum(inputstring[pos1]))
				temp += tolower(inputstring[pos1]);
			pos1++;
		}
		pos2 += c.size();
		if (temp.size())
			temp_map.push_back(temp);
		//result.push_back(temp);
		temp = "";
		pos1 = pos2;
	}
	for (vector<string>::size_type it = 0; it <temp_map.size(); it++) {
		result.push_back(atoi(temp_map[it].c_str()));
	}
	return result;
}
void inputData(string filepath)//从文件中读取数据
{	
	ifstream ifile(filepath);
	if(!ifile)      
	{ 
		cout<<filepath<<"cannot be opened!"<<endl;
		return;
	}    
//	char ch;      
	string s;
	//for (i=0;i<MAX_NUM;i++)//读取数目 
	while(!ifile.eof())
	{
		double xt[dim];
		getline(ifile, s);
		vector<int> temp_xt = split(s, ",");
		for (int i = 0; i < dim; i++) {
			xt[i] = temp_xt[i];
		}
		//将读入的字符串转化为小数	
		/*xt[0]=atof(s_X1.c_str());	
		xt[1]=atof(s_X2.c_str());		
		xt[2]=atof(s_X3.c_str());*/	
		InputFeature t1(xt);//生成特征向量	
		InputVector.push_back(t1);//保存到数组   
	}     
	ifile.close();
}//计算在误差err内，2次迭代的结果是否一样
bool CompareU(double U0[K][dim],double U[K][dim],double err){	
	for (int k=0;k<K;k++)	
	{		
		for (int d=0;d<dim;d++)		
		{		
			if(abs(U0[k][d]-U[k][d])>err)	
			{			
				return false;	
			}	
		} 
	}
	return true;
}//k-means算法核心
void k_means(double U[K][dim]){  
	double U0[K][dim];
	memcpy(U0,U,sizeof(double)*K*dim); 
	while (true)   {	
		//第一步 标定集合中的点，离哪个U点最近，即将其cluster修改为对应的分类	 
		int j;	 
		vector<InputFeature>::iterator it;
		for(it = InputVector.begin(); it != InputVector.end(); ++it)	
		{		   
			double dist[K];	
			for (j=0;j<K;j++)//计算该向量到各个标定向量的欧式距离	
			{			
				dist[j]=it->distanceOf(U[j]);	
			}		
			double minDist=dist[0];//初始化最小距离		
			it->cluster=0;//初始化分类	
			for (j=1;j<K;j++)		
			{			   
				if (dist[j]<minDist)//如果发现离第j个更近，则更新分类	
				{			
					minDist=dist[j];	
					it->cluster=j;			 
				}		
			}	
		}	   //第二步 更新重心U	 
		double sum[K][dim],num[K];	
		memset(&sum,0,sizeof(double)*K*dim);
		memset(&num,0,sizeof(double)*K);   
		for(it = InputVector.begin(); it != InputVector.end(); ++it)	
		{		
			for (int d=0;d<dim;d++)//计算所有相同分类的坐标重心	
			{			 
				sum[it->cluster][d]+=it->x[d];
			}		   
			num[it->cluster]++;//计算相同分类的数目	
		}	  
		for (j=0;j<K;j++)//更新标定向量的重心	
		{		 
			for (int d=0;d<dim;d++)		 
			{			  
				if (num[j]!=0)	
					U[j][d]=(sum[j][d])/(num[j]);	
			}	
		}	//判断是否收敛， U==U0时收敛	
		if (CompareU(U0,U,1.0e-5))
		{		  
			break;	
		}	  
		memcpy(U0,U,sizeof(double)*K*dim);//将本次迭代的结果保存 
	}   //输出聚类的结果  
	cout<<"k-means聚类的中心点坐标为："<<endl; 
	for (int k=0;k<K;k++)   {	
		cout<<"U"<<k<<"=";	  
		for (int d=0;d<dim;d++)	
		{		
			cout<<U[k][d]<<" ";	   
		}	   cout<<endl; 
	}   //此时每个元素的分类情况为：  
	cout<<"输入向量的聚类情况及坐标点："<<endl;
	vector<InputFeature>::iterator it; 
	for(it = InputVector.begin(); it != InputVector.end(); ++it)  
	{	
		it->print(); 
	}   //计算误差函数  
	double J=0;
	for(it = InputVector.begin(); it != InputVector.end(); ++it)  
	{	 
		J+=it->distanceOf(U[it->cluster]); 
	}  
	cout<<"误差函数J(c,u)="<<J<<endl;}
int main()
{	
	string filepath = "input.txt";
	inputData(filepath);//读入输入的数据	
				//初始化K=2个标定向量
	double U[K][dim]={{4.5,8.1,2.8},{-0.9,4.7,-0.3}};	
	k_means(U);//c进行k-means聚类	
	return 0;}
