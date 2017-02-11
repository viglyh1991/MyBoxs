//
//  main.cpp
//  smart_ptr
//
//  Created by zuzuche on 2017/2/10.
//  Copyright © 2017年 viglyh. All rights reserved.
//

/*
 一、RAII 机制： Resource Acquisition Is Initialization
  1> 资源获取即初始化，在使用资源的类的构造函数中申请资源，然后使用，最后在析构函数中释放资源
  2> 如果对象是用声明的方式在栈上创建的，那么RAII机制会工作正常
  3> 如果对象是用new操作符在堆上创建的，那么它的析构函数不会自动调用，程序员必须明确地用对应的delete操作符销毁它才能释放资源。
 
 二、智能指针
  1> std::auto_ptr : 用对象来代理指针，是因为重载了 operator* 和 operator->
 
  2> scope_ptr:
    > scope_ptr vs. auto_ptr
      > auto_ptr: 指针所有权可转移, 同一时刻只能有一个auto_ptr管理指针。
      > scope_ptr: 拷贝构造函数和赋值函数声明为私有的，拒绝了指针所有权的转让，从而保证了指针的绝对安全。
  
  3> scoped_array: 与scope_ptr类似，包装了new[]操作符在堆上分配的动态数组，为动态数组提供了一个代理，保证可以正确地释放内存。
    > 不推荐使用 scoped_array, 需要动态数组应使用 std::vector
  
  4> shared_ptr
    > 引用计数型的智能指针
    > 工厂函数 make_shared<T>(), 消除显示的new调用
      template<class T, class... Args> shared_ptr<T> make_shared( Args && ... args);
    > 应用于标准容器: 
      1) 将容器作为 shared_ptr 管理的对象, 如 shared_ptr<list<T>>
      2) 将shared_ptr作为容器的元素，如vector<shared_ptr<T>>,因为shared_ptr支持拷贝语义和比较操作，符合标准容器对元素的要求。标准容器不能容纳 auto_ptr scoped_ptr
  
  5> shared_array
 
  6> weak_ptr
    > 没有重载 operator* 和 ->
    > 被设计为与 shared_ptr 共同工作，可以从一个shared_ptr或者另一个weak_ptr对象构造，获得资源的观测权
    > weak_ptr没有共享资源，它的构造不会引起指针引用计数的增加，析构时也不会导致引用计数减少
    > lock() 返回被观测的shared_ptr 对象
 
  7> intrusive_ptr 
    > 侵入式的引用计数型指针。不推荐使用
 
  8> pool库
    》内存池预先分配了一块大的内存空间,然后就可以在其中使用某种算法实现高效快速的自定制内存分配
    》boost.pool库近似于一个小型的垃圾回收机制，在需要大量地分配/释放小对象时很有效率，而且完全不需要考虑delete
    》包含四个组成部分
      1) 基本pool: <boost/pool/pool.hpp>
      2) object_pool:
      3) singleton_pool:
      4) poll_alloc:
 */

#include <iostream>
#include <vector>
#include <boost/smart_ptr.hpp>

#include <boost/pool/pool.hpp>

// object_pool 是 pool 的子类，protectd 继承，因此不能使用 pool 的接口
#include <boost/pool/object_pool.hpp>

#include <boost/pool/singleton_pool.hpp>

// shared_ptr 应用于标准容器中
void t1_shared_ptr()
{
  typedef std::vector<boost::shared_ptr<int>> vs;
  vs v(10);
  
  int i = 0;
  for (vs::iterator pos = v.begin(); pos != v.end(); ++pos) {
    (*pos) = boost::make_shared<int>(++i);
    std::cout << *(*pos) << ", ";
  }
  std::cout << std::endl;
  
  boost::shared_ptr<int> p = v[9];
  *p = 100;
  std::cout << *v[9] << std::endl;
}

// shared_ptr 应用于 桥接模式
// bridge 是一种结构型设计模式，它把类的具体实现细节对用户隐藏起来，以达到类之间的最小耦合关系。
// bridge 可以任意改变具体的实现而外界对此一无所知，也减小了源文件之间的编译依赖
class sample
{
private:
  class impl;  //不完整的内部类声明
  boost::shared_ptr<impl> p;
  
public:
  sample();
  void print();
};

class sample::impl  // 内部类的实现
{
public:
  void print() {
    std::cout << "impl print" << std::endl;
  }
};

sample::sample():p(new impl) {}

void sample::print()
{
  p->print();
}

void t2_shared_ptr() {
  sample s;
  s.print();
}

// shared_ptr 应用于 工厂模式
class abstract
{
public:
  virtual void f() = 0;
  virtual void g() = 0;
protected:
  virtual ~abstract() {} //abstract的析构函数，被定义为protected，意味着除了它自己和它的子类，其他任何对象都无权调用delete来删除它
};

class impl : public abstract
{
public:
  virtual void f() {
    std::cout << "class impl f()" << std::endl;
  }
  
  virtual void g() {
    std::cout << "class impl g()" << std::endl;
  }
};

boost::shared_ptr<abstract> create() {
  return boost::shared_ptr<abstract>(new impl);
}

void t3_shared_ptr() {
  boost::shared_ptr<abstract> p = create();
  p->f();
  p->g();
}

// shared_ptr 定制删除器  shared_ptr(Y *p, D d)
// 析构时 delete p 换成 delete(p)
// 删除器d可以是一个函数对象，也可以是一个函数指针，只要它能够像函数那样被调用。可拷贝、行为像delete那样、不能抛出异常
// get_deleter<shard_ptr<T> const & p> 返回删除器的指针
// 作用：实现管理任意资源，只要这种资源提供了它自己的释放操作，shared_ptr就能够保证自动释放



// weak_ptr 协助 shared_ptr
void t4_weak_ptr() {
  boost::shared_ptr<int> sp(new int(10));
  assert(sp.use_count() == 1);
  
  boost::weak_ptr<int> wp(sp);
  assert(wp.use_count() == 1);
  
  if (!wp.expired()) {
    boost::shared_ptr<int> sp2 = wp.lock();
    *sp2 = 100;
    assert(wp.use_count() == 2);
  }
  
  assert(wp.use_count() == 1);
  sp.reset();
  assert(wp.expired());
  assert(!wp.lock());
}



int main(int argc, const char * argv[]) {
//  t1_shared_ptr();
//  t2_shared_ptr();
//  t3_shared_ptr();
  t4_weak_ptr();
  return 0;
}
