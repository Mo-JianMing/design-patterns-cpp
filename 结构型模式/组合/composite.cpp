#include <iostream>
#include <list>
using std::cout;
using std::endl;
using std::string;
using std::list;

//组件 接口描述了树中简单项目和复杂项目所共有的操作
class Component {
protected:
    Component* parent_;

public:
    virtual ~Component() {}

    void SetParent(Component* parent) {
        this->parent_ = parent;
    }
    Component* GetParent() const {
        return this->parent_;
    }

    virtual void Add(Component* component) {}
    virtual void Remove(Component* component) {}

    virtual bool IsComposite() const {
        return false;
    }

    virtual string Operation() const = 0;
};

//叶节点 是树的基本结构， 它不包含子项目
//一般情况下， 叶节点最终会完成大部分的实际工作， 因为它们无法将工作指派给其他部分
class Leaf : public Component {
public:
    string Operation() const override {
        return "叶子节点";
    }
};

//容器 ——又名“组合 ”——是包含叶节点或其他容器等子项目的单位
//容器不知道其子项目所属的具体类， 它只通过通用的组件接口与其子项目交互
class Composite : public Component {

protected:
    list<Component*> children_;

public:
    void Add(Component* component) override {
        this->children_.push_back(component);
        component->SetParent(this);
    }

    void Remove(Component* component) override {
        this->children_.remove(component);
        component->SetParent(nullptr);
    }

    bool IsComposite() const override {
        return true;
    }

    //容器接收到请求后会将工作分配给自己的子项目， 处理中间结果， 然后将最终结果返回给客户端
    string Operation() const override {
        string result;
        for (const Component* c : children_) {
            if (c == children_.back()) {
                result += c->Operation();
            }
            else {
                result += c->Operation() + "+";
            }
        }
        return "分支 (" + result + ")";
    }
};

void ClientCode(Component* component) {
    cout << "结果: " << component->Operation();
}

void ClientCode2(Component* component1, Component* component2) {
    if (component1->IsComposite()) {
        component1->Add(component2);
    }
    cout << "结果: " << component1->Operation();
}

int main() {
    Component* simple = new Leaf;
    cout << "客户端：我有一个简单的组件：\n";
    ClientCode(simple);
    cout << "\n\n";

    Component* leaf_1 = new Leaf;
    Component* leaf_2 = new Leaf;
    Component* leaf_3 = new Leaf;

    Component* branch1 = new Composite;
    branch1->Add(leaf_1);
    branch1->Add(leaf_2);
    Component* branch2 = new Composite;
    branch2->Add(leaf_3);

    Component* tree = new Composite;
    tree->Add(branch1);
    tree->Add(branch2);
    cout << "客户端：现在我有了一个复合树：\n";
    ClientCode(tree);
    cout << "\n\n";

    cout << "客户端：即使在管理树时，我也不需要检查组件类：\n";
    ClientCode2(tree, simple);
    cout << "\n";

    delete simple;
    delete tree;
    delete branch1;
    delete branch2;
    delete leaf_1;
    delete leaf_2;
    delete leaf_3;
    return 0;
}