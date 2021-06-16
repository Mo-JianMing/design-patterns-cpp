#include <iostream>
using std::cout;
using std::endl;
using std::string;

//抽象产品 为构成系列产品的一组不同但相关的产品声明接口
class AbstractProductA {
public:
    virtual ~AbstractProductA() {};
    virtual string UsefulFunctionA() const = 0;
};

//具体产品 是抽象产品的多种不同类型实现
//所有变体 （维多利亚/现代） 都必须有相应的实现产品 （椅子/沙发）
class ConcreteProductA1 : public AbstractProductA {
public:
    string UsefulFunctionA() const override {
        return "具体产品—>  A1";
    }
};

class ConcreteProductA2 : public AbstractProductA {
    string UsefulFunctionA() const override {
        return "具体产品—>  A2";
    }
};

class AbstractProductB {
public:
    virtual ~AbstractProductB() {};
    virtual string UsefulFunctionB() const = 0;

    //接受A类产品作为合作者
    virtual string AnotherUsefulFunctionB(const AbstractProductA& collaborator) const = 0;
};

class ConcreteProductB1 : public AbstractProductB {
public:
    string UsefulFunctionB() const override {
        return "具体产品—>  B1";
    }
    //合作的具体实现
    string AnotherUsefulFunctionB(const AbstractProductA& collaborator) const override {
        const string result = collaborator.UsefulFunctionA();
        return "( " + result + " ) 与 B1 合作的结果 ";
    }
};

class ConcreteProductB2 : public AbstractProductB {
public:
    string UsefulFunctionB() const override {
        return "具体产品—>  B2";
    }
    string AnotherUsefulFunctionB(const AbstractProductA& collaborator) const override {
        const string result = collaborator.UsefulFunctionA();
        return "( " + result + " ) 与 B2 合作的结果 ";
    }
};

//抽象工厂 接口声明了一组创建各种抽象产品的方法
class AbstractFactory {
public:
    virtual AbstractProductA* CreateProductA() const = 0;
    virtual AbstractProductB* CreateProductB() const = 0;
};

//具体工厂 实现抽象工厂的构建方法。 每个具体工厂都对应特定产品变体， 且仅创建此种产品变体
class ConcreteFactory1 : public AbstractFactory {
public:
    AbstractProductA* CreateProductA() const override {
        return new ConcreteProductA1();
    }
    AbstractProductB* CreateProductB() const override {
        return new ConcreteProductB1();
    }
};

class ConcreteFactory2 : public AbstractFactory {
public:
    AbstractProductA* CreateProductA() const override {
        return new ConcreteProductA2();
    }
    AbstractProductB* CreateProductB() const override {
        return new ConcreteProductB2();
    }
};

void ClientCode(const AbstractFactory& factory) {
    const AbstractProductA* product_a = factory.CreateProductA();
    const AbstractProductB* product_b = factory.CreateProductB();
    cout << product_b->UsefulFunctionB() << "\n";
    cout << product_b->AnotherUsefulFunctionB(*product_a) << "\n";
    delete product_a;
    delete product_b;
}

int main() {
    cout << "客户端：使用第一个工厂类型测试客户端代码：\n";
    ConcreteFactory1* f1 = new ConcreteFactory1();
    ClientCode(*f1);
    delete f1;
    cout << endl;
    cout << "客户端：使用第二个工厂类型测试客户端代码：\n";
    ConcreteFactory2* f2 = new ConcreteFactory2();
    ClientCode(*f2);
    delete f2;
    return 0;
}