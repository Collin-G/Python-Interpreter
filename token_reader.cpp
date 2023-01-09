#include <iostream>
#include <vector>
#include <cmath>


class tokens{

    public:
        tokens(std::string symb, std::string type){
            _symb = symb;
            _type = type;
            
        }

        void set_symbol(std::string str){
            _symb = str;
        }

        void set_type(std::string str){
            _type = str;
        }

        std::string get_symbol(){
            return _symb;
        }

        std::string get_type(){
            return _type;
        }

    private:
        std::string _symb{};
        std::string _type{};
        



};

bool in_paren(int a, int b, int c, std::vector<tokens> s){
    int total{0};
    
    int paren_count{0};

    for(int i{a}; i <= c; ++i){
        
        if(s[i].get_symbol() == "(" || s[i].get_symbol() == ")"){
            
            ++paren_count;
        }
    }


    for(int i{a}; i <= c ;++i){
        if(s[i].get_symbol() == "("){

            ++total;
        }
        if(s[i].get_symbol() == ")"){
            break;
        }
    }

    if(total == 1 && total == paren_count/2 && s[a].get_symbol() == "(" && s[c].get_symbol() == ")"){
        
        return false;
    }



    if(total > 1 && paren_count/2 >= total && s[a].get_symbol() == "(" && s[c].get_symbol() == ")"){
        int count = 0;
        for(int i{a+1}; i<=b && i < c; i++){
            if(s[i].get_symbol() == "("){
                ++count;
                
            }
            
            if(s[i].get_symbol() == ")"){
                --count;
            }

        }

        if(count != 0){
            return true;
        }
    }

   
    else {
        int count = 0;
        for(int i{a}; i<=b; i++){
            if(s[i].get_symbol() == "("){
                ++count;
                
            }
            
            if(s[i].get_symbol() == ")"){
                --count;
            }

        }

        if(count != 0){
            return true;
        }
    }

    return false;

}

void preliminary(int k, int j, std::vector<tokens> &s){
    for(int i{k}; i <= j; ++i){
        if (s[i].get_symbol() == "-"){
            s[i].set_symbol("+");
            int negative{};
            if(s[i+1].get_symbol() == "("){
                for(int a{i+2}; in_paren(i, a, j, s) == true; ++a){
                    if(in_paren(i+2, a, j, s) == false && s[a].get_type() == "int"){
                        negative = -1*std::stof(s[a].get_symbol());
                        s[a].set_symbol(std::to_string(negative));

                    }
                }
            }
            else{
                negative = -1*std::stof(s[i+1].get_symbol());
                s[i+1].set_symbol(std::to_string(negative));
            }
            
            
        }

        if(s[i].get_symbol() == "/"){
            float divisor{};
            s[i].set_symbol("*");
            if(s[i+1].get_symbol() == "("){
                for(int a{i+2}; in_paren(i, a,j, s) == true; ++a){
                    if(in_paren(i+2, a,j, s) == false && s[a].get_type() == "int"){
                        divisor = 1/std::stof(s[a].get_symbol());
                        s[a].set_symbol(std::to_string(divisor));
                    }
                }
            }
            else{
                
                divisor = 1/(std::stof(s[i+1].get_symbol()));
                s[i+1].set_symbol(std::to_string(divisor));
            } 

        }
    }
}

void head_tail(int &a, int &b, std::vector<tokens> s){
    for(int i{a}; i < b; i++){
        if(s[i].get_type() == "op" || s[i].get_type() == "int" ){
            a = i;
            for(int k{a}; s[k].get_type() == "op" || s[k].get_type() == "int"; k++){
                b = k;
                
            }
            
            return;
        }
    }

}
float arithmetic(int k, int j, std::vector<tokens> s){
    
    
    
    bool flag = false;  

    for(int i{k}; i<=j; i++){

        if((s[i].get_symbol()== "+") && in_paren(k,i,j,s) == false){
            flag = true;
            break;
            
        }
    }

    int paren_count{0};

    for(int i{k}; i <= j; ++i){
        
        if(s[i].get_symbol() == "(" || s[i].get_symbol() == ")"){
            
            ++paren_count;
        }
    }
    
    for(int i{k}; i<=j; i++){
        
        

        if(k == j){
            float num = std::stof(s[k].get_symbol());
            return(num);

        }
        
        if(j-k == paren_count && s[k].get_symbol() == "("){
            float num = std::stof(s[j].get_symbol());
            return(num);

        }

        if(j-k == paren_count && s[j].get_symbol() == ")"){
            float num = std::stof(s[k].get_symbol());
            return(num);

        }

        if(in_paren(k,i,j,s) == false && (s[i].get_symbol() == "+")){
            
            return(arithmetic(k,i-1, s)+arithmetic(i+1,j, s));
        }
        // else if(in_paren == false && (s[i].get_symbol() == "-")){
        
        //     return(arithmetic(k,i-1, s) - arithmetic(i+1,j, s));
        // }
        

        else if(in_paren(k,i,j,s) == false && (s[i].get_symbol() == "*") && flag == false){
            
            
            return(arithmetic(k,i-1, s)*arithmetic(i+1,j, s));
        }

        else if(in_paren(k,i,j,s) == false && (s[i].get_symbol() == "%") && flag == false){
            
            int x = static_cast<int>(arithmetic(k,i-1, s));
            int y = static_cast<int>(arithmetic(i+1,j, s));
            return(x % y);
        }
        // else if(in_paren == false && (s[i].get_symbol() == "/") && flag == false){
            
        //     return(arithmetic(k,i-1, s)/arithmetic(i+1,j, s));
        // }


    }
}




