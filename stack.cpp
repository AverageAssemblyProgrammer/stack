#include <iostream>
#include <string>
#include <stack>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <vector>

bool ends_with(const std::string &filename, const std::string &ext)
{
  return ext.length() <= filename.length() &&
         std::equal(ext.rbegin(), ext.rend(), filename.rbegin());
}

void usage(int ec){
  printf("Usage: ./stack [SUBCOMMAND]\n");
  printf("SUBCOMMANDS:- \n");
  printf("              sim              - simulates the given input\n");
  printf("              read <filename>  - simulates the given file\n");
  printf("              help             - prints this help screen with exit code 0\n");
  exit(ec);
}

bool is_digits(const std::string &str)
{
  // this will return 1 if the string contains only numbers else return 0
  return str.find_first_not_of("0123456789") == std::string::npos;
}

void PrintStack(std::stack<int> s)
{
  if (s.empty()) return;
  int x = s.top();
  s.pop();
  std::cout << ":- " << x << ' ' << "\n";
  PrintStack(s);
  s.push(x);
}

void interpret_program(std::stack<int> stack, std::string ip){
  while (true){
    std::cin >> ip;
    
    // pushing integers to stack 
    if (is_digits(ip) == 1){
      stack.push(stoi(ip));
    }

    // printing the first element of the stack 
    else if (ip == "print"){
      if (stack.size() == 0){
	std::cout << "warning: stack is empty" << std::endl;
      }
      else {
        std::cout << stack.top() << "\n";
        stack.pop();
      }
    }

    else if (ip == "+"){
      if (stack.size() >= 2) {
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	int c = b + a;
	stack.push(c);
      }
      else {
        std::cout << "`+` operation needs at least 2 elements on the stack" << "\n";
      }
    }

    else if (ip == "-"){
      if (stack.size() >= 2){
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	int c = b - a;
	stack.push(c);
      }
      else {
        std::cout << "`-` operation needs at least 2 elements on the stack" << "\n";
      }
    }

    else if (ip == "drop"){
      if (stack.size() >= 1){
        stack.pop();
      }
      else {
        std::cout << "the `drop` intrinsic needs at least 1 element on the stack" << "\n";
      }
    }

    else if (ip == "swap"){
      if (stack.size() >= 2){
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	stack.push(a);
	stack.push(b);
      }
      else {
        std::cout << "the `swap` intrinsic needs at least 2 elements on the stack" << "\n";
      }
    }

    else if (ip == "dup"){
      if (stack.size() >= 1){
	int a = stack.top();
	stack.pop();
	stack.push(a);
	stack.push(a);
      }	
      else{
        std::cout << "the `dup` intrinsic needs at least 1 element on the stack" << "\n";
      }
    }
    else if (ip == "drop-all"){
      if (stack.size() >= 1){
	while (stack.empty()==0){
	  stack.pop();
	}
      }
      else {
        std::cout << "the `drop all` intrinsic needs at least 1 element on the stack" << "\n";
      }      
    }
      
    // TODO: implement over intrinsic
    else if (ip == "over"){
      if (stack.size() >= 2){
	printf("Not implemented yet: `over`");
	exit(1);
      }
      else{
	std::cout << "the `over` intrinsic needs at least 2 elements on the stack" << "\n";
      }
    }

    else if (ip == "rot"){
      if (stack.size() >= 3) {
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	int c = stack.top();
	stack.pop();
	stack.push(a);
	stack.push(b);
	stack.push(c);
      }
      else {
	std::cout << "the `rot` intrinsic needs at least 3 elements on the stack" << "\n";
      }
    }

    else if (ip == "???") {
      if (stack.empty()){
	std::cout << "There is nothing on the stack currently" << "\n";
      }
      else{
	PrintStack(stack);
      }
    }
    
    else if (ip == "exit"){
      exit(0);
    }
    
    else {
      std::cout << "not implemented: " << ip << std::endl;
    }
    
  }
}

