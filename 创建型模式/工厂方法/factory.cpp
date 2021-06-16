#include <iostream>
using std::cout;
using std::endl;
using std::string;

//产品 将会对接口进行声明。 对于所有由创建者及其子类构建的对象， 这些接口都是通用的
class Product {
public:
    virtual ~Product() {}
    virtual string Operation() const = 0;
};

//具体产品 是产品接口的不同实现
class ConcreteProduct1 : public Product {
public:
    string Operation() const override {
        return "产品—> ConcreteProduct1";
    }
};
class ConcreteProduct2 : public Product {
public:
    string Operation() const override {
        return "产品—> ConcreteProduct2";
    }
};

//创建者 类声明返回产品对象的工厂方法。 该方法的返回对象类型必须与产品接口相匹配
class Creator {
public:
    virtual ~Creator() {};
    virtual Product* FactoryMethod() const = 0;

    string SomeOperation() const {
        Product* product = this->FactoryMethod();
        string result = "创建者：通过工厂方法获得对应产品对象 " + product->Operation();
        delete product;
        return result;
    }
};

//具体创建者 将会重写基础工厂方法， 使其返回不同类型的产品
//注意：并不一定每次调用工厂方法都会创建新的实例。 工厂方法也可以返回缓存、 对象池或其他来源的已有对象
class ConcreteCreator1 : public Creator {
public:
    //具体产品使用结束，需要delete
    Product* FactoryMethod() const override {
        return new ConcreteProduct1();
    }
};

class ConcreteCreator2 : public Creator {
public:
    Product* FactoryMethod() const override {
        return new ConcreteProduct2();
    }
};

void ClientCode(const Creator& creator) {
    cout << "客户端：不知道创建者的类型，但它仍然有效\n"
        << creator.SomeOperation() << endl;
}

int main() {
    cout << "App: 开启工厂 ConcreteCreator1.\n";
    Creator* creator = new ConcreteCreator1();
    ClientCode(*creator);
    cout << endl;
    cout << "App: 开启工厂 ConcreteCreator2.\n";
    Creator* creator2 = new ConcreteCreator2();
    ClientCode(*creator2);
    cout << endl;

    delete creator;
    delete creator2;
    return 0;
}