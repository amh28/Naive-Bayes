#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <utility> 
#include <sstream>

using namespace std;

typedef string S;
typedef map<S,map<S,int> > Map2;
typedef map<S,Map2> Map1;
typedef vector<S> VS;
typedef stringstream SS;
typedef vector<int> VI;


class Classifier
{
	size_t n_elems;
	size_t n_attributes;
	size_t n_classes;

	
	VS classes;
	VS attributes;	
	
	Map1 frec;	
	map<S,int> tam_class;
	map<S,map<S,int> > Mp_x;
	
		
	public:
	
	Classifier():n_elems(0),n_attributes(6), n_classes(4)
	{
		VS tmp(n_classes); tmp[0]="unacc"; tmp[1]="acc"; tmp[2]="good"; tmp[3]="vgood";
		classes=tmp;
		
		VS tmp1(n_attributes); tmp1[0]="buying"; tmp1[1]="maint"; tmp1[2]="doors"; tmp1[3]="persons"; tmp1[4]="lug_bot"; tmp1[5]="safety"; 
		attributes=tmp1;
					
	}
	void load_data();
	void classify(VS &search);
};

void Classifier::load_data()
{
	S file="car.data";
	ifstream iff(file);
	
	while(!iff.eof())
	{
		
		n_elems++; 
		
		S buying, maint,doors, persons, lug_bot, safety, class_v;	
		
		getline(iff,buying,',');						
		getline(iff,maint,',');
		getline(iff,doors,',');
		getline(iff,persons,',');
		getline(iff,lug_bot,',');
		getline(iff,safety,',');
		getline(iff,class_v,'\n');
		
		
		frec["buying"][buying][class_v]++;
		frec["maint"][maint][class_v]++;
		frec["doors"][doors][class_v]++;
		frec["persons"][persons][class_v]++;
		frec["lug_bot"][lug_bot][class_v]++;
		frec["safety"][safety][class_v]++;	
		
		tam_class[class_v]++; 			 
		
		Mp_x["buying"][buying]++;
		Mp_x["maint"][maint]++;
		Mp_x["doors"][doors]++;
		Mp_x["persons"][persons]++;
		Mp_x["lug_bot"][lug_bot]++;
		Mp_x["safety"][safety]++;	
				
	}	
	
	cout<<tam_class["unacc"]<<endl;
	cout<<tam_class["acc"]<<endl;
	cout<<tam_class["good"]<<endl;
	cout<<tam_class["vgood"]<<endl;
	
	//This happens because of getline function
	
	n_elems-=1;	
}

void Classifier::classify(VS &search)
{
	int size=search.size();
	float MAX=-1.0;
	S final_class;
	
	float p_x=1.0;
	for(int j=0;j<n_attributes;j++)
		p_x*=((Mp_x[attributes[j]][search[j]]/(n_elems*1.0))*1.0);
	
	for(int i=0;i<n_classes;i++)
	{
		float mult=1.0;
		float p_x=1.0;
		cout<<"Trabajando en clase "<<classes[i]<<endl;
		for(int j=0; j<n_attributes;j++)
		{
			cout<<"Atributo: "<<attributes[j]<<" subAttrib: "<<search[j];
							
			float temp = (frec[attributes[j]][search[j]][classes[i]])*1.0;			
			cout<<" frecuencia: "<<temp;
			temp/=(tam_class[classes[i]]*1.0);
			
			mult*=(temp*1.0);
			
			cout<<" p_x: "<<Mp_x[attributes[j]][search[j]]<<endl;
			
		}		
		
		
		
		float p_c= tam_class[classes[i]]/(n_elems*1.0);									
		mult*=(p_c*1.0); 
				
		//cout<<"p_x: "<<p_x<<endl;				
		//if(mult!=0)  mult/=p_x;
		cout<<"Probabilidad de clase "<<classes[i]<<": "<<mult<<endl<<endl;
		
		if(mult>MAX) {	MAX=mult; final_class=classes[i]; }
	}
	if(MAX!=0)
		cout<<"CLASE FINAL: "<<final_class<<" Probabilidad Maxima: "<<MAX<<endl;
	else
		cout<<"Probabilidad maxima 0"<<endl;
}

int main()
{
	Classifier c;
	c.load_data();
	
	ifstream iff("car-prueba.data");
	
	while(!iff.eof())
	{					
		S buying, maint,doors, persons, lug_bot, safety;	
		
		getline(iff,buying,',');						
		getline(iff,maint,',');
		getline(iff,doors,',');
		getline(iff,persons,',');
		getline(iff,lug_bot,',');
		getline(iff,safety,'\n');		
	
	VS vec; vec.push_back(buying);	vec.push_back(maint);	vec.push_back(doors); 
		    vec.push_back(persons); vec.push_back(lug_bot); vec.push_back(safety);
		
		cout<<"---------------------------------------------------------------------------------------"<<endl;
		cout<<"TEST: "<<buying<<" "<<maint<<" "<<doors<<" "<<persons<<" "<<lug_bot<<" "<<safety<<endl; 
		c.classify(vec);
			
	}
}



