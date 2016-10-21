#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include "schedule.h"
#include <fstream>
#include <sstream>
#include <string>

#define STEP 10
using namespace std;

vector<string> split(const string &s, char delim) {
	stringstream ss(s);
	string item;
	vector<string> tokens;
	while (getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}

schedule* minOfVector(vector<schedule*> schedules){
	int min = (int)MAX_TIME  + 1;
	int minIndex = 0;
	for (int i = 0; i < schedules.size(); i++){
		if(schedules.at(i)->timeFinish < min){
			min = schedules.at(i)->timeFinish;
			minIndex = i;
		}
	}

	return schedules.at(minIndex);
}

int main(){
	srand(time(NULL)); //for actually random with function rand() or random_shuffer()
	vector<machine *>* machines = new vector<machine *>();
	vector<task *>* tasks = new vector<task *>();

	/* Generate machines and tasks from file*/
	string line;

	ifstream inputMachine ("input/machine.txt");
	int id = 0;
	if (inputMachine.is_open()){
		while (getline (inputMachine,line) ){
			id ++ ;
			vector<string> x = split(line,',');
			if(x.size() == 2)
				machines->push_back(new machine(id, resource(stoi(x.at(0)), stoi(x.at(1)))));
			x.clear();
											
	       	}
		inputMachine.close();
	}
	else cout << "Unable to open file"; 

	ifstream inputTask ("input/task.txt");
	id = 0;
	if (inputTask.is_open()){
		while (getline (inputTask,line) ){
			id ++ ;
			vector<string> x = split(line,',');
			if(x.size() == 3)
				tasks->push_back(new task(id, resource(stoi(x.at(0)), stoi(x.at(1))), stoi(x.at(2))));	
			x.clear();
											
	       	}
		inputTask.close();
	}
	else cout << "Unable to open file"; 


	/*Init scheduleObj with generated machines and tasks above*/
	vector<schedule *> schedules;
	for (int i = 0 ; i < STEP ; i++){
		schedule * scheduleObj = new schedule(machines, tasks);
		scheduleObj->scheduling();
		schedules.push_back(scheduleObj);	
	}

	schedule *minSchedule = minOfVector(schedules);
	cout <<"The best result time:  "<< minSchedule->timeFinish << endl;
	cout <<"Now, visualize the progress by drawing.........."<<endl;

	/* Draw the best result*/
	minSchedule->draw();
}
