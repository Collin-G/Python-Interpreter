#include <iostream>
#include <sstream>
#include <fstream>
#include "token_reader.cpp"
#include <vector>
#include <string.h>
#include <string>
#include <cmath>
#include <cctype>

int expected_indents{0};
int count{0};

int while_pos{0};
int while_tabs{0};

bool loop{false};


//variable storage
std::vector<tokens> variables; 
//stores textfile
std::vector<tokens> storage;
//stores line for parsing
std::vector<tokens> line_storage;


void openFile(std::ifstream& inFile, std::string fname) {
    inFile.open(fname);
}


//read file line by line, char by char
void processFile(std::ifstream& inFile){
    
    std::string line, word;
    std::istringstream iss;
    while(!inFile.eof()){

        getline(inFile, line);
        int space_count{0};
        tokens newline("new","newline");
        storage.push_back(newline);
        
        for(int i{0}; line[i] == ' '; ++i){
            if(line[i] == ' '){
                ++space_count;
                if(space_count%4 == 0){
                    tokens indent("\t", "indent");
                    storage.push_back(indent);
                }
                
            }
            
        }
        
        if(inFile.good()) {
            iss.clear();
            iss.str(line);
            
            while(iss.good()){
                   
                iss >> word;
                
                
                std::string base{};
                for(int i{0}; i < word.length(); i++){
                    base+= word[i];
                    
                    
                    if(base == "if"){
                        tokens entry(base,"statement");
                        
                        storage.push_back(entry);
                       
                        base.clear();
                        
                    }

                    

                   if(base == "while"){
                        tokens entry(base,"statement");
                        
                        storage.push_back(entry);
                       
                        base.clear();
                        
                    }
                    
                    if(base == "print" ){

                        tokens entry(base,"statement");
                        
                        storage.push_back(entry);
                       
                        base.clear();
                        
                
                    }

                    if((isdigit(word[i])) && !isdigit(word[i+1]) && word[i+1] != '.'){
                        tokens entry(base,"int");
                        storage.push_back(entry);
                        base.clear();
                        

                    }

                    if((isalpha(base[0]) || base[0] == '_') && (!isdigit(word[i+1]) && !isalpha(word[i+1]))){
                        tokens entry(base, "var");
                        storage.push_back(entry);
                        base.clear();
                    }
                    
                    
                    if(base == "("){
                        tokens entry(base,"op");
                        storage.push_back(entry);
                        base.clear();
                        
                        
                    }
                    
                    if(base == ")"){
                        tokens entry(base,"op");
                        storage.push_back(entry);
                        base.clear();
                        


                    }

                    if(base == "int"){
                        tokens entry(base,"int_declaration");
                        storage.push_back(entry);
                        base.clear();
                        
                    }

                    if(base == "+"){
                        tokens entry(base,"op");
                        storage.push_back(entry);
                        base.clear();
                        
                    }

                    if(base == "-"){
                        tokens entry(base,"op");
                        storage.push_back(entry);
                        base.clear();
                    }
                    
                    if(base == "*"){
                        tokens entry(base,"op");
                        storage.push_back(entry);
                        base.clear();
                    }
                    
                     if(base == "%"){
                        tokens entry(base,"op");
                        storage.push_back(entry);
                        base.clear();
                    }

                    if(base == "/"){
                        tokens entry(base,"op");
                        storage.push_back(entry);
                        base.clear();
                    } 

                    if(base == "=" && word[i+1] != '='){
                        tokens entry(base,"assignment");
                        storage.push_back(entry);
                        base.clear();
                    }

                    if(base == "<" && word[i+1] != '='){
                        tokens entry(base,"comparison");
                        storage.push_back(entry);
                        base.clear();
                    }

                    if(base == ">" && word[i+1] != '='){
                        tokens entry(base, "comparison");
                        storage.push_back(entry);
                        base.clear();
                    }
                    if(base == "!="){
                        tokens entry(base, "comparison");
                        storage.push_back(entry);
                        base.clear();
                    }
                    if(base == "=="){
                        
                        tokens entry(base, "comparison");
                        storage.push_back(entry);
                        base.clear();
                    }

                    if(base == "<="){
                        
                        tokens entry(base, "comparison");
                        storage.push_back(entry);
                        base.clear();
                    }

                    if(base == ">="){
                        
                        tokens entry(base, "comparison");
                        storage.push_back(entry);
                        base.clear();
                    }


                
                
                }


            
            }
            
            
        }
    }
}

//execute line of file
void execute_line(std::vector<tokens> &s){
    int start{0};
    int end{100};
    int indents{0};
    for(int i{0}; i<s.size(); ++i){
        if(s[i].get_type() == "indent"){
            ++indents;
        }
        for(int k{0}; k < variables.size(); k++){
                if(s[i].get_symbol() == variables[k].get_type() && s[i+1].get_symbol() != "="){
                    s[i].set_symbol(variables[k].get_symbol());
                    if(isdigit(s[i].get_symbol()[0])){
                        s[i].set_type("int");
                    }
                    else{
                        s[i].set_type("string");
                    }
                    
                    
                }
        }
        
    
    }
    
    if(indents < expected_indents && count == expected_indents){
        
        --count;
        expected_indents = count;
        
    }

    

    if(indents != expected_indents && count != indents){
        return;
    }

    
   
    for(int i{0}; i<s.size(); ++i){

        
        if(s[i].get_type() == "comparison"){
            if (s[expected_indents].get_symbol() == "print") {
                std::cout<<comparison(i, 0, 100, s)<<std::endl;
            }
            else if (s[expected_indents].get_symbol() == "if" && comparison(i, 0, 100, s)){///highly sus
                ++expected_indents;
                ++count;

            }

            else if (s[expected_indents].get_symbol() == "while" && comparison(i, 0, 100, s)){
                if(loop == false){
                    ++count;
                    ++expected_indents;
                    while_tabs = indents;
                }
                
                loop = true;


            }
            return;
        }
        if(s[i].get_symbol() == "="){
            assignment(i, 0, 100, s, variables);
            return;
            
        }
       

        
    
        
    }
    if(s[indents].get_symbol() == "print"){
            if(s[1+indents].get_type() == "int" || s[1+indents].get_symbol() == "op"){
                head_tail(start, end,s);
                preliminary(start, end, s);
                std::cout<<arithmetic(start,end,s)<<std::endl;
                return;
            }
        }
   
   

}


int main(){

    std::ifstream inFile;
    openFile(inFile, "file.py");
    if(inFile.is_open()){
        std::cout<<"scanning contents: "<<std::endl;
    }
    else{
        std::cout<<"failed to open file."<<std::endl;
        exit(-1);
    }

    processFile(inFile);

    

    for(int i{0}; i < storage.size(); ++i){
        
        if(expected_indents == while_tabs && loop == true){
                //if condition is met, go back to beginning of loop
                i = while_pos;
                line_storage.clear();
                loop = false;    
            }
            

        if(storage[i].get_symbol() == "while"){
            //marks start of while loop
            while_pos = i;
            
            
            
            
        }

        line_storage.push_back(storage[i]);
        if(storage[i].get_symbol() == "new"){
            if(line_storage.size() > 1){
                execute_line(line_storage);
            }
            line_storage.clear();
            
            
        }
    }
    
  
    
    return 0;
}



