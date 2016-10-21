#ifndef _SCHEDULE_
#define _SCHEDULE_
#include <vector>
#include <algorithm>
#define MAX_TIME 9999


class resource{
public:
	double CPU;
	double RAM;
	resource();
	resource(double CPU, double RAM);
};

class task{
private:
	int id;
	resource needResource;
	int time;
	int etime;
public:
	task(int id, resource Resource, int time);
	int getID();
	resource get_needResource();
	int get_time();
	int set_etime(int etime);
	int get_etime();
	void refreshTask();
};


class machine{
private:
	int id;
	resource fullResource;
public:
	resource remainResource;

	std::vector<task*>* allTasks ;
	std::vector<task*>* processingTasks;
	std::vector<task*>* waitingTasks;

	/* Method*/
	machine(int id, resource Resource);
	int getID();
	resource get_fullResource();
	bool checkEnoughResource(task *Task);
	void insertTask(task *Task);
	int getTimeProcess();
	void freeResource(task *Task);
	void refreshMachine();
	
};

class schedule{
public:
	int timeFinish = 0;
	std::vector<machine *> * machines;
	std::vector<task *> * tasks;

	schedule(std::vector<machine *> * machines, std::vector<task *> * tasks);

	/*Choose approriate machine for a task */
	machine* chooseMachine(task *Task);

	void updateTimeFinish();	
	void scheduling();

	void draw();
	

};




#endif
