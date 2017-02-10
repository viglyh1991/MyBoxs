//
//  main.cpp
//  LearnBoost
//
//  Created by zuzuche on 2017/2/10.
//  Copyright © 2017年 viglyh. All rights reserved.
//


# include <iostream>
#include <string>
#include <boost/timer.hpp>

// progress继承自timer, 精确到小数点后两位
#include <boost/progress.hpp>

#include <boost/static_assert.hpp>

#include <vector>

using namespace std;

void t1_timer() {
  
  boost::timer t; //声明一个计时器对象，开始计时
  
  cout << "max timespan:" << t.elapsed_max() / 3600 << "h" << endl;
  cout << "min timespan:" << t.elapsed_min() << "s" << endl;
  
  for(int i = 0; i < 10000; i ++) {
    for (int j = 0; j < 10000; j++) {
    }
  }
  
  
  cout << "now time elapsed" << t.elapsed() << "s" << endl;
}


void t2_progress_timer() {
  
  // 只需声明，通过析构函数来自动输出流逝的时间
  boost::progress_timer t;
  
  for(int i = 0; i < 10000; i ++) {
    for (int j = 0; j < 10000; j++) {
    }
  }
}

// 扩展 progress_timer, 表示更高的精度
//template <int N = 2>
//class new_progress_timer : public boost::timer {
//
//public:
//  new_progress_timer(std::ostream &os = std::cout) : m_os(os) {
//    BOOST_STATIC_ASSERT(N > 0 && N <= 10);
//  }_
//
//private:
//  std::ostream &m_os;
//};

void t3_progress_display() {
  
  boost::progress_display pd(100);
  
  for (int i = 0; i < 100; i++) {
    for(int j = 0; j < 10000; j++){
      for (int z = 0; z < 1000; z++) {
        
      }
    }
    ++pd;
  }
}

int main(void) {
  
  t3_progress_display();
  
  return 0;
}

