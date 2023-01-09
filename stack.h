#ifndef STACK_d_H
#define STACK_d_H

#include <stack>
#include <deque>
#include <iostream>

class Stack {
private:
  // default deque: 
  std::deque<int> deque;
  
  // implement a default stack
  
  // user stacks: 
  std::stack<int> stack1;
  std::stack<int> stack2;
  std::stack<int> stack3;
  // user deques
  std::deque<int> deque1;
  std::deque<int> deque2;
  std::deque<int> deque3;
public:
  const char* stoc(std::string str) {
    const char* c = &*str.begin();
    return c;
  }  
public:
  // public deques and stacks are handled by the user, only the user deques and stacks will be handled by the this file
  // public stacks:- 
  std::stack<int> pstack1;
  std::stack<int> pstack2;
  std::stack<int> pstack3;
  std::stack<float> pstack4;
  // public deques:- 
  std::deque<int> pdeque1;
  #define pdeque pdeque1
  std::deque<int> pdeque2;
  std::deque<int> pdeque3;
  std::deque<float> pdeque4;
// deque code will be managed here
public:
  // TODO: check if the deques and stacks function are working correctly (the new funciton that were implemented)
  // FIXME: THE DEQUES ARE NOT WORKING RIGHT NGL

  void dpush_front(int deque=1, int x=0){
    if (deque==1){
      deque1.push_front(x);
    }
    else if (deque==2){
      deque2.push_front(x);
    }
    else if (deque==3){
      deque3.push_front(x);
    }
    else {
      std::cerr << "ERROR: deques are 1, 2 and 3 for now, invalid deque choice: " << deque << "\n";
      exit(-1);
    }
  }

  void dpush_back(int deque=1, int x=0){
    if (deque==1){
      deque1.push_back(x);
    }
    else if (deque==2){
      deque2.push_back(x);
    }
    else if (deque==3){
      deque3.push_back(x);
    }
    else {
      std::cerr << "ERROR: deques are 1, 2 and 3 for now, invalid deque choice: " << deque << "\n";
      exit(-1);
    }
  }
  
  int dtop(int deque=1){
    if (deque==1){
      if (deque1.size() >= 1) {
	int x = deque1.front();
	return x;
      }
      else {
	std::cout << "ERROR: deque size needs to be atleast 1, current deque size: " << deque1.size() << "\n";
	exit(-1);
      }
    }
    else if (deque==2){
      if (deque2.size() >= 1) {
	int x = deque2.front();
	return x;
      } 
      else {
	std::cout << "ERROR: deque size needs to be atleast 1, current deque size: " << deque2.size() << "\n";
	exit(-1);
      }
    }
    else if (deque==3){
      if (deque3.size() >= 1) { 
	int x = deque3.front();
	return x;
      }
      else {
	std::cout << "ERROR: deque size needs to be atleast 1, current deque size: " << deque3.size() << "\n";
	exit(-1);
      }
    }
    else {
      std::cerr << "ERROR: deques are 1, 2 and 3 for now, invalid deque choice: " << deque << "\n";
      exit(-1);
    }
  }

  // FIXME: THE BOTTOM FUNCTION IS RETURNING SEGSEGVs
  int dbottom(int deque=1){
    printf("ERROR: the function dbottom() right now is not working as intended, for now please use the public deques instead of the user deques");
    exit(-1);
    if (deque==1){
      if (deque1.size() > 1){
	int x = deque3.back();
	return x;      
      }
      else {
	std::cout << "ERROR: deque size needs to be more than 1, current deque size: " << deque1.size() << "\n";
	exit(-1);
      }
    }
    else if (deque==2){
      if (deque2.size() > 1) {
	int x = deque3.back();
	return x;
      }
      else {
	std::cout << "ERROR: deque size needs to be more than 1, current deque size: " << deque1.size() << "\n";
	exit(-1);
      }
    }
    else if (deque==3){
      if (deque3.size() > 1) {
	int x = deque3.back();
	return x;
      }
      else {
	std::cout << "ERROR: deque size needs to be more than 1, current deque size: " << deque3.size() << "\n";
	exit(-1);
      }
    }
    else {
      std::cerr << "ERROR: deques are 1, 2 and 3 for now, invalid deque choice: " << deque << "\n";
      exit(-1);
    }
  }
  
// stack code will be managed here
public:
  int spop(int stack){
    if (stack==1){
      int x = stack1.top();
      stack1.pop();
      return x;
    }
    else if (stack==2){
      int x = stack2.top();
      stack2.pop();
      return x;
    }
    else if (stack==3){
      int x = stack3.top();
      stack3.pop();
      return x;
    }
    else {
      std::cerr << "ERROR: stacks are 1, 2 and 3 for now, invalid stack choice: " << stack << "\n";
      exit(-1);
    } 
  }
  
  void spush(int stack, int x){
    if (stack==1){
      stack1.push(x);
    }
    else if (stack==2){
      stack2.push(x);
    }
    else if (stack==3){
      stack3.push(x);
    }
    else {
      std::cerr << "ERROR: stacks are 1, 2 and 3 for now, invalid stack choice: " << stack << "\n";
      exit(-1);
    }     
  }

  void sclear(int stackname){
    if (stackname==1){
      while (stack1.size() != 0){
	stack1.pop();
      }
    }
    else if (stackname==2){
      while (stack2.size() != 0){
	stack2.pop();
      }
    }
    else if (stackname==3){
      while (stack3.size() != 0){
	stack3.pop();
      }
    }
    else {
      std::cerr << "ERROR: stacks are 1, 2 and 3 for now, invalid stack choice: " << stackname << "\n";
      exit(-1);
    }   
  }

  void dclear(int dequename){
    if (dequename==1){
      deque1.clear();
    }
    else if (dequename==2){
      deque2.clear();
    }
    else if (dequename==3){
      deque3.clear();
    }
    else {
      std::cerr << "ERROR: deques are 1, 2 and 3 for now, invalid deque choice: " << dequename << "\n";
      exit(-1);
    }   
  }
  
};


#endif // STACK_d_H
