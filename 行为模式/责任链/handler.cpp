#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;

//声明处理者接口并描述请求处理方法的签名
class Handler {
public:
    //确定客户端如何将请求数据传递给方法。 最灵活的方式是将请求转换为对象， 然后将其以参数的形式传递给处理函数
    virtual Handler* SetNext(Handler* handler) = 0;
    virtual string Handle(string request) = 0;
};

//为了在具体处理者中消除重复的样本代码，可以根据处理者接口创建抽象处理者基类
class AbstractHandler : public Handler {
private:
    //需要有一个成员变量来存储指向链上，下个处理者的引用
    Handler* next_handler_;

public:
    AbstractHandler() : next_handler_(nullptr) {
    }
    //打算在运行时对链进行改变，来修改引用成员变量的值
    Handler* SetNext(Handler* handler) override {
        this->next_handler_ = handler;
        return handler;
    }
    string Handle(string request) override {
        if (this->next_handler_) {
            return this->next_handler_->Handle(request);
        }
        return {};
    }
};

class MonkeyHandler : public AbstractHandler {
public:
    string Handle(string request) override {
        //是否自行处理这个请求
        if (request == "香蕉") {
            return "猴子：我要吃" + request + "\n";
        }
        //是否将该请求沿着链进行传递
        else {
            return AbstractHandler::Handle(request);
        }
    }
};
class SquirrelHandler : public AbstractHandler {
public:
    string Handle(string request) override {
        if (request == "坚果") {
            return "松鼠：我要吃" + request + "\n";
        }
        else {
            return AbstractHandler::Handle(request);
        }
    }
};
class DogHandler : public AbstractHandler {
public:
    string Handle(string request) override {
        if (request == "肉丸") {
            return "狗：我要吃" + request + "\n";
        }
        else {
            return AbstractHandler::Handle(request);
        }
    }
};

void ClientCode(Handler& handler) {
    vector<string> food = { "坚果", "香蕉", "卡布奇诺" };
    for (const string& f : food) {
        cout << "客户端：谁想要一个" << f << " ?\n";
        const string result = handler.Handle(f);
        //链中可能只有单个链接，部分请求可能无法到达链尾
        if (!result.empty()) {
            cout << "  " << result;
        }
        //其他请求可能直到链尾都未被处理
        else {
            cout << "  " << f << " 直至链尾都未被处理\n";
        }
    }
}

int main() {
    MonkeyHandler* monkey = new MonkeyHandler;
    SquirrelHandler* squirrel = new SquirrelHandler;
    DogHandler* dog = new DogHandler;
    monkey->SetNext(squirrel)->SetNext(dog);

    cout << "责任链：猴子>松鼠>狗\n\n";
    ClientCode(*monkey);
    cout << "\n";
    cout << "子链：松鼠>狗\n\n";
    ClientCode(*squirrel);

    delete monkey;
    delete squirrel;
    delete dog;
    return 0;
}