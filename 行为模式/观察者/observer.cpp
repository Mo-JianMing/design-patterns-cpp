#include <iostream>
#include <list>
using std::cout;
using std::endl;
using std::string;
using std::list;

//订阅者 接口声明了通知接口。 在绝大多数情况下， 该接口仅包含一个 update更新方法。 
//该方法可以拥有多个参数， 使发布者能在更新时传递事件的详细信息
class IObserver {
public:
    virtual ~IObserver() {};
    virtual void Update(const string& message_from_subject) = 0;
};

//发布者 会向其他对象发送值得关注的事件。 事件会在发布者自身状态改变或执行特定行为后发生
class ISubject {
public:
    virtual ~ISubject() {};
    virtual void Attach(IObserver* observer) = 0;
    virtual void Detach(IObserver* observer) = 0;
    virtual void Notify() = 0;
};

class Subject : public ISubject {
public:
    virtual ~Subject() {
        cout << "再见，我是up主 \n";
    }

    void Attach(IObserver* observer) override {
        list_observer_.push_back(observer);
    }
    void Detach(IObserver* observer) override {
        list_observer_.remove(observer);
    }

    //当新事件发生时， 发送者会遍历订阅列表并调用每个订阅者对象的通知方法
    void Notify() override {
        list<IObserver*>::iterator iterator = list_observer_.begin();
        HowManyObserver();
        while (iterator != list_observer_.end()) {
            (*iterator)->Update(message_);
            ++iterator;
        }
    }

    void CreateMessage(string message = "空的") {
        this->message_ = message;
        Notify();
    }
    void HowManyObserver() {
        cout << "有 " << list_observer_.size() << " 个名单中的观察员 \n";
    }

    void SomeBusinessLogic() {
        this->message_ = "更改消息";
        Notify();
        cout << "我即将做一些重要的事情\n";
    }

private:
    list<IObserver*> list_observer_;
    string message_;
};

//具体订阅者 可以执行一些操作来回应发布者的通知
class Observer : public IObserver {
public:
    Observer(Subject& subject) : subject_(subject) {
        this->subject_.Attach(this);
        cout << "你好，我是观察员 \"" << ++Observer::static_number_ << "\".\n";
        this->number_ = Observer::static_number_;
    }
    virtual ~Observer() {
        cout << "再见，我是观察者 \"" << this->number_ << "\".\n";
    }

    void Update(const string& message_from_subject) override {
        message_from_subject_ = message_from_subject;
        PrintInfo();
    }
    void RemoveMeFromTheList() {
        subject_.Detach(this);
        cout << "观察员 \"" << number_ << "\" 从列表中删除 \n";
    }
    void PrintInfo() {
        cout << "观察员 \"" << this->number_ << "\": 有新消息可用 --> " << this->message_from_subject_ << "\n";
    }

private:
    string message_from_subject_;
    Subject& subject_;
    static int static_number_;
    int number_;
};

int Observer::static_number_ = 0;

void ClientCode() {
    Subject* subject = new Subject;
    Observer* observer1 = new Observer(*subject);
    Observer* observer2 = new Observer(*subject);
    Observer* observer3 = new Observer(*subject);
    Observer* observer4;
    Observer* observer5;

    subject->CreateMessage("你好，世界！ :D");
    observer3->RemoveMeFromTheList();

    subject->CreateMessage("今天天气很热！ :p");
    observer4 = new Observer(*subject);

    observer2->RemoveMeFromTheList();
    observer5 = new Observer(*subject);

    subject->CreateMessage("我的新车很棒！ ;)");
    observer5->RemoveMeFromTheList();

    observer4->RemoveMeFromTheList();
    observer1->RemoveMeFromTheList();

    delete observer5;
    delete observer4;
    delete observer3;
    delete observer2;
    delete observer1;
    delete subject;
}

int main() {
    ClientCode();
    return 0;
}