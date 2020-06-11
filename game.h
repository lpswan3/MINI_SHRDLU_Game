/*
 * game.h
 *
 *  Created on: 17 Apr 2020
 *      Author: johnathanliao
 */

#ifndef GAME_H_
#define GAME_H_
using namespace std;

class game{
public:
	void start();

private:
	State state;
};

void game::start(){
	state.initialize();
	state.printBoard();
	goal g;
	cout<<"Please enter the goal value(1-6):"<<endl;
	cin>>g.val;
	cout<<"Please enter the goal row(0-2)"<<endl;
	cin>>g.row;
	cout<<"Please enter the goal col(0-2)"<<endl;
	cin>>g.col;

	singleGoalSolver s(state,3,g);
	State final=s.calculate();
	queue<action>moveList =final.getRec();
	int step=moveList.size();
	cout<<"******search outcome******"<<endl;
	state.printBoard();
	while(!moveList.empty()){
		action a=moveList.front();
		state.moveBlock(a);
		state.printBoard();
		moveList.pop();
	}
	cout<<"complete in "<<step<<" steps.";
}




#endif /* GAME_H_ */
