#ifndef __OBJECT_COUNTER__
#define __OBJECT_COUNTER__

#include <mutex>
#include <map>
#include <string>

class ObjectCounter
{
public:
  enum
  {
    MockupCiphertext = 1,
    SealCiphertext = 2
  };

private:
  static std::mutex m;
  static std::map<int, int> count;
  static time_t lastPrint;

public:
  static std::string toString(int s)
  {
    switch (s) {
    case MockupCiphertext:
      return "MockupCiphertext";
    case SealCiphertext:
      return "SealCiphertext";
    default:
      throw std::runtime_error("Unknown object code " + std::to_string(s));
    }
  }

  static void add(int s, void* p = nullptr)
  {
    bool print = false;
    {
      std::lock_guard<std::mutex> g(m);
      // std::cout << "Adding " << p << std::endl;
      auto i = count.find(s);
      if (i == count.end())
        count.insert({s, 1});
      else
        ++i->second;
      if (lastPrint + 1 < time(nullptr)) {
        lastPrint = time(nullptr);
        print = true;
      }
    }
    if (print) {
      std::cout << "Dumping memory status after " << lastPrint << " microsec"
                << std::endl;
      dump();
    }
  }

  static void del(int s, void* p = nullptr)
  {
    bool print = false;
    {
      std::lock_guard<std::mutex> g(m);
      // std::cout << "Deleting " << p << std::endl;
      auto i = count.find(s);
      if (i == count.end())
        throw std::runtime_error("Decreasing ObjectCount of " + toString(s) +
                                 " which is not tracked.");
      else if (--i->second < 0)
        throw std::runtime_error("After decreasing ObjectCount of " +
                                 toString(s) + " the count is negative.");
      if (lastPrint + 1 < time(nullptr)) {
        lastPrint = time(nullptr);
        print = true;
      }
    }

    if (print) {
      std::cout << "Dumping memory status after " << lastPrint << " microsec"
                << std::endl;
      dump();
    }
  }

  static void dump()
  {
    std::lock_guard<std::mutex> g(m);
    for (auto i : count) {
      std::cout << toString(i.first) << " has " << i.second
                << " instances currently allocated" << std::endl;
    }
  }
};

template <int S>
class ObjectCounterGuard
{
public:
  ObjectCounterGuard(const ObjectCounterGuard&) { ObjectCounter::add(S, this); }
  ObjectCounterGuard() { ObjectCounter::add(S, this); }
  ~ObjectCounterGuard() { ObjectCounter::del(S, this); }
};

#endif