#include <iostream>
#include <stack>
#include <string>
#include <list>
#include <map>

using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::to_string;
using std::stack;
using std::list;
using std::map;
using std::pair;




enum class TOKEN_TYPE {
    number,
    operator_,
    function,
    variable,
    unaryMinus,
    postfixFunction,
    leftParenthesis,
    rightParenthesis
};

struct Request {
    string &str;
    size_t pos;

    Request(const string &str, size_t pos) : str(const_cast<string &>(str)), pos(pos) {};
};

struct Responce {
    string token;
    size_t pos;
    TOKEN_TYPE type;
};

class Handler {
public:
    virtual Handler *SetNext(Handler *handler) = 0;

    virtual Responce Handle(Request request) = 0;
};

class AbstractHandler : public Handler {

private:
    Handler *next_handler_;

public:
    AbstractHandler() : next_handler_(nullptr) {
    }

    Handler *SetNext(Handler *handler) override {
        this->next_handler_ = handler;

        return handler;
    }

    Responce Handle(Request request) override {
        if (this->next_handler_) {
            return this->next_handler_->Handle(request);
        } else {
            cerr << "next handler isn't defined!";
            exit(0);
        }
    }
};

class NumberHandler : public AbstractHandler {
public:
    Responce Handle(Request req) override {
        if (isdigit(req.str[req.pos])) {
            string token;
            long long int number = 0;
            size_t i;
            for (i = req.pos; i < req.str.size() && isdigit(req.str[i]); ++i) {
                number *= 10;
                number += req.str[i] - '0';
            }
            if (req.str[i] == '.' || req.str[i] == ',') {
                token += to_string(number) + req.str[i];
                number = 0;
                for (++i; i < req.str.size() && isdigit(req.str[i]); ++i) {
                    number *= 10;
                    number += req.str[i] - '0';
                }
            }

            token += to_string(number);

            return {token, i, TOKEN_TYPE::number};
        } else {
            return AbstractHandler::Handle(req);
        }

    }
};

class FunctionHandler : public AbstractHandler {
public:
    Responce Handle(Request req) override {
        if (isalnum(req.str[req.pos])) {
            string token1;
            size_t i;
            for (i = req.pos; i < req.str.size() && isalnum(req.str[i]); i++) {
                token1 += req.str[i];
            }
            if (req.str[i] == '(') {
                ++i;
                list<string> variables;

                string tmp;
                for (; req.str[i] != ')'; ++i) {
                    if (req.str[i] == ',') {
                        variables.push_back(tmp);
                        tmp.clear();
                    } else if (req.str[i] != ' ') {
                        tmp += req.str[i];
                    }
                }

                string token = "| ";

                for (const auto &val : variables) {
                    token += val + " ";
                }
                token += tmp + " $"; // last param leaves in tmp

                token += token1;

                return {token, i + 1, TOKEN_TYPE::function};

            } else {
                return AbstractHandler::Handle(req);
            }


        } else {
            return AbstractHandler::Handle(req);
        }
    }
};

class VariableHandler : public AbstractHandler {
public:
    Responce Handle(Request req) override {
        if (isalnum(req.str[req.pos])) {
            string token;
            size_t i;
            for (i = req.pos; i < req.str.size() && isalnum(req.str[i]); ++i) {
                token += req.str[i];
            }

            return {token, i, TOKEN_TYPE::variable};
        } else {
            return AbstractHandler::Handle(req);
        }

    }
};

class UnaryMinusHandler : public AbstractHandler {
public:
    Responce Handle(Request req) override {
        if (req.str[req.pos] == '-' && (!req.pos || req.str[req.pos - 1] == '(')) {
            return {"#", req.pos + 1, TOKEN_TYPE::unaryMinus};
        } else {
            return AbstractHandler::Handle(req);
        }
    }
};

class OperatorHandler : public AbstractHandler {
public:
    Responce Handle(Request req) override {
        if (req.str[req.pos] == '+' || req.str[req.pos] == '-' || req.str[req.pos] == '*' || req.str[req.pos] == '/' ||
            req.str[req.pos] == '^' || req.str[req.pos] == '%') {
            string token(1, req.str[req.pos]);

            return {token, req.pos + 1, TOKEN_TYPE::operator_};
        } else {
            return AbstractHandler::Handle(req);
        }
    }
};

