#include <iostream>
#include <string>

using namespace std;

struct expected_char_exception {char expected;};
struct unexpected_char_exception {char unexpected;};
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
        if(word[pos] != expected)
            return false;

        if(!at_end())
            advance(pos);

        return true;
    }
    
    void expect(char const& expected) {
        if(word[pos] != expected)
            throw expected_char_exception{expected};

        if(!at_end())
            advance(pos);
    }
    
    void expect_end() {
        if(!at_end())
            throw expected_end_exception{pos};
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
 *  expression := sum
 *  sum        := product {(+|-) product}
 *  product    := term {(*|/) term}
 *  term       := (+|-) term | {[0-9]} | \(sum\)
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
    cout << "You're the operator with the pocket calculator! Type q to quit." << endl;

    for(;;) {
        cout << "\n> ";
        string expression;
        getline(cin, expression);
        if(expression == "q")
            break;

        try {
            cout << (double) expression_evaluator(expression) << endl;
        }
        catch(expected_char_exception const& e) {
            cout << "error: expected '" << e.expected << "'" << endl;
        }
        catch(unexpected_char_exception const& e) {
            cout << "error: unexpected '" << e.unexpected << "'" << endl;
        }
        catch(expected_end_exception const& e) {
            cout << "error: expected end of string" << endl;
        }

    }
}
