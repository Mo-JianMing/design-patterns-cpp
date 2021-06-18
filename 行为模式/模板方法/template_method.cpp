#include <iostream>
using std::cout;
using std::endl;
using std::string;

//抽象类 会声明作为算法步骤的方法， 以及依次调用它们的实际模板方法。
class AbstractClass {
public:
    void TemplateMethod() const {
        this->BaseOperation1();
        this->RequiredOperations1();
        this->BaseOperation2();
        this->Hook1();
        this->RequiredOperation2();
        this->BaseOperation3();
        this->Hook2();
    }

protected:
    void BaseOperation1() const {
        cout << "抽象类：我正在做大部分工作\n";
    }
    void BaseOperation2() const {
        cout << "抽象类：但我让子类覆盖一些操作\n";
    }
    void BaseOperation3() const {
        cout << "抽象类：但我正在做大部分工作\n";
    }

    virtual void RequiredOperations1() const = 0;
    virtual void RequiredOperation2() const = 0;

    virtual void Hook1() const {}
    virtual void Hook2() const {}
};

//具体类 可以重写所有步骤， 但不能重写模板方法自身
class ConcreteClass1 : public AbstractClass {
protected:
    void RequiredOperations1() const override {
        cout << "具体类1 ：已实施的操作 1\n";
    }
    void RequiredOperation2() const override {
        cout << "具体类1 ：已实施的操作 2\n";
    }
};

class ConcreteClass2 : public AbstractClass {
protected:
    void RequiredOperations1() const override {
        cout << "具体类2 ：已实施的操作 1\n";
    }
    void RequiredOperation2() const override {
        cout << "具体类2 ：已实施的操作 2\n";
    }
    void Hook1() const override {
        cout << "具体类2：重写 Hook1\n";
    }
};

void ClientCode(AbstractClass* class_) {
    class_->TemplateMethod();
}

int main() {
    cout << "相同的客户端代码可以用于不同的子类：\n";
    ConcreteClass1* concreteClass1 = new ConcreteClass1;
    ClientCode(concreteClass1);
    cout << "\n";
    cout << "相同的客户端代码可以用于不同的子类：\n";
    ConcreteClass2* concreteClass2 = new ConcreteClass2;
    ClientCode(concreteClass2);
    delete concreteClass1;
    delete concreteClass2;
    return 0;
}