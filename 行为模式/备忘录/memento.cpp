#include <iostream>
#include <vector>
#include <ctime>
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Memento {
public:
    virtual string GetName() const = 0;
    virtual string date() const = 0;
    virtual string state() const = 0;
};

//备忘录是原发器状态快照的值对象， 通过构造函数一次性传递数据
class ConcreteMemento : public Memento {
private:
    string state_;
    string date_;

public:
    ConcreteMemento(const string& state) : state_(state) {
        std::time_t now = std::time(0);
        this->date_ = std::ctime(&now);
    }

    string state() const override {
        return this->state_;
    }

    string GetName() const override {
        return this->date_ + " / (" + this->state_.substr(0, 9) + "...)";
    }

    string date() const override {
        return this->date_;
    }
};

//原发器类可以生成自身状态的快照， 也可以在需要时通过快照恢复自身状态
class Originator {
private:
    string state_;

    string GenerateRandomString(int length = 10) {
        const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        int stringLength = sizeof(alphanum) - 1;

        string random_string;
        for (int i = 0; i < length; i++) {
            random_string += alphanum[std::rand() % stringLength];
        }
        return random_string;
    }

public:
    Originator(const string& state) : state_(state) {
        cout << "原发器：我的初始状态是：" << this->state_ << "\n";
    }

    void DoSomething() {
        cout << "原发器：我在做重要的事情 \n";
        this->state_ = this->GenerateRandomString(30);
        cout << "原发器：我的状态已更改为：" << this->state_ << "\n";
    }

    //原发器必须通过备忘录构造函数的一个或多个实际参数来将自身状态传递给备忘录
    Memento* Save() {
        return new ConcreteMemento(this->state_);
    }

    void Restore(Memento* memento) {
        this->state_ = memento->state();
        cout << "原发器：我的状态变成了：" << this->state_ << "\n";
    }
};

//负责人 仅知道 “何时” 和 “为何” 捕捉原发器的状态， 以及何时恢复状态
class Caretaker {
private:
    //负责人通过保存备忘录栈来记录原发器的历史状态
    vector<Memento*> mementos_;
    Originator* originator_;

public:
    Caretaker(Originator* originator) : originator_(originator)
    {
    }

    void Backup() {
        cout << "\n 负责人：保存原发器的状态...\n";
        this->mementos_.push_back(this->originator_->Save());
    }

    //当原发器需要回溯历史状态时， 负责人将从栈中获取最顶部的备忘录， 并将其传递给原发器的恢复方法
    void Undo() {
        if (!this->mementos_.size()) {
            return;
        }
        Memento* memento = this->mementos_.back();
        this->mementos_.pop_back();
        cout << "负责人：将状态恢复为：" << memento->GetName() << "\n";
        try {
            this->originator_->Restore(memento);
        }
        catch (...) {
            this->Undo();
        }
    }

    void ShowHistory() const {
        cout << "负责人：这是备忘录列表：\n";
        for (Memento* memento : this->mementos_) {
            cout << memento->GetName() << "\n";
        }
    }
};

void ClientCode() {
    Originator* originator = new Originator("Super-duper-super-puper-super.");
    Caretaker* caretaker = new Caretaker(originator);
    caretaker->Backup();
    originator->DoSomething();
    caretaker->Backup();
    originator->DoSomething();
    caretaker->Backup();
    originator->DoSomething();
    cout << "\n";
    caretaker->ShowHistory();
    cout << "\n 客户端：现在，让我们回滚操作！\n";
    caretaker->Undo();
    cout << "\n 客户端：再来一次！\n";
    caretaker->Undo();
    cout << "\n 客户端：回到开始状态！\n";
    caretaker->Undo();

    delete originator;
    delete caretaker;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    ClientCode();
    return 0;
}