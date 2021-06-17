#include <iostream>
using std::cout;
using std::endl;
using std::string;
using std::swap;

//客户端接口 描述了其他类与客户端代码合作时必须遵循的协议
class Target {
public:
    virtual ~Target() = default;

    virtual string Request() const {
        return "目标：默认目标的行为";
    }
};

//服务 中有一些功能类 （通常来自第三方或遗留系统）
//客户端与其接口不兼容， 因此无法直接调用其功能
class Adaptee {
public:
    string SpecificRequest() const {
        return "为行的标目认默：标目";
    }
};

//适配器 是一个可以同时与客户端和服务交互的类： 它在实现客户端接口的同时封装了服务对象
//适配器接受客户端通过适配器接口发起的调用， 并将其转换为适用于被封装服务对象的调用
class Adapter : public Target {
private:
    Adaptee* adaptee_;

    //中文字符在VS编译器是占三个字节
    void chinese_reverse(string& str) const {
        int right = str.length();
        for (int left = 0; left < right; left += 3, right -= 3) {
            swap(str[left], str[right - 3]);
            swap(str[left + 1], str[right - 2]);
            swap(str[left + 2], str[right - 1]);
        }
    }
public:
    Adapter(Adaptee* adaptee) : adaptee_(adaptee) {}

    //改造成符合目标接口的行为
    string Request() const override {
        string to_reverse = this->adaptee_->SpecificRequest();
        chinese_reverse(to_reverse);
        return "适配器：(已翻译) " + to_reverse;
    }
};

//客户端代码只需通过接口与适配器交互即可， 无需与具体的适配器类耦合
void ClientCode(const Target* target) {
    cout << target->Request();
}

int main() {
    cout << "客户端：我可以很好地处理 Target 对象 \n";
    Target* target = new Target;
    ClientCode(target);
    cout << "\n\n";
    Adaptee* adaptee = new Adaptee;
    cout << "客户端：Adaptee 类有一个奇怪的接口，不兼容无法使用 \n";
    cout << "Adaptee: " << adaptee->SpecificRequest();
    cout << "\n\n";
    cout << "客户端：但我可以通过适配器使用它 \n";
    Adapter* adapter = new Adapter(adaptee);
    ClientCode(adapter);
    cout << "\n";

    delete target;
    delete adaptee;
    delete adapter;
    return 0;
}