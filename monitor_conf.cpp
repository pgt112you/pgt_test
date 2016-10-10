#include "monitor_conf.h"


monitor_conf::monitor_conf(const char *filename) {
  config = YAML::LoadFile(filename);
  if (config.size() == 0) {
    succ = -1;
  }
  else { 
    succ = 0;
	fname = filename;
	number = config.size();
    data = get_monitor_data();
  }
}

int monitor_conf::get_succ() {
  return succ;
}

int monitor_conf::get_number() {
  return number;
}

std::vector<monitor_conf_data> monitor_conf::get_monitor_data() {
  std::vector<monitor_conf_data> moni_conf;
  //config = YAML::LoadFile(fname);
  for (std::size_t i=0; i<config.size(); ++i) {
    monitor_conf_data temp_conf;
    if (config[i]["name"])
      temp_conf.name = config[i]["name"].as<std::string>();
    else
      temp_conf.name = std::string("");
    if (config[i]["timeout"])
      temp_conf.timeout = config[i]["timeout"].as<int>();
    else
      temp_conf.timeout = 0;
    if (config[i]["frenc"])
      temp_conf.frenc = config[i]["frenc"].as<int>();
    else
      temp_conf.frenc = 0;
    moni_conf.push_back(temp_conf);
  }
  return moni_conf;
}

int monitor_conf::save() {
  YAML::Emitter out;
  out << YAML::Comment("NOTE: Block sequences indicate each entry with a dash and space (\"- \").");
  out << YAML::Newline << YAML::Newline; 
  out << YAML::Comment("Mappings use a colon and space (\": \") to mark each key: value pair.");
  out << YAML::Newline << YAML::Newline; 
  out << YAML::BeginSeq;
  for (std::vector<monitor_conf_data>::size_type ix=0; ix!=data.size(); ++ix) {
    if (data[ix].name.compare("syscpuidle") == 0) {
      out << YAML::Newline;
      out << YAML::Comment("syscpuidle timeout should greater than or equal 6 seconds");
    }
    else if (data[ix].name.compare("sysifstat") == 0) {
      out << YAML::Newline;
      out << YAML::Comment("sysifstat timeout should greater than or equal 6 seconds");
    }
    else if (data[ix].name.compare("sysiops") == 0) {
      out << YAML::Newline;
      out << YAML::Comment("iops timeout should greater than or equal 6 seconds");
    }
    else {
      ;
    }
    out << YAML::BeginMap;
    out << YAML::Key << "name" << YAML::Value << data[ix].name;      
    out << YAML::Key << "frenc" << YAML::Value << data[ix].frenc;      
    out << YAML::Key << "timeout" << YAML::Value << data[ix].timeout;      
    out << YAML::EndMap;
    out << YAML::Newline;
  }
  out << YAML::EndSeq;
  std::ofstream fout(fname);
  fout << out.c_str();
  fout.close();
}

int monitor_conf::edit(monitor_conf_data *p_mcd) {
  if (p_mcd == NULL) {
    return -1;
  }
  
  for (std::vector<monitor_conf_data>::size_type ix=0; ix!=data.size(); ++ix) {
    if (data[ix].name.compare(p_mcd->name) == 0) {
      data[ix].frenc = p_mcd->frenc;
      data[ix].timeout = p_mcd->timeout;
    }
  }
  return 0;
}

int monitor_conf::add(monitor_conf_data *p_mcd) {
  if (p_mcd == NULL) {
    return -1;
  }
  data.push_back(*p_mcd);
  return 0;
}

int monitor_conf::del(monitor_conf_data *p_mcd) {
  if (p_mcd == NULL) {
    return -1;
  }
  std::vector<monitor_conf_data>::size_type pos = -1;
  for (std::vector<monitor_conf_data>::size_type ix=0; ix!=data.size(); ++ix) {
    if (data[ix].name.compare(p_mcd->name) == 0) {
      pos = ix;
    }
  }
  if (pos == -1) {
    return -1;
  }
  else {
    data.erase(data.begin()+pos);
    return 0;
  }
}
