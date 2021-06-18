#include <iostream>
using std::cout;
using std::endl;
using std::string;

class Context;

//状态 接口会声明特定于状态的方法
class State {
protected:
    Context* context_;

public:
    virtual ~State() {
    }

    //状态对象， 可存储对于上下文对象的反向引用。 
    //状态可以通过该引用从上下文处获取所需信息， 并且能触发状态转移
    void set_context(Context* context) {
        this->context_ = context;
    }

    virtual void Handle1() = 0;
    virtual void Handle2() = 0;
};

//上下文 保存了对于一个具体状态对象的引用， 并会将所有与该状态相关的工作委派给它。 
//上下文通过状态接口与状态对象交互， 且会提供一个设置器用于传递新的状态对象
class Context {
private:
    State* state_;

public:
    Context(State* state) : state_(nullptr) {
        this->TransitionTo(state);
    }

    ~Context() {
        delete state_;
    }

    void TransitionTo(State* state) {
        cout << "上下文：过渡到 " << typeid(*state).name() << " \n";
        if (this->state_ != nullptr)
            delete this->state_;
        this->state_ = state;
        this->state_->set_context(this);
    }

    void Request1() {
        this->state_->Handle1();
    }
    void Request2() {
        this->state_->Handle2();
    }
};

//具体状态 会自行实现特定于状态的方法
class ConcreteStateA : public State {
public:
    void Handle1() override;

    void Handle2() override {
        cout << "具体状态A 处理 请求2 \n";
    }
};

class ConcreteStateB : public State {
public:
    void Handle1() override {
        cout << "具体状态B 处理 请求1 \n";
    }
    void Handle2() override {
        cout << "具体状态B 处理 请求2 \n";
        cout << "具体状态B 想要改变上下文的状态 \n";
        this->context_->TransitionTo(new ConcreteStateA);
    }
};

//上下文和具体状态都可以设置上下文的下个状态
void ConcreteStateA::Handle1() {
    {
        cout << "具体状态A 处理 请求1 \n";
        cout << "具体状态A 想要改变上下文的状态 \n";
        this->context_->TransitionTo(new ConcreteStateB);
    }
}

void ClientCode() {
    Context* context = new Context(new ConcreteStateA);
    context->Request1();
    context->Request2();
    delete context;
}

int main() {
    ClientCode();
    return 0;
}