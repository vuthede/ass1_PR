#include <iostream>
#include "schedule.h"
using namespace std;


int myrandom (int i) { return std::rand()%i;}
resource::resource(){
	this->CPU = 0;
	this->RAM = 0;
}

resource::resource(double CPU, double RAM):CPU(CPU),RAM(RAM){}


task::task(int id, resource Resource, int time):id(id), needResource(Resource), time(time){}

int task::getID(){
	return this->id;
}

resource task::get_needResource(){
	return this->needResource;
}

int task::get_time(){
	return this->time;
}

int task::set_etime(int etime){
	this->etime = etime;
}

int task::get_etime(){
	return this->etime;
}

void task::refreshTask(){
	this->etime = 0;
}


machine::machine(int id, resource Resource){
	this->id = id;
	this->fullResource = Resource;
	this->remainResource = Resource;
	allTasks = new vector<task *>();
	processingTasks = new vector<task *>();
	waitingTasks = new vector<task *>();	
}

int machine::getID(){
	return this->id;
}

resource machine::get_fullResource(){
	return this->fullResource;
}

bool machine::checkEnoughResource(task *Task){
	if (Task->get_needResource().CPU > this->remainResource.CPU)
		return false;
	if (Task->get_needResource().RAM > this->remainResource.RAM)
		return false;

	return true;
}

void machine::insertTask(task *Task){
	this->allTasks->push_back(Task);
}


int machine::getTimeProcess(){
	/* Insert tasks into waitingTasks queue */
	for(int i = 0 ;i < allTasks->size(); i++){
		waitingTasks->push_back(allTasks->at(i));
	}

	int currentTime = 0;
	while(waitingTasks->size() > 0){
		while(waitingTasks->size() > 0 && checkEnoughResource(waitingTasks->at(0))){
			
			/* Transmit task from wait queue to process queue*/
			processingTasks->push_back(waitingTasks->at(0));
			remainResource.CPU -= waitingTasks->at(0)->get_needResource().CPU;
		       	remainResource.RAM -= waitingTasks->at(0)->get_needResource().RAM;

			/* Set end time for transmitted task */
			waitingTasks->at(0)->set_etime(waitingTasks->at(0)->get_time() + currentTime);
			
			/* Delete this task from wait queue*/
			waitingTasks->erase(waitingTasks->begin());
		}
			
		/* sort tasks ini processingTasks in ascending order of end time*/
		sort(processingTasks->begin(),processingTasks->end(), []( task *x, task *y){ return (x->get_etime() < y->get_etime());});

		/* Current time is when the first task in processingtasks queue has just been finished*/
		currentTime = processingTasks->at(0)->get_etime();

		/*Free resource for machine when this task has been finished*/
		freeResource(processingTasks->at(0));
		
		/* Remove the finished task into processingTasks queue*/
		processingTasks->erase(processingTasks->begin());
	
	}
	
	/* Get the time that machine finish all tasks*/
	int time = (processingTasks->size() == 0) ? currentTime : processingTasks->at(processingTasks->size() - 1)->get_etime();
	
	/* Free process and wait queue*/
	processingTasks->clear();
	waitingTasks->clear();
	remainResource = get_fullResource();

	return time;
}



void machine::freeResource(task *Task){
	this->remainResource.CPU += Task->get_needResource().CPU;
	this->remainResource.RAM += Task->get_needResource().RAM;
}

void machine::refreshMachine(){
	this->remainResource = fullResource;
	this->allTasks->clear();
	this->processingTasks->clear();
	this->waitingTasks->clear();
}

/*Implement funtions for class schedule
 *Rememner to add srand into parallel algorithm
 * */

schedule::schedule(vector<machine *> * machines, vector<task *> * tasks){
	this->machines = new vector<machine *>();
	this->tasks = new vector<task *>();

	for (int i = 0 ; i < machines->size(); i++)
		this->machines->push_back(new machine(i, machines->at(i)->get_fullResource()));

	for (int i = 0 ; i < tasks->size(); i ++)
		this->tasks->push_back(new task(i, tasks->at(i)->get_needResource(), tasks->at(i)->get_time()));

	random_shuffle(this->machines->begin(), this->machines->end(), myrandom);	       
	random_shuffle(this->tasks->begin(), this->tasks->end(), myrandom);

}


machine* schedule::chooseMachine(task *Task){
	int minDeltaTime = 1000000;
	int indexChosenMachine = 0;
	for (int i = 0; i < this->machines->size(); i++ ){
		this->machines->at(i)->allTasks->push_back(Task);
		int deltaTime = this->machines->at(i)->getTimeProcess() - this->timeFinish;
		this->machines->at(i)->allTasks->pop_back();
		if (deltaTime < minDeltaTime){
			minDeltaTime = deltaTime;
			indexChosenMachine = i;
		}
	}
	return this->machines->at(indexChosenMachine);
}


void schedule::updateTimeFinish(){
	int time = -1;
	for (int i = 0 ; i < machines->size(); i++)
		time = (machines->at(i)->getTimeProcess() > time) ? machines->at(i)->getTimeProcess() : time ;

	this->timeFinish = time;
}


void schedule::scheduling(){
	for (int i = 0 ; i < this->tasks->size(); i++){
		chooseMachine(this->tasks->at(i))->insertTask(this->tasks->at(i));
		updateTimeFinish();
	}
}

void schedule::draw(){
	//TODO
	

}