void interpret_read_program(std::stack<int> stack, std::vector<std::string> ips){
  for (std::string ip : ips) {
    if (is_digits(ip) == 1){
      stack.push(stoi(ip));
    }

    // printing the first element of the stack 
    else if (ip == "print"){
      if (stack.size() == 0){
	std::cout << "warning: stack is empty" << std::endl;
      }
      else {
        std::cout << stack.top() << "\n";
        stack.pop();
      }
    }

    else if (ip == "+"){
      if (stack.size() >= 2) {
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	int c = b + a;
	stack.push(c);
      }
      else {
        std::cout << "`+` operation needs at least 2 elements on the stack" << "\n";
      }
    }

    else if (ip == "-"){
      if (stack.size() >= 2){
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	int c = b - a;
	stack.push(c);
      }
      else {
        std::cout << "`-` operation needs at least 2 elements on the stack" << "\n";
      }
    }

    else if (ip == "drop"){
      if (stack.size() >= 1){
        stack.pop();
      }
      else {
        std::cout << "the `drop` intrinsic needs at least 1 element on the stack" << "\n";
      }
    }

    else if (ip == "swap"){
      if (stack.size() >= 2){
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	stack.push(a);
	stack.push(b);
      }
      else {
        std::cout << "the `swap` intrinsic needs at least 2 elements on the stack" << "\n";
      }
    }

    else if (ip == "dup"){
      if (stack.size() >= 1){
	int a = stack.top();
	stack.pop();
	stack.push(a);
	stack.push(a);
      }	
      else{
        std::cout << "the `dup` intrinsic needs at least 1 element on the stack" << "\n";
      }
    }
    else if (ip == "drop-all"){
      if (stack.size() >= 1){
	while (stack.empty()==0){
	  stack.pop();
	}
      }
      else {
        std::cout << "the `drop all` intrinsic needs at least 1 element on the stack" << "\n";
      }      
    }
      
    // TODO: implement over intrinsic
    else if (ip == "over"){
      if (stack.size() >= 2){
	printf("Not implemented yet: `over`");
	exit(1);
      }
      else{
	std::cout << "the `over` intrinsic needs at least 2 elements on the stack" << "\n";
      }
    }

    else if (ip == "rot"){
      if (stack.size() >= 3) {
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	int c = stack.top();
	stack.pop();
	stack.push(a);
	stack.push(b);
	stack.push(c);
      }
      else {
	std::cout << "the `rot` intrinsic needs at least 3 elements on the stack" << "\n";
      }
    }

    else if (ip == "???") {
      if (stack.empty()){
	std::cout << "There is nothing on the stack currently" << "\n";
      }
      else{
	PrintStack(stack);
      }
    }
    
    else if (ip == "exit"){
      exit(0);
    }
    
    else {
      std::cout << "not implemented: " << ip << std::endl;
    }
  }
}

int main(int argc, char* argv[]){
  std::string ip;
  std::stack<int> stack;

  if (argc < 2){
    usage(1);
  }
  std::string filepath;
  
  if(strcmp(argv[1],"sim")==0){
    interpret_program(stack, ip);
  }
  else if (strcmp(argv[1], "read") == 0){
    if (argc != 3){
      printf("ERROR: filename not provided\n");
      usage(1);
    }
    filepath = argv[2];
    // TODO: use boost filesystem later
    if (std::filesystem::is_directory(filepath) == 1){
      printf("ERROR: is a directory");
      exit(1);
    }
    if (std::filesystem::exists(filepath) == 0){
      printf("ERROR: file does not exist");
      exit(1);
    }
    if (ends_with(filepath, ".stack")){
      std::ifstream file(filepath);
      std::vector<std::string> words;
      std::string foundWord;
      while (file >> foundWord) {
	words.push_back(foundWord);
      }
      interpret_read_program(stack, words);
      file.close();
    }
    else {
      printf("ERROR: file needs to end with .stack extension");
      exit(1);
    }    
  }
  else if (strcmp(argv[1], "help") == 0){
    usage(0);
  }
  else {
    usage(1); 
  }

  return 0;
}
