#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include<string>
#define dim  3//��������x��ά��
#define K    2//����ĸ���
#define MAX_NUM 100//��ȡ�������ݵ������Ŀ
using namespace std;

//�����࣬��װ��������x,����Ҫ�ֵ��ĸ����
class InputFeature{
public:	
	double x[dim];//�����������ݵ���������
	int cluster;//��־�������һ��(0,1,...,K)
	InputFeature(double x[dim])//���캯��
	{       memcpy(this->x,x,sizeof(double)*dim);
	cluster=-1;	}	//���뺯��	
	void print()	{
		cout<<"cluster="<<cluster<<"   x=[";
		for (int i=0;i<dim;i++)
		{
			cout<<x[i]<<" ";
		}
		cout<<"]"<<endl;	
	}	//�������������������������2����(ŷʽ����)
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
vector<InputFeature> InputVector;//����������������
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
void inputData(string filepath)//���ļ��ж�ȡ����
{	
	ifstream ifile(filepath);
	if(!ifile)      
	{ 
		cout<<filepath<<"cannot be opened!"<<endl;
		return;
	}    
//	char ch;      
	string s;
	//for (i=0;i<MAX_NUM;i++)//��ȡ��Ŀ 
	while(!ifile.eof())
	{
		double xt[dim];
		getline(ifile, s);
		vector<int> temp_xt = split(s, ",");
		for (int i = 0; i < dim; i++) {
			xt[i] = temp_xt[i];
		}
		//��������ַ���ת��ΪС��	
		/*xt[0]=atof(s_X1.c_str());	
		xt[1]=atof(s_X2.c_str());		
		xt[2]=atof(s_X3.c_str());*/	
		InputFeature t1(xt);//������������	
		InputVector.push_back(t1);//���浽����   
	}     
	ifile.close();
}//���������err�ڣ�2�ε����Ľ���Ƿ�һ��
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
}//k-means�㷨����
void k_means(double U[K][dim]){  
	double U0[K][dim];
	memcpy(U0,U,sizeof(double)*K*dim); 
	while (true)   {	
		//��һ�� �궨�����еĵ㣬���ĸ�U�������������cluster�޸�Ϊ��Ӧ�ķ���	 
		int j;	 
		vector<InputFeature>::iterator it;
		for(it = InputVector.begin(); it != InputVector.end(); ++it)	
		{		   
			double dist[K];	
			for (j=0;j<K;j++)//����������������궨������ŷʽ����	
			{			
				dist[j]=it->distanceOf(U[j]);	
			}		
			double minDist=dist[0];//��ʼ����С����		
			it->cluster=0;//��ʼ������	
			for (j=1;j<K;j++)		
			{			   
				if (dist[j]<minDist)//����������j������������·���	
				{			
					minDist=dist[j];	
					it->cluster=j;			 
				}		
			}	
		}	   //�ڶ��� ��������U	 
		double sum[K][dim],num[K];	
		memset(&sum,0,sizeof(double)*K*dim);
		memset(&num,0,sizeof(double)*K);   
		for(it = InputVector.begin(); it != InputVector.end(); ++it)	
		{		
			for (int d=0;d<dim;d++)//����������ͬ�������������	
			{			 
				sum[it->cluster][d]+=it->x[d];
			}		   
			num[it->cluster]++;//������ͬ�������Ŀ	
		}	  
		for (j=0;j<K;j++)//���±궨����������	
		{		 
			for (int d=0;d<dim;d++)		 
			{			  
				if (num[j]!=0)	
					U[j][d]=(sum[j][d])/(num[j]);	
			}	
		}	//�ж��Ƿ������� U==U0ʱ����	
		if (CompareU(U0,U,1.0e-5))
		{		  
			break;	
		}	  
		memcpy(U0,U,sizeof(double)*K*dim);//�����ε����Ľ������ 
	}   //�������Ľ��  
	cout<<"k-means��������ĵ�����Ϊ��"<<endl; 
	for (int k=0;k<K;k++)   {	
		cout<<"U"<<k<<"=";	  
		for (int d=0;d<dim;d++)	
		{		
			cout<<U[k][d]<<" ";	   
		}	   cout<<endl; 
	}   //��ʱÿ��Ԫ�صķ������Ϊ��  
	cout<<"���������ľ������������㣺"<<endl;
	vector<InputFeature>::iterator it; 
	for(it = InputVector.begin(); it != InputVector.end(); ++it)  
	{	
		it->print(); 
	}   //��������  
	double J=0;
	for(it = InputVector.begin(); it != InputVector.end(); ++it)  
	{	 
		J+=it->distanceOf(U[it->cluster]); 
	}  
	cout<<"����J(c,u)="<<J<<endl;}
int main()
{	
	string filepath = "input.txt";
	inputData(filepath);//�������������	
				//��ʼ��K=2���궨����
	double U[K][dim]={{4.5,8.1,2.8},{-0.9,4.7,-0.3}};	
	k_means(U);//c����k-means����	
	return 0;}
