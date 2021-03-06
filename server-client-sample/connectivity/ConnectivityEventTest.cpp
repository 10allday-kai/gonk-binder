/* (c) 2020 KAI OS TECHNOLOGIES (HONG KONG) LIMITED All rights reserved. This
 * file or any portion thereof may not be reproduced or used in any manner
 * whatsoever without the express written permission of KAI OS TECHNOLOGIES
 * (HONG KONG) LIMITED. KaiOS is the trademark of KAI OS TECHNOLOGIES (HONG
 * KONG) LIMITED or its affiliate company and may be registered in some
 * jurisdictions. All other trademarks are the property of their respective
 * owners.
 */

#include <binder/IServiceManager.h>
#include "ConnectivityEventTest.h"
#include <b2g/connectivity/IConnectivity.h>

#define KAIOS_CON_DEBUG(args...) \
  __android_log_print(ANDROID_LOG_INFO, "KaiOS_AIDL_Connectivity", ##args)

using android::IBinder;
using android::IServiceManager;
using android::sp;
using android::binder::Status;
using b2g::connectivity::IConnectivity;
using b2g::connectivity::NetworkInfoParcel;

void msleep(unsigned int ms) {
  int microsecs;
  struct timeval tv;
  microsecs = ms * 1000;
  tv.tv_sec = microsecs / 1000000;
  tv.tv_usec = microsecs % 1000000;
  select(0, NULL, NULL, NULL, &tv);
}

ConnectivityEventTest::ConnectivityEventTest() {
  android::defaultServiceManager()->addService(
      android::String16(ConnectivityEventTest::getServiceName()), this, false,
      android::IServiceManager::DUMP_FLAG_PRIORITY_DEFAULT);
  android::sp<android::ProcessState> ps(android::ProcessState::self());
  ps->startThreadPool();
}

Status ConnectivityEventTest::onActiveNetworkChanged(
    const NetworkInfoParcel& networkInfo) {
  KAIOS_CON_DEBUG("onActiveNetworkChanged event.");
  for (uint32_t i = 0; i < networkInfo.gateways.size(); i++) {
    KAIOS_CON_DEBUG("Dump gateway[%d] : %s", i,
                    networkInfo.gateways[i].c_str());
  }
  KAIOS_CON_DEBUG("Let's try block 5 seconds in event.");
  msleep(5000);
  KAIOS_CON_DEBUG("onActiveNetworkChanged event done.");
  return Status::ok();
};

Status ConnectivityEventTest::onNetworkChanged(
    const NetworkInfoParcel& networkInfo) {
  KAIOS_CON_DEBUG("onNetworkChanged event.");

  // Let's try get active networkinfo directly.
  sp<IServiceManager> sm = android::defaultServiceManager();
  sp<IConnectivity> sConnectivity = nullptr;
  sp<IBinder> binderConnectivity;
  binderConnectivity = sm->getService(IConnectivity::SERVICE_NAME());

  if (binderConnectivity != nullptr) {
    sConnectivity = android::interface_cast<IConnectivity>(binderConnectivity);
    NetworkInfoParcel activeNetworkInfo;
    Status activeNetworkInfoState =
        sConnectivity->getActiveNetworkInfo(&activeNetworkInfo);
    if (activeNetworkInfoState.isOk()) {
      KAIOS_CON_DEBUG("Current active networkinfo name:%s type:%d state:%d",
                      activeNetworkInfo.name.c_str(), activeNetworkInfo.type,
                      activeNetworkInfo.state);
    } else {
      KAIOS_CON_DEBUG("Failed to get active networkinfo.");
    }

    // Get all network info.
    std::vector<NetworkInfoParcel> networkInfos;
    Status networkInfoStatus = sConnectivity->getNetworkInfos(&networkInfos);
    if (networkInfoStatus.isOk()) {
      KAIOS_CON_DEBUG("Dump all networkinfos");
      for (uint32_t i = 0; i < networkInfos.size(); i++) {
        KAIOS_CON_DEBUG("NetworkInfo name: %s, type: %d: state: %d",
                        networkInfos[i].name.c_str(), networkInfos[i].type,
                        networkInfos[i].state);
      }
    } else {
      KAIOS_CON_DEBUG("Failed to get networkinfo list");
    }
  }
  return Status::ok();
};
