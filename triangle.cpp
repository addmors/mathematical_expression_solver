#include <iostream>
#include<memory>
#include <string>
#include<stack>
#include <vector>
using namespace std;


enum class TokenType {
	VALUE,
	RIGHT_BRACES,
	LEFT_BRACES,
	SIGN_PLUS,
	SIGN_MINUS, 
	SIGN_MULT,
	SING_DIV
};

struct Token{
  string value;
  TokenType type;
};

vector<Token> Tokenize(istream& cl) {
	//TODO;
}
struct Node { virtual float Evaluate() const = 0;};

struct Value : public Node{
public:
	Value(float d) :d_(d) {};
	float  Evaluate() const override {return d_;}
private:
	const float d_;
};

class Operation : public Node {
public:
	Operation(char value)
		:precedence([value] {
		return (value == '*' || value == '/') ? 2 : 1; }()),
		op_(value) {}
		const uint8_t precedence;
		float Evaluate() const override {
			if (op_ == '*')  return left_->Evaluate()*right_->Evaluate();
			else if (op_ == '+')  return left_->Evaluate()+right_->Evaluate();
			else if (op_ == '-')  return left_->Evaluate() - right_->Evaluate();
			else if (op_ == '/') return left_->Evaluate() / right_->Evaluate();
			return 0;}
		void SetLeft(shared_ptr<Node> node) { left_ = node; }
		void SetRight(shared_ptr<Node> node) { right_ = node; }
private:
	const char op_;
	shared_ptr<Node> left_, right_;
};


template <class Iterator>
shared_ptr<Node> Parse(Iterator& token, Iterator end) {
	if (token == end) return make_shared<Value>('0');
	stack<shared_ptr<Node>> values;
	stack<shared_ptr<Operation>> ops;

	auto PopOps = [&](int precedence) {
		while (!ops.empty() && ops.top()->precedence >= precedence) {
			auto value1 = values.top();
			values.pop();
			auto value2 = values.top();
			values.pop();
			auto op = ops.top();
			ops.pop();
			op->SetRight(value1);
			op->SetLeft(value2);
			values.push(op);
		}
	};
	while (token != end && *token != ')') {
		const auto &value = *token;
		if (value >= '0' && value <= '9') {
			string::size_type sz;
			string temp({token,end});
			values.push(make_shared<Value>(
				stof(temp,&sz) ));
			token += sz-1;
		}
		else if (value == '(') values.push(Parse(++token, end));
		else if (value == '*' || value == '/') {
			PopOps(2);
			ops.push(make_shared<Operation>(value));
		}
		else if (value == '+' || value == '-') {
			PopOps(1);
			ops.push(make_shared<Operation>(value));
		}
		++token;
	}
	while (!ops.empty())  PopOps(0);
	return values.top();
};

int main() {
	string tokens;
	cout << "Enter expression: ";
	getline(cin, tokens);
	float x = 0;
	auto token = tokens.begin();
	auto node = Parse(token, tokens.end());
	cout << "Expression value: " << node->Evaluate() << endl;
	return 0;
}