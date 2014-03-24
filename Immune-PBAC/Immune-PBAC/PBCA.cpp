#include "StdAfx.h"
#include "PBCA.h"
#define intact -1 
PBCA::PBCA(void)
{
	//initialize the parameter
	tsp_in.L=intact;
	tsp_in.pmax=intact;
	tsp_in.pz=intact;
	tsp_in.round=intact;
	tsp_in.start_point=intact;
	tsp_in.times=intact;
	current_map.length=0;
	srand (time(NULL)); // This will ensure a really randomized number by help of valid_time.
}
PBCA::~PBCA(void)
{
}
bool PBCA::input() //each round get 5 parameter always true
{
	cout<<"Please input parameters in order:"<<endl;
	cout<<"1.Number of cities 2.Start point 3.Rounds to run"<<endl;
	cout<<"4.Max population 5.Size of the safe zone"<<endl;
	while(1)
	{
		cin>>tsp_in.L>>tsp_in.start_point>>tsp_in.round>>tsp_in.pmax>>tsp_in.pz;
		if(tsp_in.L<=intact||tsp_in.start_point<=intact||tsp_in.round<=intact||tsp_in.pmax<=intact||tsp_in.pz<=intact)//0 are not included here...but it should be...lazy
		cout<<"Error detected,please input again"<<endl;
		else
			{
				tsp_in.times=1;
				cin.ignore(INT_MAX,'\n');                      //everyround have 5 paremeter if not clean may cause bug
				cout<<"Input completed"<<endl;
				return true;
		}
	}
}
bool PBCA::input(ifstream &ifs) //each valid_time get 1 line and kept it for tsp.times rounds when file is over return false
{
	cout<<"Begin to read file"<<endl;
	if(!ifs.eof())
	{
		ifs>>tsp_in.L>>tsp_in.start_point>>tsp_in.round>>tsp_in.pmax>>tsp_in.pz>>tsp_in.times;
		cout<<"reading finished"<<endl;
		valid_time=tsp_in.times;
		return true;
	}
	else
		return false;
}
void PBCA::Test_showparameters()
{
	cout<<"tsp_in.L="<<tsp_in.L<<" "<<"tsp_in.start_point="<<tsp_in.start_point<<" "<<"tsp_in.round="<<tsp_in.round<<" ";
	cout<<"tsp_in.pmax="<<tsp_in.pmax<<" "<<"tsp_in.pz="<<tsp_in.pz<<" "<<"tsp_in.times="<<tsp_in.times<<" "<<endl;
}
int PBCA::largest_amount()
{
	int amount=1;
	for(int i=1;i<=tsp_in.L;i++)
			amount=amount*i;
	return amount;
}
bool PBCA::random_produce(int amount,list<sequence> &list)
{
	int dice_roll;
	/*unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	uniform_int_distribution<int> distribution(1,tsp_in.L); //produce random number in the interval [1,tsp_in.L]*/	
	if(amount>largest_amount())
	{
		amount=largest_amount();
		cout<<"amount overflow,after adjusting produce "<<amount<<"individuals"<<endl;
	}

	for(int i=0;i<amount;i++)//amount stand for the quantity of sequence
	{
		sequence S;          /*initialize the sequence*/
		S.fitness=intact;
		S.length=tsp_in.L;
		for(int n=0;n<tsp_in.L;n++)
		{
			while(1)// To generate leagal genom to put into sequence
			{
				dice_roll=rand()%tsp_in.L+1; // Randomizing the number between 1-tsp_in.
				if(!is_InTheGene(dice_roll,S,n))
					break;
			}
			S.seq[n]=dice_roll;
		}
		if(find(list.begin(),list.end(),S)==list.end())
			list.push_front(S);
		else
			i--;

	}
	return true; //tp
}
bool PBCA::is_InTheGene(int numb,sequence S,int length) //numb in the S return true 
{
	for(int i=0;i<length;i++)
	{
		if(numb==S.seq[i])
			return true;
	}
	return false;
}
void PBCA::Test_showSequenceArray()
{
	list<sequence>::iterator it;
	if(!bacteria.empty())
	{
		cout<<"bacteria sequence:"<<endl;
		for(it=bacteria.begin();it!=bacteria.end();it++)
			{
				for(int i=0;i<tsp_in.L;i++)
					cout<<it->seq[i];
				cout<<"   fitness:"<<it->fitness<<endl;
		}
	}
	else
		cout<<"Bacteria list is empty!"<<endl;

	if(!phage.empty())
	{
		cout<<"phage sequence:"<<endl;
		for(it=phage.begin();it!=phage.end();it++)
			{
				for(int i=0;i<tsp_in.L;i++)
					cout<<it->seq[i];
				cout<<"   fitness:"<<it->fitness<<endl;
		}
	}
	else
		cout<<"phage list is empty!"<<endl;
}
void PBCA::mutate(sequence S)  //add a new array into the phage array
{
	int r1=rand()%S.length;
	int r2=rand()%S.length;
	sequence copy_s=S;
	int temp=copy_s.seq[r2];
	copy_s.seq[r2]=copy_s.seq[r1];
	copy_s.seq[r1]=temp;//exchange

	
	if(find(phage.begin(),phage.end(),copy_s)==phage.end())
	phage.push_back(copy_s);
}
bool operator ==(sequence s1,sequence s2)
{
	if(s1.length!=s2.length)
		return false;
	else
	{
		for(int i=0;i<s1.length;i++)
		{
			if(s1.seq[i]!=s2.seq[i])
				return false;
		}
		return true;
	}
}
void PBCA::All_fitness(list<sequence> &lis)
{
	list<sequence>::iterator it;
	for( it=lis.begin();it!=lis.end();it++)
	{
		fitness(*it);
	}
}
bool CompareRule(sequence s1,sequence s2)
{
	if(s1.fitness>s2.fitness)
		return true;
	else
		return false;
}
void PBCA::select(int number) //chop the last numbers items form phage array
{
	for(int i=0;i<number;i++)
	{
		if(phage.size()<tsp_in.pz) //chop the worst part above safe zone
			break;
		else
		{
			phage.pop_back();
		}
	}	
}
void PBCA::Mutate_Selection()
{
	float mp;
	int population=phage.size();
	float dicer; 
	list<sequence>::iterator it;

	mp=1.0-(float)phage.size()/(phage.size()+bacteria.size());
	for(it=phage.begin();it!=phage.end();it++)
	{
		dicer=(float)rand()/(float)RAND_MAX;//product random number between 0-1
		if(dicer<mp)                        //another way to mutate
			mutate(*it);
	}
	All_fitness(phage);
	phage.sort(CompareRule);
	//select(phage.size()-population);  //mutation will not expand the population
	Test_showSequenceArray();

	
}
int PBCA::fitness(sequence &S)
{ 
	int fitness=0;
	int now=0,next=0;
	int now_location,next_location;
	for(int i=0;i<S.length;i++)
	{

		now_location=i;
		next_location=(now+1)%S.length;			
		now=S.seq[now_location]-1;
		next=S.seq[next_location]-1;                 //sequence start form 1 matrix start from 0 thus -1
		fitness+=current_map.matrix[now][next];
	}
	S.fitness=fitness;
	return fitness;
}
int* PBCA::auto_select()
{
	int ptr[MaxLength];
	int count=0;
	//ptr=(int *)malloc(sizeof(int));
	//Test_showMapVector();
	for(list<map>::iterator it=map_vector.begin();it!=map_vector.end();it++)
	{
		if(it->length==tsp_in.L)
		{
			ptr[count]=it->number;
			count++;
		}
	}	
	ptr[count]=-1;
	return ptr;
}
void PBCA::Select_Map()
{
	int record;
	if(current_map.length==0)
		cout<<"Have no map selected!"<<endl;
	else
		{
			cout<<"Current map is:No."<<current_map.number<<endl;
			cout<<"Show current map?Y/N"<<endl;
			
			char temp=' ';
			while(temp!='y'&&temp!='Y'&&temp!='n'&&temp!='N')
				temp=getchar();
			if(temp=='y'||temp=='Y')
			{
				for(int i=0;i<current_map.length;i++)
				{
					for(int j=0;j<current_map.length;j++)
					{
						cout<<current_map.matrix[i][j]<<" ";
					}
					cout<<endl;
				}
			}
			cout<<endl;
	}//show ccurrent map-end of if(have a current map)

			cout<<"Available maps:"<<endl; 
			int *pointer=auto_select();  //pick out the map whose length match with parameter
			int temp_array[MaxLength];
			for(int i=0;pointer[i]>=0;i++)  //copy picked map(their number) to the temp_array
			{
				temp_array[i]=pointer[i];
			}
			for(int i=0;temp_array[i]>=0;i++)   //out put the choice we have
			{                
				cout<<"No."<<temp_array[i]<<" ";//Abnormally change->pointer maybe caused by the attrubute of pointer.So copy the number to array,should use cpy
				record=i;                      //record the available map array length        
			}
			cout<<endl;
			/*end of building temp_array*/

		    cout<<"Auto select?Y/N?"<<endl; 
			char temp_char=' ';
			while(temp_char!='y'&&temp_char!='Y'&&temp_char!='n'&&temp_char!='N')
				temp_char=getchar();
			int temp_location;
			if(temp_char=='y'||temp_char=='Y')  //randomly pick one
			{
				                       
				cout<<"record="<<record<<endl;
				temp_location=rand()%(record+1);
				/*copy pick matrix to the current one*/
				current_map.number=temp_array[temp_location];
				for(list<map>::iterator it=map_vector.begin();it!=map_vector.end();it++) //Find No.temp MAP in the array and copy it to current map
				{
					if(it->number==temp_array[temp_location])
					{
						current_map.length=it->length;
						//current_map.matrix=it->matrix;  Be careful here
						for(int i=0;i<current_map.length;i++)
							for(int n=0;n<current_map.length;n++)
							{
								current_map.matrix[i][n]=it->matrix[i][n];
							}
					}
				}//copy-over
			}
			else //pick one by hand
			{
				while(1) //check out the input is legle or not
				{
					cout<<"Input the number of map:"<<endl;
					int temp_num=0;
					cin>>temp_num;
					for(list<map>::iterator it=map_vector.begin();it!=map_vector.end();it++)
					{
						if(it->number==temp_num&&it->length==tsp_in.L)
						{
							current_map.length=it->length;
							for(int i=0;i<current_map.length;i++)
								for(int n=0;n<current_map.length;n++)
								{
									current_map.matrix[i][n]=it->matrix[i][n];
								}
								current_map.number=it->number;
								goto out;
						}
						else
							cout<<"Invalid map:1.Dismatch in length/ 2.Number is invalid."<<endl;
					}
				}
			}
			out:cout<<"current map is No."<<current_map.number<<endl;

}
void PBCA::Read_Map()
{
	int number=0;
	ifstream ifs;
	ifs.open("..\\map.txt");
	
	if(!ifs)
		cout<<"Fail to open map file"<<endl;
	else
		cout<<"Success to open map file"<<endl;
	while(!ifs.eof())                   //each time read a block of date
	{  
		map M;
		M.number=number;
		number++;
		ifs>>M.length;
		for(int i=0;i<M.length;i++)
			for(int j=0;j<M.length;j++)
			{
				ifs>>M.matrix[i][j];
			}
			map_vector.push_back(M);
	}
	map_vector.pop_back();//Because read block, the last block will be counted for twice 
}
void PBCA::Test_showMapVector()
{
	list<map>::iterator it;
	for(it=map_vector.begin();it!=map_vector.end();it++)
		{
			for(int i=0;i<it->length;i++)
			{
				for(int j=0;j<it->length;j++)
				{
					cout<<it->matrix[i][j]<<" ";
				}
				cout<<endl;
			}
			cout<<endl;
	}
}
float PBCA::Survive_Rate(sequence p,sequence b)
{
	float S,prate;
	int maxf,de;

	if(p.fitness>b.fitness)
	{
		maxf=p.fitness;
	}
	else
	{
		maxf=b.fitness;
		
	}
	de=b.fitness-p.fitness;
	prate=phage.size()/(phage.size()+bacteria.size());
	S=0.5+de/maxf+(1.0-prate)/4;
	return S;

}
void PBCA::fight()
{
	All_fitness(bacteria);
	//bacteria.sort(CompareRule);
	list<sequence>::iterator it_b,it_p;
	for(it_p=phage.begin();it_p!=phage.end();it_p++)
	{
		for(it_b=bacteria.begin();it_b!=bacteria.end();it_b++) //phage challange every bacteria
		{
			if(it_p->fitness>it_b->fitness)
			{
				float dicer=(float)rand()/(float)RAND_MAX;
				float S=Survive_Rate(*it_p,*it_b);
				if(dicer<S)//bacteria win
				{
					replace(*it_b,*it_p);
				}
				else//phage win
				{
					mutate(*it_p);
				}
			}
		}

	}



}
void PBCA::replace(sequence b,sequence p)  //more item could be added here
{
	int replace_length=tsp_in.L/4+1;
	list<int> temp_list_b,temp_list_p;
	for(int i=0;i<replace_length;i++)
	{
		temp_list_b.push_front(b.seq[i]);//preserve the numbers will be insert back to the bacteria sequence
		temp_list_p.push_front(p.seq[i]);//preserve the numbers transmitted into bacteria
	}
	for(int i=0;i<replace_length;i++)
	{
		b.seq[i]=p.seq[i];//copy the segment
		list<int>::iterator it=find(temp_list_b.begin(),temp_list_b.end(),p.seq[i]);
		if(it!=temp_list_b.end())
			temp_list_b.erase(it);//delete the overlap ones
	}
	/*reorgnize the sequence*/
	for(int i=replace_length;i<tsp_in.L;i++)
	{
		/*if(b.seq[i]in the temp_list_p )
			replace this and pop list_b*/
		if(find(temp_list_p.begin(),temp_list_p.end(),b.seq[i])!=temp_list_p.end())
		{
			b.seq[i]=temp_list_b.front();
			temp_list_b.pop_front();
		}
	}
	if(!Is_SequenceInList(b,bacteria))
		bacteria.push_back(b);

}
bool PBCA::Is_SequenceInList(sequence S,list<sequence> L)
{
	if(find(L.begin(),L.end(),S)!=L.end())
		return true;
	else
		return false;

}
void PBCA::population_regulation()
{
	bacteria.sort(CompareRule);
	phage.sort(CompareRule);
	int population_now=bacteria.size()+phage.size();
	while(population_now>tsp_in.pmax)
	{
		if(bacteria.size()>tsp_in.pz&&phage.size()>tsp_in.pz)
		{
			if(bacteria.back().fitness>phage.back().fitness)
				phage.pop_back();
			else
				bacteria.pop_back();
		}
		else if(bacteria.size()>tsp_in.pz&&phage.size()<=tsp_in.pz)
			bacteria.pop_back();
		else if(bacteria.size()<=tsp_in.pz&&phage.size()>tsp_in.pz)
			phage.pop_back();
	}
	
}
void PBCA::Test_Run()
{	tsp_in.L=6;
	tsp_in.pmax=100;
	tsp_in.pz=10;
	tsp_in.round=10;
	tsp_in.start_point=1;
	tsp_in.times=1;

	sequence s1;
	s1.length=6;
	s1.seq[0]=1;
	s1.seq[1]=2;
	s1.seq[2]=3;
	s1.seq[3]=4;
	s1.seq[4]=5;
	s1.seq[5]=6;

	sequence s2;
	s2.length=6;	
	s2.seq[0]=1;
	s2.seq[1]=5;
	s2.seq[2]=3;
	s2.seq[3]=4;
	s2.seq[4]=2;
	s2.seq[5]=6;
	replace(s1,s2);
}
void PBCA::Output()
{
	bacteria.sort(CompareRule);
	phage.sort(CompareRule);
	if(bacteria.front().fitness>phage.front().fitness)
	{
		cout<<"The best solution is:"<<endl;
		for(int i=0;i<tsp_in.L;i++)
			cout<<bacteria.front().seq[i];	
		cout<<endl;
		cout<<"Fitness="<<bacteria.front().fitness<<endl;
	}
	else
	{
		cout<<"The best solution is:"<<endl;
		for(int i=0;i<tsp_in.L;i++)
			cout<<phage.front().seq[i];	
		cout<<endl;
		cout<<"Fitness="<<phage.front().fitness<<endl;
	}

}
void PBCA::Run()
{


	/*reading*/

	int count=1;
	ifstream ifs;
	ifs.open(PATH);
	Read_Map();
	if(!ifs)
		cout<<"Fail to open input file"<<endl;
	else 
		cout<<"sucess to open input file"<<endl;

	for(int i=0;;i++)
	{
		if(valid_time<1)                            //if the last input parameter is becoming invalid
			if(!input(ifs)&&valid_time==0)          //read in this step.only when ifs reach the end of file and the last set of parameter become invalid then jump out
					break;
	/*MainBody*/
		Select_Map();
		random_produce(tsp_in.pz/2,bacteria);
		random_produce(tsp_in.pz/2,phage);
		for(int j=0;j<tsp_in.round;j++)
		{
			/*Test_showparameters();
			;*/
			Mutate_Selection();
			fight();
			population_regulation();
			Test_showSequenceArray();
		}
		Output();
	/*Rest area*/

		valid_time--;
		bacteria.clear();
		phage.clear();
		cout<<i<<endl;
	} 

	

	getchar();

}