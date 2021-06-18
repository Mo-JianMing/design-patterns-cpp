#include <iostream>
#include <array>
using std::cout;
using std::endl;
using std::string;
using std::array;

class ConcreteComponentA;
class ConcreteComponentB;

//访问者 接口声明了一系列以对象结构的具体元素为参数的访问者方法
class Visitor {
public:
    virtual void VisitConcreteComponentA(const ConcreteComponentA* element) const = 0;
    virtual void VisitConcreteComponentB(const ConcreteComponentB* element) const = 0;
};

//元素 接口声明了一个方法来 “接收” 访问者
class Component {
public:
    virtual ~Component() {}
    virtual void Accept(Visitor* visitor) const = 0;
};

//具体元素 必须实现接收方法。 该方法的目的是根据当前元素类将其调用重定向到相应访问者的方法
class ConcreteComponentA : public Component {
public:
    void Accept(Visitor* visitor) const override {
        visitor->VisitConcreteComponentA(this);
    }

    string ExclusiveMethodOfConcreteComponentA() const {
        return "A";
    }
};

class ConcreteComponentB : public Component {
public:
    void Accept(Visitor* visitor) const override {
        visitor->VisitConcreteComponentB(this);
    }
    string SpecialMethodOfConcreteComponentB() const {
        return "B";
    }
};

//具体访问者 会为不同的具体元素类实现相同行为的几个不同版本
class ConcreteVisitor1 : public Visitor {
public:
    void VisitConcreteComponentA(const ConcreteComponentA* element) const override {
        cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor1\n";
    }

    void VisitConcreteComponentB(const ConcreteComponentB* element) const override {
        cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor1\n";
    }
};

class ConcreteVisitor2 : public Visitor {
public:
    void VisitConcreteComponentA(const ConcreteComponentA* element) const override {
        cout << element->ExclusiveMethodOfConcreteComponentA() << " + ConcreteVisitor2\n";
    }
    void VisitConcreteComponentB(const ConcreteComponentB* element) const override {
        cout << element->SpecialMethodOfConcreteComponentB() << " + ConcreteVisitor2\n";
    }
};

void ClientCode(array<const Component*, 2> components, Visitor* visitor) {

    for (const Component* comp : components) {
        comp->Accept(visitor);
    }
}

int main() {
    array<const Component*, 2> components = { new ConcreteComponentA, new ConcreteComponentB };
    cout << "客户端代码通过基本访问者界面与所有访问者一起工作：\n";
    ConcreteVisitor1* visitor1 = new ConcreteVisitor1;
    ClientCode(components, visitor1);
    cout << "\n";
    cout << "它允许相同的客户端代码与不同类型的访问者一起工作：\n";
    ConcreteVisitor2* visitor2 = new ConcreteVisitor2;
    ClientCode(components, visitor2);

    for (const Component* comp : components) {
        delete comp;
    }
    delete visitor1;
    delete visitor2;

    return 0;
}