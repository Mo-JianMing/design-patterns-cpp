#include <iostream>
using std::cout;
using std::endl;
using std::string;

//部件 声明封装器和被封装对象的公用接口
class Component {
public:
    virtual ~Component() {}
    virtual string Operation() const = 0;
};

//具体部件 类是被封装对象所属的类
//它定义了基础行为， 但装饰类可以改变这些行为
class ConcreteComponent : public Component {
public:
    string Operation() const override {
        return "具体组件";
    }
};

//基础装饰 类拥有一个指向被封装对象的引用成员变量
//该变量的类型应当被声明为通用部件接口， 这样它就可以引用具体的部件和装饰
class Decorator : public Component {
protected:
    Component* component_;

public:
    Decorator(Component* component) : component_(component) {
    }

    //装饰基类会将所有操作委派给被封装的对象
    string Operation() const override {
        return this->component_->Operation();
    }
};

//具体装饰类 定义了可动态添加到部件的额外行为
//具体装饰类会重写装饰基类的方法， 并在调用父类方法之前或之后进行额外的行为
class ConcreteDecoratorA : public Decorator {
public:
    ConcreteDecoratorA(Component* component) : Decorator(component) {
    }

    string Operation() const override {
        return "具体装饰器A (" + Decorator::Operation() + ")";
    }
};

class ConcreteDecoratorB : public Decorator {
public:
    ConcreteDecoratorB(Component* component) : Decorator(component) {
    }

    string Operation() const override {
        return "具体装饰器B (" + Decorator::Operation() + ")";
    }
};

void aClientCode(Component* component) {
    cout << "结果: " << component->Operation();
}

int main() {
    Component* simple = new ConcreteComponent;
    cout << "客户端：我有一个简单的组件：\n";
    aClientCode(simple);
    cout << "\n\n";

    //创建装饰并将其组合成客户端所需的形式
    Component* decorator1 = new ConcreteDecoratorA(simple);
    Component* decorator2 = new ConcreteDecoratorB(decorator1);
    cout << "客户端：现在我有一个装饰过的组件：\n";
    aClientCode(decorator2);
    cout << "\n";

    delete simple;
    delete decorator1;
    delete decorator2;
    return 0;
}