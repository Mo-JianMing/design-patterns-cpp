#include <iostream>
using std::cout;
using std::endl;
using std::string;

class BaseComponent;

//中介者 接口声明了与组件交流的方法， 但通常仅包括一个通知方法
class Mediator {
public:
    virtual void Notify(BaseComponent* sender, string event) const = 0;
};

//组件 是各种包含业务逻辑的类。 每个组件都有一个指向中介者的引用， 
//该引用被声明为中介者接口类型。 组件不知道中介者实际所属的类， 
//因此可通过将其连接到不同的中介者以使其能在其他程序中复用
class BaseComponent {
protected:
    Mediator* mediator_;

public:
    BaseComponent(Mediator* mediator = nullptr) : mediator_(mediator) {
    }
    void set_mediator(Mediator* mediator) {
        this->mediator_ = mediator;
    }
};

//具体组件并不知道其他组件的情况。 如果组件内发生了重要事件， 它只能通知中介者处理
class Component1 : public BaseComponent {
public:
    void DoA() {
        cout << "组件 1 执行 A\n";
        this->mediator_->Notify(this, "A");
    }
    void DoB() {
        cout << "组件 1 执行 B\n";
        this->mediator_->Notify(this, "B");
    }
};

class Component2 : public BaseComponent {
public:
    void DoC() {
        cout << "组件 2 执行 C\n";
        this->mediator_->Notify(this, "C");
    }
    void DoD() {
        cout << "组件 2 执行 D\n";
        this->mediator_->Notify(this, "D");
    }
};

//具体中介者 封装了多种组件间的关系。 
//具体中介者通常会保存所有组件的引用并对其进行管理， 甚至有时会对其生命周期进行管理
class ConcreteMediator : public Mediator {
private:
    Component1* component1_;
    Component2* component2_;

public:
    ConcreteMediator(Component1* c1, Component2* c2) : component1_(c1), component2_(c2) {
        this->component1_->set_mediator(this);
        this->component2_->set_mediator(this);
    }
    void Notify(BaseComponent* sender, string event) const override {
        if (event == "A") {
            cout << "中介 对 A 做出反应并触发以下操作：\n";
            this->component2_->DoC();
        }
        if (event == "D") {
            cout << "中介 对 D 做出反应并触发以下操作：\n";
            this->component1_->DoB();
            this->component2_->DoC();
        }
    }
};

void ClientCode() {
    Component1* c1 = new Component1;
    Component2* c2 = new Component2;
    ConcreteMediator* mediator = new ConcreteMediator(c1, c2);
    cout << "客户端 触发操作 A \n";
    c1->DoA();
    cout << "\n";
    cout << "客户端 触发操作 D \n";
    c2->DoD();

    delete c1;
    delete c2;
    delete mediator;
}

int main() {
    ClientCode();
    return 0;
}