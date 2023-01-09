#include <iostream>
#include <string>
#include <stack>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <vector>
#include <map>
#include <cassert>
#include <tuple>

#define null NULL

bool ends_with(const std::string &filename, const std::string &ext)
{
  return ext.length() <= filename.length() &&
    std::equal(ext.rbegin(), ext.rend(), filename.rbegin());
}

void usage(int ec){
  printf("Usage: ./stack [SUBCOMMAND]\n");
  printf("SUBCOMMANDS:- \n");
  printf("              com              - compiles the code to nasm assembly\n");
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

std::map<std::string, std::string> OPS{ { "OP_PLUS", "+" }, { "OP_MINUS", "-" }, { "OP_PRINT", "print" }, { "OP_EQUAL", "=" }, { "OP_IF", "if" }, { "OP_END", "end" }, { "OP_ELSE", "else" } };

std::vector<std::tuple<int, int>> crossreference_blocks(std::vector<std::string> program) {
  std::stack<int> stack;
  std::vector<std::tuple<int, int>> vec;
  std::size_t ip = 0;
  while (program.size() > ip) {
    assert(OPS.size() == 7);
    if (program[ip] == OPS["OP_IF"]) {
      stack.push(ip);
    }
    // TODOO: implement else token
    // else if (program[ip] == OPS["OP_ELSE"]) {
    //   int if_ip = stack.top();
    //   stack.pop();
    // }
    else if (program[ip] == OPS["OP_END"]) {
      int if_ip = stack.top();
      stack.pop();
      if (program[if_ip] != OPS["OP_IF"]) {
	printf("End can only close if blocks for now");
	exit(-1);
      }
      vec.push_back(std::make_tuple(if_ip, ip));
    }
    ip += 1;
  }
  return vec;
}

void compile_program(std::vector<std::string> program, std::string fd) {
  Stack stack;
  std::ofstream outdata;
  int StackExtLen    = 6; // stack is a ext of len 5 and adding the dot gives us lenght 6
  std::string AsmExt = ".asm";
  int AsmExtLen      = 4; // asm is a ext of len 3 and adding the dot gives us lenght 4
  std::string filename = fd.erase(fd.length()-StackExtLen);
  std::string outasm = filename + AsmExt;
  outdata.open(outasm);
  if(!outdata) { // file couldn't be opened
    std::cerr << "ERROR: the file could not be opened" << "\n";
    exit(1);
  }

  long unsigned int ip = 0;
  
  // Thank you tsoding.
  outdata << "BITS 64\n";
  outdata << "segment .text\n";
  outdata << "print:\n";
  outdata << "    mov     r9, -3689348814741910323\n";
  outdata << "    sub     rsp, 40\n";
  outdata << "    mov     BYTE [rsp+31], 10\n";
  outdata << "    lea     rcx, [rsp+30]\n";
  outdata << ".L2:\n";
  outdata << "    mov     rax, rdi\n";
  outdata << "    lea     r8, [rsp+32]\n";
  outdata << "    mul     r9\n";
  outdata << "    mov     rax, rdi\n";
  outdata << "    sub     r8, rcx\n";
  outdata << "    shr     rdx, 3\n";
  outdata << "    lea     rsi, [rdx+rdx*4]\n";
  outdata << "    add     rsi, rsi\n";
  outdata << "    sub     rax, rsi\n";
  outdata << "    add     eax, 48\n";
  outdata << "    mov     BYTE [rcx], al\n";
  outdata << "    mov     rax, rdi\n";
  outdata << "    mov     rdi, rdx\n";
  outdata << "    mov     rdx, rcx\n";
  outdata << "    sub     rcx, 1\n";
  outdata << "    cmp     rax, 9\n";
  outdata << "    ja      .L2\n";
  outdata << "    lea     rax, [rsp+32]\n";
  outdata << "    mov     edi, 1\n";
  outdata << "    sub     rdx, rax\n";
  outdata << "    xor     eax, eax\n";
  outdata << "    lea     rsi, [rsp+32+rdx]\n";
  outdata << "    mov     rdx, r8\n";
  outdata << "    mov     rax, 1\n";
  outdata << "    syscall\n";
  outdata << "    add     rsp, 40\n";
  outdata << "    ret\n";
  outdata << "global _start\n";
  outdata << "_start:\n";

  std::vector<std::tuple<int, int>> ifpos = crossreference_blocks(program);
  int ifp = 0;
  
  // for debuging perposes
  // ------------------------------------------------
  // for (auto [x,y] : ifpos){
  //   std::cout << "{" << x << ", " << y << "}\n";
  // }

  // for (std::string ip : program) {
  //   std::cout << ip << "\n";
  // }
  // ------------------------------------------------

  assert(OPS.size() == 7 && "Exhaustable handling of opeprations in compile_program()");

  // TODOOO: implement else block in crossreference_blocks
  // TODO: implement else block in compile_program 
  // reference: https://youtu.be/QlKSCn-Al6Q?t=3822
  
  while (program.size() > ip){
    if (is_digits(program[ip]) == 1) {
      outdata << "    ;; -- push --\n";
      outdata << "    push " << program[ip] << "\n";
    }
    else if (program[ip] == OPS["OP_PRINT"]) {
      outdata << "    ;; -- print --\n";
      outdata << "    pop rdi\n";
      outdata << "    call print\n";
    }
    
    else if (program[ip] == OPS["OP_PLUS"]) {
      outdata << "    ;; -- plus --\n";
      outdata << "    pop rax\n";
      outdata << "    pop rbx\n";
      outdata << "    add rax, rbx\n";
      outdata << "    push rax\n";
    }

    else if (program[ip] == OPS["OP_MINUS"]) {
      outdata << "    ;; -- minus --\n";
      outdata << "    pop rax\n";
      outdata << "    pop rbx\n";
      outdata << "    sub rbx, rax\n";
      outdata << "    push rbx\n";
    }

    else if (program[ip] == OPS["OP_EQUAL"]) {
      outdata << "    ;; -- equal --\n";
      outdata << "    mov rcx, 0\n";	
      outdata << "    mov rdx, 1\n";	
      outdata << "    pop rax\n";	
      outdata << "    pop rbx\n";  
      outdata << "    cmp rax, rbx\n";	
      outdata << "    cmove rcx, rdx\n";
      outdata << "    push rcx\n";
    }
    
    else if (program[ip] == OPS["OP_IF"]) {
      static_assert(std::tuple_size_v<std::decay_t<decltype(ifpos[ifp])>> >= 2, "`if` instruction does not have a reference to the end of its block\n");
      int jc = std::get<1>(ifpos[ifp]);
      ifp++;
      outdata << "    ;; -- if --\n";
      outdata << "    pop rax\n";
      outdata << "    test rax, rax\n";
      outdata << "    jz addr_" << jc << "\n";
    }

    else if (program[ip] == OPS["OP_END"]) {
      outdata << "addr_" << ip << ":" << "\n";
    }

    // else if (program[ip] == OPS["OP_ELSE"]) {
      
    // }
    
    else {
      std::cerr << "ERROR: unreachable: " << program[ip] << "\n";
      exit(1);
    }
    
    ip += 1;
  }
  
  outdata << "    mov rax, 60\n";
  outdata << "    mov rdi, 0\n";
  outdata << "    syscall\n";
  
  outdata.close();

  // get the raw_filename by erasing the extension
  std::string raw_filename = outasm.erase(outasm.length()-AsmExtLen);
  // after the erase function mutates the string outasm, re-concatenate the asm extension into the filename 
  outasm = outasm + AsmExt;

  // command definations and filenames
  std::string linked_filename = raw_filename + ".o";
  std::string nasm_compile_command_str = "nasm -felf64 " + outasm;
  std::string ld_command_str           = "ld -o " + raw_filename + " " + linked_filename;

  // print the commands that will be executed 
  std::cout << nasm_compile_command_str << "\n";
  std::cout << ld_command_str           << "\n";

  // execute the commands to compile the assembly file into a static exe
  system(stack.stoc(nasm_compile_command_str));
  system(stack.stoc(ld_command_str));
}

// TODO: implement a stack.h version of interpret_read_program();
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

    else if (ip == "*"){
      if (stack.size() >= 2){
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	int c = b * a;
	stack.push(c);
      }
      else {
        std::cout << "`*` operation needs at least 2 elements on the stack" << "\n";
      }
    }

    else if (ip == "mod"){
      if (stack.size() >= 2){
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	int c = b % a;
	stack.push(c);
      }
      else {
	std::cout << "`mod` operation needs at least 2 elements on the stack" << "\n"; 
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

    else if (ip == "*"){
      if (stack.size() >= 2){
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	int c = b * a;
	stack.push(c);
      }
      else {
        std::cout << "`*` operation needs at least 2 elements on the stack" << "\n";
      }
    }

    else if (ip == "mod"){
      if (stack.size() >= 2){
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	int c = b % a;
	stack.push(c);
      }
      else {
	std::cout << "`mod` operation needs at least 2 elements on the stack" << "\n"; 
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
        std::cout << "the `drop-all` intrinsic needs at least 1 element on the stack" << "\n";
      }      
    }
    
    else if (ip == "over"){
      if (stack.size() >= 2){
<<<<<<< HEAD
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	stack.push(b);
	stack.push(a);
	stack.push(b);
=======
	printf("not implemented yet");
	exit(0);
>>>>>>> 29f94a9c7286bebb0ad268d9b1de981911c2c7b6
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

void interpret_program1(){
  std::string ip;
  Stack stack;
  while (true) {
    std::cin >> ip;
    if (is_digits(ip) == 1){
      stack.pstack1.push(stoi(ip));
    }
<<<<<<< HEAD
=======

>>>>>>> 29f94a9c7286bebb0ad268d9b1de981911c2c7b6
    else if (ip == "print"){
      if (stack.pstack1.size() == 0){
	std::cout << "warning: stack is empty" << std::endl;
      }
      else {
        std::cout << stack.pstack1.top() << "\n";
        stack.pstack1.pop();
      }
    }

    else if (ip == "+"){
      if (stack.pstack1.size() >= 2) {
	int a = stack.pstack1.top();
	stack.pstack1.pop();
	int b = stack.pstack1.top();
	stack.pstack1.pop();
	int c = a + b;
	stack.pstack1.push(c);
      }
      else {
        std::cout << "`+` operation needs at least 2 elements on the stack" << "\n";
      }
    }

    else if (ip == "-"){
      if (stack.pstack1.size() >= 2){
	int a = stack.pstack1.top();
	stack.pstack1.pop();
	int b = stack.pstack1.top();
	stack.pstack1.pop();
	int c = b - a;
	stack.pstack1.push(c);
      }
      else {
        std::cout << "`-` operation needs at least 2 elements on the stack" << "\n";
      }
    }
    
    else if (ip == "*"){
      if (stack.pstack1.size() >= 2){
	int a = stack.pstack1.top();
	stack.pstack1.pop();
	int b = stack.pstack1.top();
	stack.pstack1.pop();
	int c = b * a;
	stack.pstack1.push(c);
      }
      else {
        std::cout << "`*` operation needs at least 2 elements on the stack" << "\n";
      }
    }

    else if (ip == "mod"){
      if (stack.pstack1.size() >= 2){
	int a = stack.pstack1.top();
	stack.pstack1.pop();
	int b = stack.pstack1.top();
	stack.pstack1.pop();
	int c = b % a;
	stack.pstack1.push(c);
      }
      else {
	std::cout << "`mod` operation needs at least 2 elements on the stack" << "\n"; 
      }
    }
    
    else if (ip == "drop"){
      if (stack.pstack1.size() >= 1){
	int tmp = stack.pstack1.top();
        stack.pstack1.pop();
	if (stack.pstack2.size() == 90){
	  std::cout << "Warning: backup stack has  90 elements now, when it reaches 100, the backup stack will be cleared";
	}
	else if (stack.pstack2.size() >= 100){
	  while (stack.pstack2.empty()==0){
	    std::cout << "NOTE: deleted all elements from stack2 (backup stack)";
	    stack.pstack2.pop();
	  }
	}
	stack.pstack2.push(tmp);
      }
      else {
        std::cout << "the `drop` intrinsic needs at least 1 element on the stack" << "\n";
      }
    }

    else if (ip == "*"){
      if (stack.size() >= 2){
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	int c = b * a;
	stack.push(c);
      }
      else {
        std::cout << "`*` operation needs at least 2 elements on the stack" << "\n";
      }
    }

    else if (ip == "mod"){
      if (stack.size() >= 2){
	int a = stack.top();
	stack.pop();
	int b = stack.top();
	stack.pop();
	int c = b % a;
	stack.push(c);
      }
      else {
	std::cout << "`mod` operation needs at least 2 elements on the stack" << "\n"; 
      }
    }
    
    else if (ip == "swap"){
      if (stack.pstack1.size() >= 2){
	int a = stack.pstack1.top();
	stack.pstack1.pop();
	int b = stack.pstack1.top();
	stack.pstack1.pop();
	stack.pstack1.push(a);
	stack.pstack1.push(b);
      }
      else {
        std::cout << "the `swap` intrinsic needs at least 2 elements on the stack" << "\n";
      }
    }

    else if (ip == "dup"){
      if (stack.pstack1.size() >= 1){
	int a = stack.pstack1.top();
	stack.pstack1.pop();
	stack.pstack1.push(a);
	stack.pstack1.push(a);
      }	
      else{
        std::cout << "the `dup` intrinsic needs at least 1 element on the stack" << "\n";
      }
    }

    else if (ip == "drop-all"){
      if (stack.pstack1.size() >= 1){
	while (stack.pstack1.empty()==0){
	  stack.pstack1.pop();
	}
      }
      else {
        std::cout << "the `drop-all` intrinsic needs at least 1 element on the stack" << "\n";
      }      
    }
      
    else if (ip == "over"){
      if (stack.pstack1.size() >= 2){
	int a = stack.pstack1.top();
	stack.pstack1.pop();
	int b = stack.pstack1.top();
	stack.pstack1.pop();
	stack.pstack1.push(b);
	stack.pstack1.push(a);
	stack.pstack1.push(b);
       
      }
      else{
	std::cout << "the `over` intrinsic needs at least 2 elements on the stack" << "\n";
      }
    }
    
    else if (ip == "rot"){
      if (stack.pstack1.size() >= 3) {
	int a = stack.pstack1.top();
	stack.pstack1.pop();
	int b = stack.pstack1.top();
	stack.pstack1.pop();
	int c = stack.pstack1.top();
	stack.pstack1.pop();
	stack.pstack1.push(a);
	stack.pstack1.push(b);
	stack.pstack1.push(c);
      }
      else {
	std::cout << "the `rot` intrinsic needs at least 3 elements on the stack" << "\n";
      }
    }
<<<<<<< HEAD

    else if (ip == "!drop"){
      if (stack.pstack2.size() >= 1) {
	int tmp = stack.pstack2.top();
	stack.pstack2.pop();
	stack.pstack1.push(tmp);
      }
      else {
	std::cout << "the `!drop` intrinsic needs at least 1 element on the backup stack" << "\n";
      }
    }

    else if (ip == "size1"){
      std::cout << "Note: size of stack1: " << stack.pstack1.size() << "\n";
    }

    else if (ip == "size2") {
      std::cout << "Note: size of stack2: " << stack.pstack2.size() << "\n";
    }

    else if (ip == "size3") {
      std::cout << "Note: size of stack3: " << stack.pstack3.size() << "\n";
    }    

    // TODO: implemenent multiple stack operators
=======
>>>>>>> 29f94a9c7286bebb0ad268d9b1de981911c2c7b6
    
    else if (ip == "???") {
      if (stack.pstack1.empty()){
	std::cout << "There is nothing on the stack currently" << "\n";
      }
      else{
	PrintStack(stack.pstack1);
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
  std::stack<int> stack;
<<<<<<< HEAD
   
=======
  
>>>>>>> 29f94a9c7286bebb0ad268d9b1de981911c2c7b6
  if (argc < 2){
    usage(1);
  }
  std::string filepath;
  
  if(strncmp(argv[1],"sim", 8)==0){
    // interpret_program(stack, ip);
    interpret_program1();
  }
  else if (strncmp(argv[1], "read", 8) == 0){
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
  // TODO: add a -r flag
  else if (strncmp(argv[1], "com", 8)  == 0){
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
      compile_program(words, filepath);
      file.close();
    }
    else {
      printf("ERROR: file needs to end with .stack extension");
      exit(1);
    }    
  }
  else if (strncmp(argv[1], "help", 8) == 0){
    usage(0);
  }
  else {
    std::cout << "ERROR: unknown subcommand:- `" << argv[1] << "`" << "\n";
    usage(-1); 
  }

  return 0;
}
