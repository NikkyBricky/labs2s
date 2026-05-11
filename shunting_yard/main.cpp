#include <iostream>
#include <string>
#include <cctype>
#include "../stack/Stack.h"

std::string sorting_station(const std::string exp){
    Stack st;
    std::string sorted_exp;

    std::string ops = "+-*/";

    for (auto it = exp.begin(); it != exp.end(); ++it){
	auto token = *it;

	auto next = it;
	++next;
	auto next_token = *next;

        if (isdigit(token) || token == '-' && isdigit(next_token)){
	    sorted_exp += token;

	    if (token == '0' && isdigit(next_token)){
	        return "wrong number(0 at the beginning)";
	    }

	    if (!isdigit(next_token)){
	        sorted_exp += " ";
	    }
	}

	else if (ops.find(token) != std::string::npos){
	    while (!st.isEmpty() && (token == '+' || token == '-') && (ops.find(st.top()) != std::string::npos)){
	        sorted_exp += st.top();
	        sorted_exp += " ";
		st.pop();
		
	    }
	    st.push(token);	    
	}

	else if (token == '('){
	    st.push(token);
	}

	else if (token == ')'){
	    while (!st.isEmpty() && st.top() != '('){
	        sorted_exp += st.top();
	        sorted_exp += " ";
		st.pop();
	    }

	    if (st.isEmpty()){
	        return "missed opening parenthesis";
	    }
	    st.pop();
	}
    }

    while (!st.isEmpty()){
        if (st.top() == '('){
	    return "missed closing paranthesis";
	}
	sorted_exp += st.top();
	sorted_exp += " ";
	st.pop();
    }

    return sorted_exp;
}

int main(){
    std::cout << "Введите выражение:" << std::endl;
    std::string exp;
    getline(std::cin, exp);

    std::cout << sorting_station(exp) << std::endl;
}

