#include <iostream>
#include "server_conf.h"
#include "monitor_conf.h"


int main() {
  server_conf sc("server.yaml");
  server_conf_data scd = sc.get_server_data();
  std::cout << scd.retries << std::endl;
  update_server_conf_data uscd = {std::string("sn"), std::string("HP123456")};
  sc.edit(&uscd);
  uscd.name = std::string("proxyport");
  uscd.value = std::string("32000");
  sc.edit(&uscd);
  sc.save();
  monitor_conf mc("monitor.yaml");
  std::vector<monitor_conf_data> mcd_vec = mc.get_monitor_data();
  std::cout << mcd_vec[0].name << std::endl;
  monitor_conf_data mcd = {"sysload", 22, 7};
  mc.edit(&mcd);
  //monitor_conf_data mcd1 = {"sysload2", 23, 4};
  //mc.add(&mcd1);
  monitor_conf_data mcd2 = {"sysload2", 23, 4};
  mc.del(&mcd2);
  mc.save();
  return 0;
  
}
