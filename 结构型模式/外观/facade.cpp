#include <iostream>
using std::cout;
using std::endl;
using std::string;

//复杂子系统 由多个不同对象构成。 如果要用这些对象完成有意义的工作， 你必须深入了解子系统的实现细节
//比如按照正确顺序初始化对象和为其提供正确格式的数据
//子系统类不会意识到外观的存在， 它们在系统内运作并且相互之间可直接进行交互
class Subsystem1 {
public:
    string Operation1() const {
        return "子系统1：准备好了！\n";
    }

    string OperationN() const {
        return "子系统1：启动！\n";
    }
};

class Subsystem2 {
public:
    string Operation1() const {
        return "子系统2：准备好了！\n";
    }

    string OperationZ() const {
        return "子系统2：启动！\n";
    }
};

//外观 提供了一种访问特定子系统功能的便捷方式， 其了解如何重定向客户端请求， 知晓如何操作一切活动部件
class Facade {
protected:
    Subsystem1* subsystem1_;
    Subsystem2* subsystem2_;

public:
    Facade(
        Subsystem1* subsystem1 = nullptr,
        Subsystem2* subsystem2 = nullptr) {
        this->subsystem1_ = subsystem1 ? subsystem1 : new Subsystem1;
        this->subsystem2_ = subsystem2 ? subsystem2 : new Subsystem2;
    }
    ~Facade() {
        delete subsystem1_;
        delete subsystem2_;
    }

    //将客户端代码的调用重定向到子系统中的相应对象处
    string Operation() {
        string result = "Facade 初始化子系统：\n";
        result += this->subsystem1_->Operation1();
        result += this->subsystem2_->Operation1();
        result += "Facade 命令子系统执行操作：\n";
        result += this->subsystem1_->OperationN();
        result += this->subsystem2_->OperationZ();
        return result;
    }
};

void ClientCode(Facade* facade) {
    cout << facade->Operation();
}

int main() {
    Subsystem1* subsystem1 = new Subsystem1;
    Subsystem2* subsystem2 = new Subsystem2;
    Facade* facade = new Facade(subsystem1, subsystem2);
    ClientCode(facade);

    delete facade;
    return 0;
}