/*
 * Solver.h
 *
 *  Created on: 12 Apr 2020
 *      Author: johnathanliao
 */

#ifndef SOLVER_H_
#define SOLVER_H_
using namespace std;


struct calAction:public action{
	State state;
	int heuristicVal;
	int fVal;
	bool operator<(const calAction c)const{
		if(this->fVal > c.fVal){
			return true;
		}
		return false;
	}
	bool operator==(calAction a){
		if(this->src==a.src&&this->des==a.des&&
				this->heuristicVal==a.heuristicVal&&this->fVal==a.fVal){
			return true;
		}
		return false;
	}
};

class singleGoalSolver{
public:
	singleGoalSolver(State s,int size,goal g){
		current=s;
		this->size=size;
		this->g=g;
	}
	void insertMove(State s);
	int calHeuristic(calAction);
	State calculate();
	bool generated(calAction);
private:
	queue<calAction>move;
	priority_queue<calAction> openList;
	vector<calAction> closeList;
protected:
	State current;
	int size;
	goal g;
};

State singleGoalSolver::calculate(){
	calAction best;
	best.fVal=100;
	insertMove(current);
	for(int i = move.size();i > 0; i--){
		openList.push(move.front());
		move.pop();
	}
	int count=0;
	cout<<"A* search start..."<<endl;
	while(!openList.empty()){
		calAction bestNode;
		bestNode=openList.top();
		cout<<"state visited "<<count+1<<endl;
		bestNode.state.printBoard();
		openList.pop();
		if(bestNode.state.checkGoal(g)){
			if(best<bestNode){
				return bestNode.state;
			}
		}
		else{
			closeList.push_back(bestNode);
			insertMove(bestNode.state);
			for(int i = move.size();i > 0; i--){
				if(!generated(move.front())){
					openList.push(move.front());
				}
				move.pop();
			}
		}
		count++;
	}
}

void singleGoalSolver::insertMove(State s){
	queue<action>moveList=s.getMoveList();
	for(int i = moveList.size(); i > 0; i--){
		calAction cAction;
		cAction.des=moveList.front().des;
		cAction.src=moveList.front().src;
		cAction.state=s;
		cAction.state.moveBlock(cAction);
		cAction.heuristicVal=0;
		cAction.heuristicVal=calHeuristic(cAction);
		cAction.fVal=cAction.heuristicVal+s.getRec().size();
		move.push(cAction);
		moveList.pop();
	}
}

int singleGoalSolver::calHeuristic(calAction newAction){
	State state=newAction.state;
	location l=state.getLocation(g.val);
	if(state.checkGoal(g)){
		return 0;
	}
	int val=state.blockAbove(g.val)*2;

	if(l.row < (state.getSize(g.col))){
		val+=1;
	}

	if(state.getSize(g.col) > g.row){
		val+=state.getSize(g.col)-g.row;
	}else if(state.getSize(g.col) < g.row){
		val+=g.row-state.getSize(g.col);
	}
	if(l.col==g.col){val+=5;}
	return val;
}

bool singleGoalSolver::generated(calAction c){
	for(vector<calAction>::iterator i = closeList.begin();i!=closeList.end();i++){
		if(*i==c){
			return true;
		}
	}
	return false;
}
#endif /* SOLVER_H_ */
