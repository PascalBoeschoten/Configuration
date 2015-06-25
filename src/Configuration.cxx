/// \file Configuration.h
/// \brief C++ interface to retrieve configuration parameters.
///
/// \author Sylvain Chapeland, CERN

#include <Configuration.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>
#include <vector>
#include <boost/algorithm/string/predicate.hpp>

using namespace std;

class configFilePrivate {
  public:
  configFilePrivate();
  ~configFilePrivate();
  
  friend class configFile;
  protected:
    boost::property_tree::ptree pt;    
    
};

configFilePrivate::configFilePrivate(){
}

configFilePrivate::~configFilePrivate(){
}


configFile::configFile(){
  dPtr=new configFilePrivate();
  if (dPtr==NULL) {
    std::stringstream ss;
    ss << "new() error in " << __FILE__ << " line " << __LINE__;
    throw ss.str();
  }
}

configFile::~configFile(){
  if (dPtr!=NULL) {
    delete dPtr;
  }
}

#define PREFIX_FILE "file:"
#define SUFFIX_FILE_INI {".ini", ".cfg"}

void configFile::load(const char *path) {
  if (path==NULL) throw std::string("Invalid argument");

  //
  // open location according to prefix
  //
  
  
  // filesystem file
  if (boost::algorithm::starts_with(path, PREFIX_FILE)) {
    const char *filename;
    filename=&path[strlen(PREFIX_FILE)];
    
    // TODO: filter out comments in file with boost filtering_stream
    
    //
    // parse file according to suffix
    //
    
    // INI file
    std::vector <const char*>suffix_ini SUFFIX_FILE_INI;
    for ( auto suffix : suffix_ini ) {
      if (boost::algorithm::ends_with(filename, suffix)) {
        try {
          boost::property_tree::ini_parser::read_ini(filename, dPtr->pt);
        }
        catch (boost::property_tree::ini_parser::ini_parser_error perr) {    
          std::stringstream ss;
          if (perr.line()) {
            ss << perr.message() << " in " << perr.filename() << " line " << perr.line();
          } else {
            ss << perr.message() << " " << perr.filename();
          }
          throw ss.str();    
        }
        return;
      }
    }
    throw std::string("Invalid type in file name");
  } else {
    throw std::string("Invalid path prefix");
  }
}
  
void configFile::getValue(const char *key,std::string &value){
  try {
    value=dPtr->pt.get<std::string>(key);
  }
  catch(const boost::property_tree::ptree_error &e) {
    throw std::string(e.what());
  }
}

void configFile::getValue(const char *key, int &value){
  try {
    value=dPtr->pt.get<int>(key);
  }
  catch(const boost::property_tree::ptree_error &e) {
    throw std::string(e.what());
  }
}

void configFile::getValue(const char *key, float &value){
  try {
    value=dPtr->pt.get<float>(key);
  }
  catch(const boost::property_tree::ptree_error &e) {
    throw std::string(e.what());
  }
}