#include <iostream>
#include <unordered_map>
using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::ostream;

//内在状态：包含不变的、可在许多对象中重复使用的数据的成员变量
struct SharedState
{
    string brand_;
    string model_;
    string color_;

    SharedState(const string& brand, const string& model, const string& color)
        : brand_(brand), model_(model), color_(color)
    {
    }

    friend ostream& operator<<(ostream& os, const SharedState& ss)
    {
        return os << "[ " << ss.brand_ << " , " << ss.model_ << " , " << ss.color_ << " ]";
    }
};

//外在状态：包含每个对象各自不同的情景数据的成员变量
struct UniqueState
{
    string owner_;
    string plates_;

    UniqueState(const string& owner, const string& plates)
        : owner_(owner), plates_(plates)
    {
    }

    friend ostream& operator<<(ostream& os, const UniqueState& us)
    {
        return os << "[ " << us.owner_ << " , " << us.plates_ << " ]";
    }
};

//享元 类包含原始对象中部分能在多个对象中共享的状态，同一享元对象可在许多不同情景中使用
//享元中存储的状态被称为 “内在状态”。 传递给享元方法的状态被称为 “外在状态”
class Flyweight
{
private:
    SharedState* shared_state_;

public:
    Flyweight(const SharedState* shared_state) : shared_state_(new SharedState(*shared_state))
    {
    }
    Flyweight(const Flyweight& other) : shared_state_(new SharedState(*other.shared_state_))
    {
    }
    ~Flyweight()
    {
        delete shared_state_;
    }

    SharedState* shared_state() const
    {
        return shared_state_;
    }

    void Operation(const UniqueState& unique_state) const
    {
        cout << "Flyweight：显示共享汽车 (" << *shared_state_
            << ") 其实际拥有者 (" << unique_state << ") \n";
    }
};

//享元工厂 （Flyweight Factory） 会对已有享元的缓存池进行管理
//有了工厂后， 客户端就无需直接创建享元， 它们只需调用工厂并向其传递目标享元的一些内在状态即可
//工厂会根据参数在之前已创建的享元中进行查找， 如果找到满足条件的享元就将其返回； 如果没有找到就根据参数新建享元
class FlyweightFactory
{
private:
    unordered_map<string, Flyweight> flyweights_;

    string GetKey(const SharedState& ss) const
    {
        return ss.brand_ + "_" + ss.model_ + "_" + ss.color_;
    }

public:
    FlyweightFactory(std::initializer_list<SharedState> share_states)
    {
        for (const SharedState& ss : share_states)
        {
            this->flyweights_.emplace(this->GetKey(ss), Flyweight(&ss));
        }
    }

    Flyweight GetFlyweight(const SharedState& shared_state)
    {
        string key = this->GetKey(shared_state);

        if (this->flyweights_.find(key) == this->flyweights_.end())
        {
            cout << "FlyweightFactory：找不到享元对象，创建新的 \n";
            this->flyweights_.emplace(key, Flyweight(&shared_state));
        }
        else
        {
            cout << "FlyweightFactory: 重用现有的享元对象 \n";
        }
        return this->flyweights_.at(key);
    }
    void ListFlyweights() const
    {
        size_t count = this->flyweights_.size();

        cout << "\nFlyweightFactory: 我拥有 " << count << " 个享元对象：\n";
        for (auto pair : this->flyweights_)
        {
            cout << pair.first << "\n";
        }
    }
};

void AddCarToPoliceDatabase(
    FlyweightFactory& ff, const string& plates, const string& owner,
    const string& brand, const string& model, const string& color)
{
    cout << "\n客户端：将汽车添加到数据库中 \n";

    //客户端就只能通过工厂来请求享元， 它们需要将享元的内在状态作为参数传递给工厂
    const Flyweight& flyweight = ff.GetFlyweight({ brand, model, color });

    flyweight.Operation({ owner, plates });
}

int main()
{
    FlyweightFactory* factory = new FlyweightFactory({
        {"雪佛兰", "科迈罗 2018", "粉色的"}, {"奔驰", "C300", "黑色的"},
        {"奔驰", "C500", "红色的"}, {"宝马", "M5", "红色的"}, {"宝马", "X6", "白色的"}
        });
    factory->ListFlyweights();

    AddCarToPoliceDatabase(*factory,
        "粤A 00001",
        "邓小平",
        "宝马",
        "M5",
        "红色的");

    AddCarToPoliceDatabase(*factory,
        "粤A 00002",
        "邓小平",
        "宝马",
        "X1",
        "红色的");
    factory->ListFlyweights();

    delete factory;
    return 0;
}