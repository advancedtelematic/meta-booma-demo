/*
 * Copyright (C) 2019 HERE Global B.V. and its affiliate(s).
 * All rights reserved.
 *
 * This software and other materials contain proprietary information
 * controlled by HERE and are protected by applicable copyright legislation.
 * Any use and utilization of this software and other materials and
 * disclosure to any third parties is conditional upon having a separate
 * agreement with HERE for the access, use, utilization or disclosure of this
 * software. In the absence of such agreement, the use of the software is not
 * allowed.
 */

#include "example.h"

#include <fstream>
#include <iostream>

#include <boost/optional.hpp>
#include <boost/program_options.hpp>

#include <olp/authentication/TokenProvider.h>
#include <olp/core/cache/CacheSettings.h>
#include <olp/core/cache/KeyValueCache.h>
#include <olp/core/client/HRN.h>
#include <olp/core/client/OlpClientSettings.h>
#include <olp/core/client/OlpClientSettingsFactory.h>

#include <olp/dataservice/write/StreamLayerClient.h>
#include <olp/dataservice/write/model/PublishDataRequest.h>

using namespace olp::dataservice::write;
using namespace olp::dataservice::write::model;

namespace {
// std::string gEndpoint;
std::string gAppid;
std::string gSecret;
std::string gCatalog;
std::string gLayer;
std::string gFile;

int parseProgramOptions(int argc, const char *const *argv) {
  namespace po = boost::program_options;

  po::options_description desc("Required Parameters");

  // clang-format off
        desc.add_options()
        ("appid", po::value<std::string>(), "App id")
        ("secret", po::value<std::string>(), "App secret")
        ("catalog", po::value<std::string>(), "Catalog to write to")
        ("layer", po::value<std::string>(), "Layer ID to write to")
        // TODO remove content-type as an option here once test script is updated to remove it. Currently it is not used.
        ("content-type", po::value<std::string>(), "Content Type of the Layer")
        ("file", po::value<std::string>(), "Fill path to file containing data to write");
  // clang-format on

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    return -1;
  }
  if (!vm.count("appid") || !vm.count("secret") ||
      !vm.count("catalog") || !vm.count("layer") || !vm.count("file")) {
    std::cout << desc << std::endl;
    return -1;
  }

  gAppid = vm["appid"].as<std::string>();
  gSecret = vm["secret"].as<std::string>();
  gCatalog = vm["catalog"].as<std::string>();
  gLayer = vm["layer"].as<std::string>();
  gFile = vm["file"].as<std::string>();

  return 0;
}

std::shared_ptr<StreamLayerClient> createStreamLayerClient() {
  // Create a task scheduler instance
  std::shared_ptr<olp::thread::TaskScheduler> task_scheduler =
      olp::client::OlpClientSettingsFactory::CreateDefaultTaskScheduler(1u);

  // Create a network client
  std::shared_ptr<olp::http::Network> http_client = olp::client::
      OlpClientSettingsFactory::CreateDefaultNetworkRequestHandler();

  olp::authentication::Settings settings({gAppid, gSecret});
  settings.task_scheduler = task_scheduler;
  settings.network_request_handler = http_client;

  olp::client::AuthenticationSettings auth_settings;
  auth_settings.provider =
      olp::authentication::TokenProviderDefault(std::move(settings));

  olp::client::OlpClientSettings client_settings;
  client_settings.authentication_settings = auth_settings;
  client_settings.task_scheduler = std::move(task_scheduler);
  client_settings.network_request_handler = std::move(http_client);
  client_settings.cache =
      olp::client::OlpClientSettingsFactory::CreateDefaultCache({});


  return std::make_shared<StreamLayerClient>(
      olp::client::HRN{gCatalog}, StreamLayerClientSettings(), client_settings);
}
} // namespace

int runExample(int argc, const char *const *argv) {

  std::cout << "-------------------\n";
  std::cout << "Welcome to Booma!!!\n";
  std::cout << "-------------------\n";

  auto outcome = parseProgramOptions(argc, argv);
  if (outcome < 0) {
    return outcome;
  }

  // Read data to write from file path
  std::ifstream fileStream(gFile.c_str(), std::ios::in | std::ios::binary);
  if (!fileStream) {
    std::cout << "Error reading file at path " << gFile << std::endl;
    return -1;
  }
  auto buffer = std::make_shared<std::vector<unsigned char>>(
      (std::istreambuf_iterator<char>(fileStream)),
      std::istreambuf_iterator<char>());

  auto client = createStreamLayerClient();

  // Write data to OLP Stream Layer using StreamLayerClient
  auto response =
      client
          ->PublishData(
              PublishDataRequest().WithData(buffer).WithLayerId(gLayer))
          .GetFuture()
          .get();

  if (!response.IsSuccessful()) {
    std::cout << "Error writing data - HTTP Status: "
              << response.GetError().GetHttpStatusCode()
              << " Message: " << response.GetError().GetMessage() << std::endl;
    return -1;
  } else {
    std::cout << "Publish Successful - TraceID: "
              << response.GetResult().GetTraceID() << std::endl;
  }

  return 0;
}
