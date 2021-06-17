#include <iostream>
using std::cout;
using std::endl;
using std::string;

//服务接口 声明了服务接口, 代理必须遵循该接口才能伪装成服务对象
class Subject {
public:
    virtual void Request() const = 0;
};

class RealSubject : public Subject {
public:
    void Request() const override {
        cout << "真正的服务对象: 处理请求！\n";
    }
};

//代理 类包含一个指向服务对象的引用成员变量
//代理完成其任务 （例如延迟初始化、 记录日志、 访问控制和缓存等） 后会将请求传递给服务对象
//通常情况下， 代理会对其服务对象的整个生命周期进行管理
class Proxy : public Subject {
private:
    RealSubject* real_subject_;

    bool CheckAccess() const {
        cout << "代理：在触发真实请求之前检查访问 \n";
        return true;
    }
    void LogAccess() const {
        cout << "代理：记录请求的时间 \n";
    }

public:
    Proxy(RealSubject* real_subject) : real_subject_(new RealSubject(*real_subject)) {
    }

    ~Proxy() {
        delete real_subject_;
    }

    //代理在完成一些任务后应将工作委派给服务对象
    void Request() const override {
        if (this->CheckAccess()) {
            this->real_subject_->Request();
            this->LogAccess();
        }
    }
};

void ClientCode(const Subject& subject) {
    subject.Request();
}

int main() {
    cout << "客户端: 用真正的服务对象处理请求：\n";
    RealSubject* real_subject = new RealSubject;
    ClientCode(*real_subject);
    cout << "\n";
    cout << "客户端: 用代理对象处理请求：\n";
    Proxy* proxy = new Proxy(real_subject);
    ClientCode(*proxy);

    delete real_subject;
    delete proxy;
    return 0;
}