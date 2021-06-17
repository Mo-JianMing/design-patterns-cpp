#include <iostream>
using std::cout;
using std::endl;
using std::string;

//实现部分 为所有具体实现声明通用接口， 抽象部分仅能通过在这里声明的方法与实现对象交互
//抽象部分可以列出和实现部分一样的方法， 但是抽象部分通常声明一些复杂行为， 这些行为依赖于多种由实现部分声明的原语操作
class Implementation {
public:
    virtual ~Implementation() {}
    virtual string OperationImplementation() const = 0;
};

//具体实现 中包括特定于平台的代码
class ConcreteImplementationA : public Implementation {
public:
    string OperationImplementation() const override {
        return "具体实现A: 这是平台 A 上的结果 \n";
    }
};

class ConcreteImplementationB : public Implementation {
public:
    string OperationImplementation() const override {
        return "具体实现B: 这是平台 B 上的结果 \n";
    }
};

//抽象部分 提供高层控制逻辑， 依赖于完成底层实际工作的实现对象
class Abstraction {
protected:
    Implementation* implementation_;

public:
    Abstraction(Implementation* implementation) : implementation_(implementation) {
    }

    virtual ~Abstraction() {
    }

    virtual string Operation() const {
        return "抽象：基本操作：\n" +
            this->implementation_->OperationImplementation();
    }
};

//精确抽象 提供控制逻辑的变体。 与其父类一样， 它们通过通用实现接口与不同的实现进行交互
class ExtendedAbstraction : public Abstraction {
public:
    ExtendedAbstraction(Implementation* implementation) : Abstraction(implementation) {
    }
    string Operation() const override {
        return "具体抽象：扩展操作：\n" +
            this->implementation_->OperationImplementation();
    }
};

void ClientCode(const Abstraction& abstraction) {
    cout << abstraction.Operation();
}

int main() {
    Implementation* implementation = new ConcreteImplementationA;
    Abstraction* abstraction = new Abstraction(implementation);
    ClientCode(*abstraction);
    cout << endl;
    delete implementation;
    delete abstraction;

    implementation = new ConcreteImplementationB;
    abstraction = new ExtendedAbstraction(implementation);
    ClientCode(*abstraction);

    delete implementation;
    delete abstraction;
    return 0;
}