#include <ctime>
#include <chrono>
#include <thread>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <fstream>

using namespace std::chrono_literals;

class Saver{
private:
  std::string output_prefix;
  std::string time_fmt;
  std::string output_fname;
  std::ofstream of;
  std::mutex of_mutex;
  std::string line;
  char buffer_[100];
  void change_of()
  {
    while(true){
      std::time_t t = std::time(nullptr);
      std::tm tm = *std::localtime(&t);
      int n = std::strftime(buffer_, sizeof(buffer_), time_fmt.c_str(), &tm);
      std::string new_output_fname = output_prefix + "/" + buffer_;
      if(new_output_fname!=output_fname){
        std::lock_guard<std::mutex> guard(of_mutex);
        of.close();
        of.open(new_output_fname, std::ios_base::out|std::ios_base::app);
        output_fname = new_output_fname;
      }
      std::this_thread::sleep_for(1s);
    }
  }

  void write(){
    while(true){
      std::getline(std::cin, line);
      std::lock_guard<std::mutex> guard(of_mutex);
      auto t = std::time(nullptr);
      of << t << "\t" << line << "\n";
    }
  }

public:
  Saver(const std::string& output_prefix, const std::string& time_fmt)
  :output_prefix(output_prefix), time_fmt(time_fmt){}
  void run(){
    auto change_of_t = std::thread(&Saver::change_of, this);
    auto write_t = std::thread(&Saver::write, this);
    change_of_t.join();
    write_t.join();
  }
};

int main(int argc, char **argv)
{
  Saver(argv[1], argv[2]).run();
}