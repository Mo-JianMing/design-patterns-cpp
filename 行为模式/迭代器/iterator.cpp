#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;

//迭代器 接口声明了遍历集合所需的操作： 获取下一个元素、 获取当前位置和重新开始迭代等
template <typename T, typename U>
class Iterator {
public:
    typedef typename vector<T>::iterator iter_type;

    //迭代器对象必须与单个集合实体链接
    Iterator(U* p_data, bool reverse = false) : m_p_data_(p_data) {
        m_it_ = m_p_data_->m_data_.begin();
    }

    void First() {
        m_it_ = m_p_data_->m_data_.begin();
    }

    void Next() {
        m_it_++;
    }

    bool IsDone() {
        return (m_it_ == m_p_data_->m_data_.end());
    }

    iter_type Current() {
        return m_it_;
    }

private:
    U* m_p_data_;
    iter_type m_it_;
};

//集合 接口声明一个或多个方法来获取与集合兼容的迭代器
//请注意， 返回方法的类型必须被声明为迭代器接口， 因此具体集合可以返回各种不同种类的迭代器
template <class T>
class Container {
    friend class Iterator<T, Container>;

public:
    void Add(T a) {
        m_data_.push_back(a);
    }

    Iterator<T, Container>* CreateIterator() {
        //集合对象必须将自身传递给迭代器的构造函数来创建两者之间的链接
        return new Iterator<T, Container>(this);
    }

private:
    vector<T> m_data_;
};

class Data {
public:
    Data(int a = 0) : m_data_(a) {}

    void set_data(int a) {
        m_data_ = a;
    }

    int data() {
        return m_data_;
    }

private:
    int m_data_;
};

//客户端 通过集合和迭代器的接口与两者进行交互。
//这样一来客户端无需与具体类进行耦合， 允许同一客户端代码使用各种不同的集合和迭代器
void ClientCode() {
    cout << "________________带 int 的迭代器______________________________________" << endl;
    Container<int> cont;

    for (int i = 0; i < 10; i++) {
        cont.Add(i);
    }

    Iterator<int, Container<int>>* it = cont.CreateIterator();
    for (it->First(); !it->IsDone(); it->Next()) {
        cout << *it->Current() << endl;
    }

    Container<Data> cont2;
    Data a(100), b(1000), c(10000);
    cont2.Add(a);
    cont2.Add(b);
    cont2.Add(c);

    cout << "________________具有自定义类的迭代器______________________________" << endl;
    Iterator<Data, Container<Data>>* it2 = cont2.CreateIterator();
    for (it2->First(); !it2->IsDone(); it2->Next()) {
        cout << it2->Current()->data() << endl;
    }
    delete it;
    delete it2;
}

int main() {
    ClientCode();
    return 0;
}