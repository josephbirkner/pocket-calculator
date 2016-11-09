#include <iostream>
#include <string>

using namespace std;

struct expected_char_exception {char expected;};
struct unexpected_char_exception {char expected;};
struct expected_end_exception {int pos;};

/**
 * Recursive descent parser boilerplate functions
 */
class parser
{
protected:
    string const word;
    int pos = 0;
    bool skip_whitespace = true;
    
public:
    parser(string const& word) : word(word) {
    }
    
    void advance(int& pos_) const {
        while( pos_ < word.size() && ++pos_ && (!skip_whitespace || isspace(word[pos_])) );
    }
    
    bool maybe_expect(char const& expected) {
        if(pos >= word.size() - 1)
            return false;
            
        if(word[pos] == expected) {
            advance(pos);
            return true;
        }
        else
            return false;
    }
    
    void expect(char const& expected) {
        if(pos >= word.size() - 1)
            throw expected_char_exception{expected};
            
        if(word[pos] == expected)
            advance(pos);
        else
            throw expected_char_exception{expected};
    }
    
    void expect_end() {
        if(!at_end())
            throw expected_end_exception{pos};
    }
    
    inline char const& next() const {
        int next_pos = pos;
        advance(next_pos);
        return word[next_pos];
    }
    
    inline char const& current() const {
        return word[pos];
    }
    
    inline bool at_end() const {return at_end(pos);}

    bool at_end(int pos) const {return pos >= word.size() - 1 || (skip_whitespace && isspace(word[pos]) && at_end(pos+1));}
};

/**
 * Expression evaluator that parses a string and evaluates
 * it as a mathematical expression according to this grammar:
 * - 
 */
class expression_evaluator : public parser
{
public:
    expression_evaluator(string const& s) : parser(s) {
        if(at_end())
            return;
        parse_sum(result);
        expect_end();
    }
    
    double result = .0;

    operator double() {return result;}
    
private:
    void parse_sum(double& res)
    {
        double product = .0;
        parse_product(product);
        res = product;
        while(!at_end())
        {
            if(maybe_expect('+')) {
                parse_product(product);
                res += product;
            }
            else if(maybe_expect('-')) {
                parse_product(product);
                res -= product;
            }
            else
                return;
        }
    }
    
    void parse_product(double& res)
    {
        double term = .0;
        parse_term(term);
        res = term;
        while(!at_end())
        {
            if(maybe_expect('*')) {
                parse_term(term);
                res *= term;
            }
            else if(maybe_expect('/')) {
                parse_term(term);
                res /= term;
            }
            else
                return;
        }
    }
    
    void parse_term(double& res)
    {
        if(current() == '-' || current() == '+')
        {
            bool negative = maybe_expect('-');
            if(!negative)
                maybe_expect('+');
            
            parse_term(res);
            if(negative)
                res *= -1;
        }
        else if(isdigit(current()))
        {
            string number;
            while(isdigit(current())) {
                number += current();
                advance(pos);
            }
            res = static_cast<double>(stoi(number));
        }
        else if(maybe_expect('(')) {
            parse_sum(res);
            expect(')');
        }
        else
            throw unexpected_char_exception{current()};
    }
};

int main()
{
    cout << (double) expression_evaluator("-(1+1+1)--1") << endl;
}