bool comparison(int i, int start, int end, std::vector<tokens> s){
    
    if(s[i].get_symbol() == "!="){
        if((s[i-1].get_type() == "int" || s[i-1].get_type() == "op") && (s[i+1].get_type() == "int" || s[i+1].get_type() == "op")){
            
            head_tail(start,i,s);
            preliminary(start, i, s);
            float result1 = arithmetic(start,i,s);
            
            ++i;
            head_tail(i, end, s);
            preliminary(i, end,s);
            float result2 = arithmetic(i,end,s);
            
            
            if(result1 != result2){
                        
                return true;
            }
            
            else{
    
                return false;
            }
            

        }
    }
    if(s[i].get_symbol() == "=="){
        if((s[i-1].get_type() == "int" || s[i-1].get_type() == "op") && (s[i+1].get_type() == "int" || s[i+1].get_type() == "op")){
            
            head_tail(start,i,s);
            preliminary(start, i, s);
            float result1 = arithmetic(start,i,s);
            
            ++i;
            head_tail(i, end, s);
            preliminary(i, end,s);
            float result2 = arithmetic(i,end,s);
            
            
            if(result1 == result2){
                        
                return true;
            }
            
            else{
    
                return false;
            }
            

        }
    }

    if(s[i].get_symbol() == ">="){
        if((s[i-1].get_type() == "int" || s[i-1].get_type() == "op") && (s[i+1].get_type() == "int" || s[i+1].get_type() == "op")){
            
            head_tail(start,i,s);
            preliminary(start, i, s);
            float result1 = arithmetic(start,i,s);
            
            ++i;
            head_tail(i, end, s);
            preliminary(i, end,s);
            float result2 = arithmetic(i,end,s);
            
            
            if(result1 >= result2){
                        
                return true;
            }
            
            else{
    
                return false;
            }
            

        }
    }

    if(s[i].get_symbol() == "<="){
        if((s[i-1].get_type() == "int" || s[i-1].get_type() == "op") && (s[i+1].get_type() == "int" || s[i+1].get_type() == "op")){
            
            head_tail(start,i,s);
            preliminary(start, i, s);
            float result1 = arithmetic(start,i,s);
            
            ++i;
            head_tail(i, end, s);
            preliminary(i, end,s);
            float result2 = arithmetic(i,end,s);
            
            
            if(result1 <= result2){
                        
                return true;
            }
            
            else{
    
                return false;
            }
            

        }
    }

    if(s[i].get_symbol() == "<"){
        if((s[i-1].get_type() == "int" || s[i-1].get_type() == "op") && (s[i+1].get_type() == "int" || s[i+1].get_type() == "op")){
            
            head_tail(start,i,s);
            preliminary(start, i, s);
            float result1 = arithmetic(start,i,s);
            
            ++i;
            head_tail(i, end, s);
            preliminary(i, end,s);
            float result2 = arithmetic(i,end,s);
            
            
            if(result1 < result2){
                        
                return true;
            }
            
            else{
    
                return false;
            }
            

        }
    }

    if(s[i].get_symbol() == ">"){
        if((s[i-1].get_type() == "int" || s[i-1].get_type() == "op") && (s[i+1].get_type() == "int" || s[i+1].get_type() == "op")){
            
            head_tail(start,i,s);
            preliminary(start, i, s);
            float result1 = arithmetic(start,i,s);
            
            ++i;
            head_tail(i, end, s);
            preliminary(i, end,s);
            float result2 = arithmetic(i,end,s);
            
            
            if(result1 > result2){
                        
                return true;
            }
            
            else{
    
                return false;
            }
            

        }
    }
}

void assignment(int i, int start, int end, std::vector<tokens> s, std::vector<tokens> &v){
    for(int k{0}; k<v.size(); k++){
        
        if(s[i-1].get_symbol() == v[k].get_type()){
            
            head_tail(i,end,s);
            preliminary(i, end, s);
            float assigned = arithmetic(i,end,s);
            v[k].set_symbol(std::to_string(assigned));
            
            return;
        }
    }
    tokens entry(s[i+1].get_symbol(), s[i-1].get_symbol());
    v.push_back(entry);
    return;


}