class PostfixFunctionHandler : public AbstractHandler {
public:
    Responce Handle(Request req) override {
        if (req.str[req.pos] == '!') {
            return {"!", req.pos + 1, TOKEN_TYPE::postfixFunction};
        } else {
            return AbstractHandler::Handle(req);
        }
    }
};

class LeftParenthesisHandler : public AbstractHandler {
public:
    Responce Handle(Request req) override {
        if (req.str[req.pos] == '(') {
            return {"(", req.pos + 1, TOKEN_TYPE::leftParenthesis};
        } else {
            return AbstractHandler::Handle(req);
        }
    }
};

class RightParenthesisHandler : public AbstractHandler {
public:
    Responce Handle(Request req) override {
        if (req.str[req.pos] == ')') {
            return {")", req.pos + 1, TOKEN_TYPE::rightParenthesis};
        } else {
            return AbstractHandler::Handle(req);
        }
    }
};

class ErrorHandler : public AbstractHandler {
public:
    Responce Handle(Request req) override {
        cerr << "Wrong statement!";
        exit(0);
    }
};

string to_rpn(const string &str) {
    map<string, short> priority;
    priority["+"] = 0;
    priority["-"] = 0;
    priority["*"] = 1;
    priority["/"] = 1;
    priority["^"] = 2;

    NumberHandler nh;
    FunctionHandler fh;
    VariableHandler vh;
    UnaryMinusHandler umh;
    OperatorHandler oh;
    PostfixFunctionHandler pfh;
    LeftParenthesisHandler lph;
    RightParenthesisHandler rph;
    ErrorHandler eh;

    nh.SetNext(&fh);
    fh.SetNext(&vh);
    vh.SetNext(&umh);
    umh.SetNext(&oh);
    oh.SetNext(&pfh);
    pfh.SetNext(&lph);
    lph.SetNext(&rph);
    rph.SetNext(&eh);

    string result;
    stack<pair<string, TOKEN_TYPE>> stack1;
    Responce res;
    Request req(str, 0);
    while (req.pos < str.size()) {
        res = nh.Handle(req);

        int k;
        for (k = 0; str[k + res.pos] == ' '; ++k); // skip spaces
        req.pos = res.pos = k + res.pos;

        switch (res.type) {

            case TOKEN_TYPE::number:
                result += res.token + " ";
                break;
            case TOKEN_TYPE::operator_:
                while (!stack1.empty() && (stack1.top().second == TOKEN_TYPE::function ||
                                           (priority.contains(stack1.top().first) &&
                                            priority[stack1.top().first] >= priority[res.token]))) {
                    result += stack1.top().first + " ";
                    stack1.pop();
                }
                stack1.push({res.token, res.type});
                break;
            case TOKEN_TYPE::function ... TOKEN_TYPE::variable:
                result += res.token + " ";
                break;
//            case TOKEN_TYPE::variable:
//                result += res.token + " ";
//                break;
            case TOKEN_TYPE::unaryMinus:
                result += "-";
                break;
            case TOKEN_TYPE::postfixFunction:
                result += res.token + " ";
                break;
            case TOKEN_TYPE::leftParenthesis:
                stack1.push({res.token, res.type});
                break;
            case TOKEN_TYPE::rightParenthesis:
                while (stack1.top().first != "(") {
                    result += stack1.top().first + " ";
                    stack1.pop();
                }
                stack1.pop();
                break;
        }
    }

    while (!stack1.empty()) {
        result += stack1.top().first + " ";
        stack1.pop();
    }

    result.pop_back();

    return result;
}


int main() {
    Responce rsp;
    NumberHandler nh;
    FunctionHandler fh;
    VariableHandler vh;
    UnaryMinusHandler umh;
    OperatorHandler oh;
    PostfixFunctionHandler pfh;

    nh.SetNext(&fh);
    fh.SetNext(&vh);
    vh.SetNext(&umh);
    umh.SetNext(&oh);
    oh.SetNext(&pfh);

    string str = "-31+4* 2 / (-1-5!)^2 + sin(x)";


    cout << to_rpn(str);

    return 0;
}
