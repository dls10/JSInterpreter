/*
 *  jsonParse.h
 *  jsonParse
 *
 *  Contains the declarations of all of the objects we will use for our JSON parser.
 *
 */

#include <regex>
#include <string>
#include <tuple>

//This is an enum class to help make the code more readable.
//Listed below are all of the types of tokens that the
//JSON spec asks us to account for.
enum class RegexType{
    
    STRING, //a JSON string described at JSON.org.
    OPENCURLY, //an open curly brace character ({)
    CLOSECURLY, //a closed curly brace character (})
    NULLTYPE, //the null value in JS (null)
    NUMBER, //a valid Javascript number
    COMMA, //a comma character (,)
    WHITESPACE, //any whitespace character, space, tab, newline, etc.
    COLON, // a colon character (:)
    OPENSQUARE, //an open square bracket [
    CLOSESQUARE, //a close square bracket ]
    FAIL, // a special token for strings that do not conform to any of the other tokens
    TRUETYPE, //a true value in Javascript (true)
    FALSETYPE //a false value in Javascript (true)
    
    
};


/*
 * This will provide the class for each Token that we wish to parse into an internal representation.
 * To be initialized it needs two things, the lexeme, which represents the actual data
 * that each token has, and a type of Regex that indicates what type of token it is.
 *
 */
class Token{
public:
    Token(std::string lex, RegexType t_type);
    std::string getLexeme() const;
    RegexType getType() const;
private:
    //a string representing part of the input that has been classified as a given token type.
    std::string lexeme;
    //an enum that associates a RegexType with the lexeme
    RegexType type;
};



/*
 * This class associates a compiled regular expression with a named constant that indicates what type
 * of token the regex is parsing.
 */
class RegexPattern {
public:
    RegexPattern (std::string regexp, RegexType t_type);
    std::tuple<std::size_t, RegexType> longestMatch(std::string input);
    RegexType getType() const;
    std::string getRegex() const;
private:
    //an unescaped ECMAscript regex expression tha
    std::string regexPattern;
    //the enum associated with the given regular expresssion
    RegexType type;
};

//TODO Need to have a better return strategy when the lexing fails. Perhaps std::optional?
//
//This function takes in a input string and attempts to tokenize it
//based on the rules given at JSON.org. If any of the regular
//expression matches fails, than the function returns an empty string.
//Otherwise it returns the tokenized input that will be used for parsing.
//
std::vector<Token> lex(std::string input);





