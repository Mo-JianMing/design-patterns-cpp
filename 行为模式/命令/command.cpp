#include <iostream>
using std::cout;
using std::endl;
using std::string;

//命令 接口通常仅声明一个执行命令的方法
class Command {
public:
    virtual ~Command() {
    }
    virtual void Execute() const = 0;
};

//具体命令 会实现各种类型的请求。 具体命令自身并不完成工作， 而是会将调用委派给一个业务逻辑对象
class SimpleCommand : public Command {
private:
    string pay_load_;

public:
    explicit SimpleCommand(string pay_load) : pay_load_(pay_load) {
    }
    void Execute() const override {
        cout << "简单命令：看，我可以做一些简单的事情，比如打印 (" << this->pay_load_ << ")\n";
    }
};

//接收者 类包含部分业务逻辑。 几乎任何对象都可以作为接收者。 
//绝大部分命令只处理如何将请求传递到接收者的细节， 接收者自己会完成实际的工作
class Receiver {
public:
    void DoSomething(const string& a) {
        cout << "接收器：正在处理 (" << a << ".)\n";
    }
    void DoSomethingElse(const string& b) {
        cout << "接收器：也在工作 (" << b << ".)\n";
    }
};

//复杂的命令 包含一个成员变量来存储接收者对象的引用
class ComplexCommand : public Command {
private:
    Receiver* receiver_;

    string a_;
    string b_;

public:
    ComplexCommand(Receiver* receiver, string a, string b) : receiver_(receiver), a_(a), b_(b) {
    }

    void Execute() const override {
        cout << "复杂命令：复杂的事情应该由接收者对象来完成 \n";
        this->receiver_->DoSomething(this->a_);
        this->receiver_->DoSomethingElse(this->b_);
    }
};

//触发者 类负责对请求进行初始化， 其中必须包含一个成员变量来存储对于命令对象的引用
//作用是触发命令， 而不向接收者直接发送请求
class Invoker {
private:
    Command* on_start_;
    Command* on_finish_;

public:
    ~Invoker() {
        delete on_start_;
        delete on_finish_;
    }

    void SetOnStart(Command* command) {
        this->on_start_ = command;
    }
    void SetOnFinish(Command* command) {
        this->on_finish_ = command;
    }

    void DoSomethingImportant() {
        cout << "触发者：有人要在我开始之前做点什么吗？\n";
        if (this->on_start_) {
            this->on_start_->Execute();
        }
        cout << "触发者: ......做一些非常重要的事情......\n";
        cout << "触发者: 我完成后有人要做什么吗？\n";
        if (this->on_finish_) {
            this->on_finish_->Execute();
        }
    }
};

int main() {
    Invoker* invoker = new Invoker;
    invoker->SetOnStart(new SimpleCommand("你好！"));
    Receiver* receiver = new Receiver;
    invoker->SetOnFinish(new ComplexCommand(receiver, "发送邮件", "保存报告"));
    invoker->DoSomethingImportant();

    delete invoker;
    delete receiver;
    return 0;
}