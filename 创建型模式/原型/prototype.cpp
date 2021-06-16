#include <iostream>
#include <unordered_map>
using std::cout;
using std::endl;
using std::string;
using std::unordered_map;

//原型 接口将对克隆方法进行声明。 在绝大多数情况下， 其中只会有一个名为 clone克隆的方法
class Prototype {
protected:
    string prototype_name_;

public:
    Prototype() {}
    Prototype(string prototype_name)
        : prototype_name_(prototype_name) {
    }
    virtual ~Prototype() {}

    //派生类需要实现克隆
    virtual Prototype* Clone() const = 0;

    //含有纯虚函数的类  不能创建对象
    virtual void Method() const = 0;
};

//具体原型 类将实现克隆方法。 除了将原始对象的数据复制到克隆体中之外， 
//该方法有时还需处理克隆过程中的极端情况， 例如克隆关联对象和梳理递归依赖等等
class ConcretePrototype1 : public Prototype {
private:
    float concrete_prototype_field1_;

public:
    ConcretePrototype1(string prototype_name, float concrete_prototype_field)
        : Prototype(prototype_name), concrete_prototype_field1_(concrete_prototype_field) {
    }

    //返回一个指向克隆体的指针，客户端有责任delete它，或者用智能指针
    Prototype* Clone() const override {
        return new ConcretePrototype1(*this);
    }

    void Method() const override {
        cout << "具体原型类 " << prototype_name_ << " 的内容为 : " << concrete_prototype_field1_ << endl;
    }
};

class ConcretePrototype2 : public Prototype {
private:
    float concrete_prototype_field2_;

public:
    ConcretePrototype2(string prototype_name, float concrete_prototype_field)
        : Prototype(prototype_name), concrete_prototype_field2_(concrete_prototype_field) {
    }
    Prototype* Clone() const override {
        return new ConcretePrototype2(*this);
    }
    void Method() const override {
        cout << "具体原型类 " << prototype_name_ << " 的内容为 : " << concrete_prototype_field2_ << endl;
    }
};

enum Type {
    PROTOTYPE_1 = 0,
    PROTOTYPE_2
};

class PrototypeFactory {
private:
    unordered_map<Type, Prototype*, std::hash<int>> prototypes_;

public:
    PrototypeFactory() {
        prototypes_[Type::PROTOTYPE_1] = new ConcretePrototype1("PROTOTYPE_1 ", 50.f);
        prototypes_[Type::PROTOTYPE_2] = new ConcretePrototype2("PROTOTYPE_2 ", 60.f);
    }

    ~PrototypeFactory() {
        delete prototypes_[Type::PROTOTYPE_1];
        delete prototypes_[Type::PROTOTYPE_2];
    }

    //只需要指定想要的原型类型，就可以克隆此类型的对象
    Prototype* CreatePrototype(Type type) {
        return prototypes_[type]->Clone();
    }
};

void Client(PrototypeFactory& prototype_factory) {
    cout << "创建一个 Prototype 1\n";
    Prototype* prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_1);
    prototype->Method();
    delete prototype;
    cout << "\n";

    cout << "创建一个 Prototype 2 \n";
    prototype = prototype_factory.CreatePrototype(Type::PROTOTYPE_2);
    prototype->Method();
    delete prototype;
}

int main() {
    PrototypeFactory* prototype_factory = new PrototypeFactory();
    Client(*prototype_factory);
    delete prototype_factory;
    return 0;
}