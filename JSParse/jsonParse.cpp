


#include "jsonParse.h"
#include <iostream>

/*
 * Constructor for Token class.
 */
Token::Token(std::string lex, RegexType t_type){
    lexeme = lex;
    type = t_type;
    
}


/*
 * Returns the lexeme of a given token.
 */
std::string Token::getLexeme() const{
    return lexeme;
}

/*
 * Returns the type of the token.
 */
RegexType Token::getType() const{
    return type;
}


/*
 * Constructor for instantiating a RegexPattern. It needs an formatted regex 
 * string that describes a ECMAScript regular expression, and an enum RegexType that 
 * is defined the header file, describing what type of Token the RegexString is representing. 
 */
RegexPattern::RegexPattern(std::string regexp, RegexType t_type){
    regexPattern = regexp;
    type = t_type;
}

std::string RegexPattern::getRegex() const{
    return regexPattern;
}

RegexType RegexPattern::getType() const{
    return type;
}

/*
 This takes in part of the input given to the parser and returns a number indicating what
 index the largest match ends at.
 */
std::tuple<std::size_t, RegexType> RegexPattern::longestMatch(std::string input){
    std::regex patternRegex (getRegex(), std::regex_constants::ECMAScript);
    
    std::cout << "The string we are trying to match is " + input << std::endl;
    std::regex_iterator<std::string::iterator> iteratorBegin (input.begin(), input.end(), patternRegex);
    
    std::regex_iterator<std::string::iterator> iteratorEnd;
    
    std::vector<std::string> matches;
    
    while (iteratorBegin != iteratorEnd) {
        
        std::string candidate = iteratorBegin->str();
        
        std::cout << "The regex match is : " << candidate << std::endl;
        
        std::size_t length = candidate.length();
        
        std::string part_of_input = input.substr(0, length);
        
        std::cout << "The two strings we are trying to determine are equal: " << candidate << " and " << part_of_input << std::endl;
        
        if(candidate == part_of_input){
            matches.push_back(candidate);
        }
        
        iteratorBegin++;
    }
    
    std::size_t max = 0;
    for(std::vector<std::string>::iterator it = matches.begin(); it != matches.end(); ++it){
        std::size_t potentialMax = it->length();
        if(potentialMax > max){
            max = potentialMax;
        }
    }
    return std::tuple<std::size_t, RegexType> {max, getType()};
    
}


/*
 * Takes a string that represents the potential JSON you wish to parse and performs the
 * first part of the front end analysis: scanning. That is, this function asserts that
 * the microsyntax of the JSON specification is satsifed in the input. Returns an empty
 * vector if the lexer finds an error.
 */
std::vector<Token> lex(std::string input){
    
    std::vector<RegexPattern> regexes{
        RegexPattern{"\"([^\"\\\\[:cntrl:]]|\\\\([\"\\\\/bfnrt]|u[0123456789abcdefABCDEF]{4}))*\"", RegexType::STRING},
        RegexPattern{"(-)?(0|[1-9][0-9]*)(\\.[0-9]+)?([eE][+-]?[0-9]+)?", RegexType::NUMBER},
        RegexPattern{"true", RegexType::TRUETYPE},
        RegexPattern{"false", RegexType::FALSETYPE},
        RegexPattern{"null", RegexType::NULLTYPE},
        RegexPattern{"\\{", RegexType::OPENCURLY},
        RegexPattern{"\\}", RegexType::CLOSECURLY},
        RegexPattern{"\\{", RegexType::OPENCURLY},
        RegexPattern{",", RegexType::COMMA},
        RegexPattern{"\\[", RegexType::OPENSQUARE},
        RegexPattern{"\\]", RegexType::CLOSESQUARE},
        RegexPattern{":", RegexType::COLON},
        RegexPattern{"[\\s]+", RegexType::WHITESPACE}
        
    };
    
    std::vector<Token> lexResults;
    
    std::string tokenless_input = input;
    
    std::vector<std::tuple<std::size_t, RegexType>> longestMatchForEach;
    
    longestMatchForEach.resize(regexes.size());
    
    
    
    while (!tokenless_input.empty()){
        std::cout << tokenless_input << std::endl;
        std::transform(
                       regexes.begin(),
                       regexes.end(),
                       longestMatchForEach.begin(),
                       [tokenless_input](RegexPattern p){
                           std::cout << "The tokenless input we are trying to match is " << tokenless_input << std::endl;
                           std::cout << "The regex we are trying to match is " << p.getRegex() << std::endl;
                           return p.longestMatch(tokenless_input);
                       });
        
        std::cout << "Found potential longest matches for tokenless inputs" << std::endl;
        
        std::tuple<size_t, RegexType> maxElement {0, RegexType::FAIL};
        
        //attempts to find the maximum regex max. if it finds a non fail token, it
        //finds the index to consume the input by.
        for (std::vector<std::tuple<std::size_t, RegexType>>::iterator it = longestMatchForEach.begin();
             it != longestMatchForEach.end(); ++it) {
            std::tuple<std::size_t, RegexType> result = *it;
            if (std::get<0>(result) > std::get<0>(maxElement)) {
                maxElement = result;
            }
            
        }
        
        if(std::get<0>(maxElement) == 0){
            return std::vector<Token>();
        }
        
        std::size_t consumeIndex = std::get<0>(maxElement);
        
        std::cout << "The longest regex match that was found was " + std::to_string(consumeIndex) << std::endl;
        
        std::string stringInToken = tokenless_input.substr(0, consumeIndex);
        
        std::cout << "The new token is " + stringInToken << std::endl;
        
        Token newToken {stringInToken, std::get<1>(maxElement)};
        
        lexResults.push_back(newToken);
        
        //reduce the input by the
        tokenless_input = tokenless_input.substr(consumeIndex, tokenless_input.size() - consumeIndex);
        
        std::cout << "The token less input is now " + tokenless_input << std::endl;
        
        
    }
    
    return lexResults;
    
}




int main(){
    
    std::string s ("{\"hi\":\"hello\"}");
    
    
    
    std::cout << "Testing the input: " + s << std::endl;

    
    std::vector<Token> lexResults = lex(s);
    
    std::cout << "The number of tokens is " + std::to_string(lexResults.size()) << std::endl;
    
    return 0;
}
