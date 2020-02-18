#include <fstream>
#include <iostream>

#include <boost/optional.hpp>
#include <boost/program_options.hpp>

#include <olp/authentication/TokenProvider.h>
#include <olp/core/client/HRN.h>

#include <olp/dataservice/write/StreamLayerClient.h>
#include <olp/dataservice/write/model/PublishDataRequest.h>

using namespace olp::dataservice::write;
using namespace olp::dataservice::write::model;
using namespace olp::client;


int main(int argc, char** argv) {

  std::cout << "-------------------\n";
  std::cout << "Welcome to Booma!!!\n";
  std::cout << "-------------------\n";

  olp::authentication::Settings settings;
  olp::client::OlpClientSettings clientSettings; 
  //olp::authentication::Settings settings;
  
  std::cout << "Successfully accessed objects from OLP-EDGE-SDK";
  return 0;
}
