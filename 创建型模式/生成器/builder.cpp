#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;

//产品 是最终生成的对象。 由不同生成器构造的产品无需属于同一类层次结构或接口
class Product1 {
public:
    vector<string> parts_;
    void ListParts()const {
        cout << "产品部分: ";
        for (size_t i = 0; i < parts_.size(); i++) {
            if (parts_[i] == parts_.back()) {
                cout << parts_[i];
            }
            else {
                cout << parts_[i] << ", ";
            }
        }
        cout << "\n\n";
    }
};

//生成器 接口声明在所有类型生成器中通用的产品构造步骤
class Builder {
public:
    virtual ~Builder() {}
    virtual void ProducePartA() const = 0;
    virtual void ProducePartB() const = 0;
    virtual void ProducePartC() const = 0;
};

//具体生成器 提供构造过程的不同实现。 具体生成器也可以构造不遵循通用接口的产品
class ConcreteBuilder1 : public Builder {
private:
    Product1* product;

public:
    ConcreteBuilder1() {
        this->Reset();
    }

    void Reset() {
        this->product = new Product1();
    }

    ~ConcreteBuilder1() {
        delete product;
    }

    void ProducePartA()const override {
        this->product->parts_.push_back("PartA1");
    }

    void ProducePartB()const override {
        this->product->parts_.push_back("PartB1");
    }

    void ProducePartC()const override {
        this->product->parts_.push_back("PartC1");
    }

    Product1* GetProduct() {
        Product1* result = this->product;
        this->Reset();
        return result;
    }
};

//主管 类定义调用构造步骤的顺序， 这样你就可以创建和复用特定的产品配置
class Director {
private:
    Builder* builder;

public:
    void set_builder(Builder* builder) {
        this->builder = builder;
    }

    void BuildMinimalViableProduct() {
        this->builder->ProducePartA();
    }

    void BuildFullFeaturedProduct() {
        this->builder->ProducePartA();
        this->builder->ProducePartB();
        this->builder->ProducePartC();
    }
};

//客户端 必须将某个生成器对象与主管类关联
void ClientCode(Director& director)
{
    ConcreteBuilder1* builder = new ConcreteBuilder1();
    director.set_builder(builder);
    cout << "标准基础产品：\n";
    director.BuildMinimalViableProduct();

    //产品展示
    Product1* p = builder->GetProduct();
    p->ListParts();
    delete p;

    cout << "标准全功能产品：\n";
    director.BuildFullFeaturedProduct();

    p = builder->GetProduct();
    p->ListParts();
    delete p;

    cout << "定制产品：\n";
    builder->ProducePartA();
    builder->ProducePartC();
    p = builder->GetProduct();
    p->ListParts();
    delete p;
    delete builder;
}

int main() {
    Director* director = new Director();
    ClientCode(*director);
    delete director;
    return 0;
}