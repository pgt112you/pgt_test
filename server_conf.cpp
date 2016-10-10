#include "server_conf.h"


server_conf::server_conf(const char *filename) {
  config = YAML::LoadFile(filename);
  if (config.size() == 0) {  
    succ = -1; 
  }
  else {
    succ = 0;
	fname = filename;
    number = config.size();
    data = get_server_data();
  }
}

int server_conf::get_succ() {
  return succ;
}

int server_conf::get_number() {
  return number;
}

server_conf_data server_conf::get_server_data() {
  server_conf_data serv_conf;
  // sn
  if (config["sn"])
    serv_conf.sn = config["sn"].as<std::string>();
  else 
    serv_conf.sn = "";
  // proxyaddr
  if (config["proxyaddr"]) 
    serv_conf.proxyaddr = config["proxyaddr"].as<std::string>();
  else
    serv_conf.proxyaddr = "";
  // proxyport
  if (config["proxyport"]) 
    serv_conf.proxyport = config["proxyport"].as<int>();
  else
    serv_conf.proxyport = 0;
  // proxyaddr1
  if (config["proxyaddr1"]) 
    serv_conf.proxyaddr1 = config["proxyaddr1"].as<std::string>();
  else
    serv_conf.proxyaddr1 = "";
  // proxyport
  if (config["proxyport1"]) 
    serv_conf.proxyport1 = config["proxyport1"].as<int>();
  else
    serv_conf.proxyport1 = 0;
  // retries
  if (config["retries"]) 
    serv_conf.retries = config["retries"].as<int>();
  else
    serv_conf.retries = 0;
  // batchsend_frequency
  if (config["batchsend_frequency"]) 
    serv_conf.batchsend_frequency = config["batchsend_frequency"].as<int>();
  else
    serv_conf.batchsend_frequency = 3600;
  // thread_num
  if (config["thread_num"]) 
    serv_conf.thread_num = config["thread_num"].as<int>();
  else
    serv_conf.thread_num = 3600;
  // heartbeat frequency
  if (config["heartbeat_frequency"])
    serv_conf.heartbeat_frequency = config["heartbeat_frequency"].as<int>();
  else
    serv_conf.heartbeat_frequency = 3600;

  return serv_conf;
}

int server_conf::save() {
  //if (p_scd == NULL) {
  //  // NULL point
  //  return -1;
  //}
  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Comment("NOTE: Mappings use a colon and space (\": \") to mark each key: value pair.");
  out << YAML::Newline << YAML::Newline;
  out << YAML::Comment("sn number. unique identifier for this machine.");
  out << YAML::Key << "sn";
  out << YAML::Value << data.sn;
  out << YAML::Newline << YAML::Newline;
  out << YAML::Comment("proxy server ip address");
  out << YAML::Key << "proxyaddr";
  out << YAML::Value << data.proxyaddr;
  out << YAML::Key << "proxyport";
  out << YAML::Value << data.proxyport;
  out << YAML::Newline << YAML::Newline;
  out << YAML::Comment("reserve proxy server ip address");
  out << YAML::Key << "proxyaddr1";
  out << YAML::Value << data.proxyaddr1;
  out << YAML::Key << "proxyport1";
  out << YAML::Value << data.proxyport1;
  out << YAML::Newline << YAML::Newline;
  out << YAML::Comment("retry times send data to proxy server");
  out << YAML::Key << "retries";
  out << YAML::Value << data.retries;
  out << YAML::Newline << YAML::Newline;
  out << YAML::Comment("batch send frquency");
  out << YAML::Key << "batchsend_frequency";
  out << YAML::Value << data.batchsend_frequency;
  out << YAML::Newline << YAML::Newline;
  out << YAML::Comment("thread number of thread pool");
  out << YAML::Key << "thread_num";
  out << YAML::Value << data.thread_num;
  out << YAML::Newline << YAML::Newline;
  out << YAML::Key << "heartbeat frequency";
  out << YAML::Value << data.heartbeat_frequency;
  out << YAML::EndMap;
  std::ofstream fout(fname);
  fout << out.c_str();
  fout.close();
  return 0;
}

int server_conf::edit(update_server_conf_data *p_uscd) {
  if (p_uscd == NULL) {
    // NULL point
    return -1;
  }
  if (p_uscd->name.compare("sn") == 0) {
    data.sn = p_uscd->value;
  }
  else if (p_uscd->name.compare("proxyaddr") == 0) {  
    data.proxyaddr = p_uscd->value;
  }
  else if (p_uscd->name.compare("proxyport") == 0) {  
    data.proxyport = atoi(p_uscd->value.c_str());
  }
  else if (p_uscd->name.compare("proxyaddr1") == 0) {  
    data.proxyaddr1 = p_uscd->value;
  }
  else if (p_uscd->name.compare("proxyport1") == 0) {  
    data.proxyport1 = atoi(p_uscd->value.c_str());
  }
  else if (p_uscd->name.compare("retries") == 0) {  
    data.retries = atoi(p_uscd->value.c_str());
  }
  else if (p_uscd->name.compare("batchsend_frequency") == 0) {  
    data.batchsend_frequency = atoi(p_uscd->value.c_str());
  }
  else if (p_uscd->name.compare("thread_num") == 0) {  
    data.thread_num = atoi(p_uscd->value.c_str());
  }
  else if (p_uscd->name.compare("heartbeat_frequency") == 0) {  
    data.heartbeat_frequency = atoi(p_uscd->value.c_str());
  }
  else {
    // not a existed server configuration item.
    return -1;
  }

  return 0;

}
