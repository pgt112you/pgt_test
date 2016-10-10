#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "yaml-cpp/yaml.h"


#ifndef AGENT_MONITOR_CONF_H_
#define AGENT_MONITOR_CONF_H_

typedef struct {
  std::string name;
  int frenc;
  int timeout;
} monitor_conf_data;
  

// monitor configuration class
// initial monitor configuration object like
//    monitor_conf mf("monitor.yaml")
// get monitor configuration like
//    std::vector<monitor_conf> mfd = mf.get_monitor_data()
// set monitor configuration like
//    mf.set_monitor_data()
class monitor_conf {
 public:
  explicit monitor_conf(const char *filename);
  std::vector<monitor_conf_data> get_monitor_data();
  int get_succ();
  int get_number();
  int save();
  int edit(monitor_conf_data *p_mcd);
  int add(monitor_conf_data *p_mcd);
  int del(monitor_conf_data *p_mcd);

 private:
  // configuration file name
  const char *fname;
  YAML::Node config;
  // succ mark
  // succ 0 is initialize monitor_conf object success 
  // succ -1 is configuration file is not existed or file is empty
  int succ;  
  // monitor number
  int number;
  std::vector<monitor_conf_data> data;
};


#endif
