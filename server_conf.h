#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "yaml-cpp/yaml.h"


#ifndef AGENT_SERVER_CONF_H_
#define AGENT_SERVER_CONF_H_

typedef struct {
  std::string sn;
  std::string proxyaddr;
  int proxyport;
  std::string proxyaddr1;
  int proxyport1;
  int retries;
  int batchsend_frequency;
  int thread_num;
  int heartbeat_frequency;
} server_conf_data;

typedef struct {
  std::string name;
  std::string value;
} update_server_conf_data;

// server configuration class
// initial server configuration object like
//    server_conf mf("server.yaml")
// get server configuration like
//    std::vector<server_conf> mfd = mf.get_server_data()
// set server configuration like
//    mf.set_server_data()
class server_conf {
 public:
  //explicit server_conf(const char *filename):filename(filename){};
  explicit server_conf(const char *filename);
  server_conf_data get_server_data();
  int get_succ();
  int get_number();
  int save();
  int edit(update_server_conf_data *p_uscd);

 private:
  // configuraton file name
  const char *fname;
  YAML::Node config;
  // succ mark 
  // succ 0 is initialize monitor_conf object success
  // succ -1 is configuration file is not existed or file is empty
  int succ;
  int number;
  server_conf_data data;
};


#endif
