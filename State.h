/*
 * State.h
 *
 *  Created on: 11 Apr 2020
 *      Author: johnathanliao
 */
#ifndef STATE_H_
#define STATE_H_
#include <ctime>
#include<stdlib.h>
using namespace std;

struct action{
	int des;
	int src;
};

struct goal{
	int val;
	int row;
	int col;
	bool operator==(goal g){
		if(g.val==this->val && g.row == this->row && g.col == this->col){return true;}
		return false;
	}
};

struct location{
	int row;
	int col;
};

class State{
private:
	queue<action>recMove;
	int size;
	int blockNum;
	stack<int> board[3];

public:
	State(){
		size=3;
		blockNum=6;
	}
	~State(){
		for(int i = size; i < 0; i++){
			board[i].pop();
		}
	}
	State(const State &s){
		size=s.size;
		blockNum=s.blockNum;
		recMove=s.recMove;
		for(int i = 0; i < 3; i++){
			board[i]=s.board[i];
		}
        recMove=s.recMove;
	}
	void initialize();
	//initialize the board with random status
	void printBoard();
	//print the board in visualize format
	queue<action>getRec(){return recMove;};
	int getSize(int col){return board[col].size();};
	location getLocation(int block);
	void moveBlock(action);
	bool checkGoal(goal);
	int blockAbove(int);
	queue<action>getMoveList();
	void operator=(State);
	bool operator==(State);
	bool operator<(State);
};

//print the board
void State:: printBoard(){
	string val;
	for(int i = size-1; i >= 0; i--){
		cout<<"|";
		for(int j = 0; j < size; j++){
			if(!board[j].empty()&&board[j].size() > i){
				stack<int>temp=board[j];
				int n = temp.size();
				while(n > i+1){
					temp.pop();
					n--;
				}
				val=to_string(temp.top());
			}
			else{
				val=" ";
			}
			cout<<val<<"|";
		}
		cout<<endl;
	}
	cout<<endl;
}

//generate an random order board
void State:: initialize(){
	int count=size*size;
	srand(time(0));
	vector<int>randList;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			randList.push_back(j);
		}
	}

	for(int i = 1; i <= blockNum; i++){
		int position=rand()%count;
		board[randList[position]].push(i);
		randList[position]=randList.back();
		randList.pop_back();
		count--;
	}
	randList.clear();
	randList.shrink_to_fit();
}


void State:: moveBlock(action newAction){
	int source,des;
	source=newAction.src;
	des=newAction.des;
	if(!board[source].empty() &&board[des].size()!=size && source!=des){
		board[des].push(board[source].top());
		board[source].pop();
	}
	recMove.push(newAction);
}


bool State::checkGoal(goal g){
	stack<int>temp=board[g.col];
	if(temp.size()!=0 && temp.size()-1==g.row){
		for(int i = temp.size()-1;i > 0 &&i!=g.row;i--){
			temp.pop();
		}
		if(temp.top()==g.val){
			return true;
		}
	}
	return false;
}

int State::blockAbove(int block){
	for(int i = 0; i < size; i++){
		int blockNum=0;
		stack<int>temp=board[i];
		for(int j = temp.size(); j > 0; j--){
			if(temp.top()==block){
				return blockNum;
			}
			else{
				blockNum+=1;
				temp.pop();
			}
		}
	}
}

location State::getLocation(int block){
	for(int i = 0; i < size; i++){
		int blockNum=0;
		stack<int>temp=board[i];
		for(int j = temp.size()-1; j >= 0; j--){
			if(temp.top()==block){
				location l;
				l.row=j;
				l.col=i;
				return l;
			}
			else{
				temp.pop();
			}
		}
	}
}

queue<action>State::getMoveList(){
	queue<action>moveList;
	for(int i = 0; i < size ;i++){
		if(!board[i].empty()){
			for(int j = 0; j < size ; j++){
				if( j!=i && board[j].size() < size){
					action newMove;
					newMove.des=j;
					newMove.src=i;
					moveList.push(newMove);
				}
			}
		}
	}
	return moveList;
}

void State::operator=(State s){
	this->size=s.size;
	this->blockNum=s.blockNum;
	for(int i = 0; i < size; i++){
		this->board[i]=s.board[i];
	}
	this->recMove=s.recMove;
};
bool State::operator==(State s){
	for(int i = 0; i < size; i++){
		if(this->board[i]!=s.board[i]){return false;}
	}
	return true;
}
bool State:: operator<(State s){
	if(this->recMove.size()<s.recMove.size()){
		return true;
	}
	return false;
}

#endif /* STATE_H_ */
