#include <iostream>
#include <vector>
#include <algorithm>
using std::cout;
using std::endl;
using std::string;
using std::vector;

//策略 接口是所有具体策略的通用接口， 它声明了一个上下文用于执行策略的方法
class Strategy
{
public:
    virtual ~Strategy() {}
    virtual string DoAlgorithm(const vector<string>& data) const = 0;
};

//上下文 维护指向具体策略的引用， 且仅通过策略接口与该对象进行交流
class Context
{
private:
    Strategy* strategy_;

public:
    Context(Strategy* strategy = nullptr) : strategy_(strategy)
    {
    }
    ~Context()
    {
        delete this->strategy_;
    }

    void set_strategy(Strategy* strategy)
    {
        delete this->strategy_;
        this->strategy_ = strategy;
    }

    //当上下文需要运行算法时， 它会在其已连接的策略对象上调用执行方法
    void DoSomeBusinessLogic() const
    {
        cout << "上下文：使用策略对数据进行排序（不确定它会如何做）\n";
        string result = this->strategy_->DoAlgorithm(vector<string>{"a", "e", "c", "b", "d"});
        cout << result << "\n";
    }
};

//具体策略 实现了上下文所用算法的各种不同变体
class ConcreteStrategyA : public Strategy
{
public:
    string DoAlgorithm(const vector<string>& data) const override
    {
        string result;
        for_each(begin(data), end(data), [&result](const string& letter) {
            result += letter;
            });
        sort(begin(result), end(result));

        return result;
    }
};
class ConcreteStrategyB : public Strategy
{
    string DoAlgorithm(const vector<string>& data) const override
    {
        string result;
        for_each(begin(data), end(data), [&result](const string& letter) {
            result += letter;
            });
        sort(begin(result), end(result));
        for (int i = 0; i < result.size() / 2; i++)
        {
            std::swap(result[i], result[result.size() - i - 1]);
        }

        return result;
    }
};

void ClientCode()
{
    Context* context = new Context(new ConcreteStrategyA);
    cout << "客户端：策略设置为正常排序 \n";
    context->DoSomeBusinessLogic();
    cout << "\n";
    cout << "客户端：策略设置为反向排序 \n";
    context->set_strategy(new ConcreteStrategyB);
    context->DoSomeBusinessLogic();
    delete context;
}

int main()
{
    ClientCode();
    return 0;
}