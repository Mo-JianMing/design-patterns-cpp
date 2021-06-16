#include <iostream>
using std::cout;
using std::endl;
using std::string;

class Singleton
{
protected:
    //单例类的构造函数应该始终是私有的，
    //以防止外部直接使用 new 运算符进行构造调用
    Singleton(const string& value) : value_(value)
    {
    }
    //对象存储在静态区，才能被静态方法访问
    static Singleton* singleton_;
    string value_;

public:
    //因为单例类有且只有一个实例
    //所以应该删除拷贝构造函数，赋值运算符重载函数
    Singleton(Singleton& other) = delete;
    void operator=(const Singleton&) = delete;

    //向外提供获取唯一实例的静态方法
    static Singleton* GetInstance(const string& value);

    string value() const {
        return value_;
    }
};

Singleton* Singleton::singleton_ = nullptr;;

Singleton* Singleton::GetInstance(const string& value)
{
    if (singleton_ == nullptr) {
        singleton_ = new Singleton(value);
    }
    return singleton_;
}

int main()
{
    Singleton* singleton_A = Singleton::GetInstance("结婚对象—老婆^v^");
    cout << singleton_A->value() << endl;   //输出  老婆

    Singleton* singleton_B = Singleton::GetInstance("出轨对象—小三^/\^");
    cout << singleton_B->value() << endl;   //依然同一个实例，所以输出  老婆
    return 0;
}