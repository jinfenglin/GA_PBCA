#pragma once
#include <list>
#include <iostream>
#include <fstream>
#include <ctime>
#include<cstdlib>
#include<algorithm>
#include <malloc.h>
#include <stdlib.h>

using namespace std;
#define MaxSize 1000 
#define MaxLength 100
#define PATH "..\\input file.txt"
struct TSP_input
{
	int L;            //L is the length of sequence,the number of cities
	int start_point;  //where the merchant start
	int round;         //rounds need to run	
	int times;        //How many times this parameter will be used

	int pz;           //pz is the size of safe zone
	int pmax;         //pmax is the max population
};
struct sequence      //struct of the phage/bacteria
{
	int seq[MaxSize]; 
	int length;
	int fitness;
};
struct map
{
	int matrix[MaxLength][MaxLength];
	int length;
	int number;
};
class PBCA
{
public:
	PBCA(void);
	~PBCA(void);
	int valid_time;//allow others to use valid_time from tsp_in
	bool input(ifstream &ifs );//Get parameter from input to fullfill tsp_in, have 2 ways to read 1.consoler 2.text file read. This is the text version
	bool input();         //consoler version reading
	bool random_produce(int amount,list<sequence> &list); //randomly produce genes
	bool random_produce();           //if amount is miss will produce the same number of pz
	void Run();

	void Mutate_Selection();        //let the phage evolve
	int fitness(sequence&);          //calculate the fitness
	void Read_Map();                //only 1 map pertime
	void Select_Map();              //select map for the parameter
	void mutate(sequence);          //mutate on a single sequence
	void All_fitness(list<sequence>&);
	void replace(sequence b,sequence p);
	void select(int number);       //amount want to delete
	void fight();                  //competition between phage and bacteria
	void Test_Run();
	void population_regulation();
	/*Test tools*/
	void Test_showparameters();//output parameter
	void Test_showSequenceArray();//output bacteria and phage array
	void Test_showMapVector();//output map vector
	void Output();
	bool Is_SequenceInList(sequence,list<sequence>);
	friend bool operator ==(sequence s1,sequence s2);
private:
	TSP_input tsp_in;
	map current_map;
	list<map> map_vector;
	list<sequence> bacteria,phage;
	int* auto_select();
	int largest_amount();
	float Survive_Rate(sequence,sequence);
	bool is_InTheGene(int numb,sequence S,int length);//Check out whether number int is in the sequence 
	
	
};                           
                            

