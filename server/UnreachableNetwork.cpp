/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "Netd"

#include "UnreachableNetwork.h"

#include "RouteController.h"

namespace android {
namespace net {

// The unreachable network is used to reject traffic. It is used for system purposes only.
UnreachableNetwork::UnreachableNetwork(unsigned netId) : Network(netId) {}

int UnreachableNetwork::addUsers(const UidRanges& uidRanges) {
    if (hasInvalidUidRanges(uidRanges)) {
        return -EINVAL;
    }

    int ret = RouteController::addUsersToUnreachableNetwork(mNetId, uidRanges);
    if (ret) {
        ALOGE("failed to add users to unreachable network");
        return ret;
    }
    mUidRanges.add(uidRanges);
    return 0;
}

int UnreachableNetwork::removeUsers(const UidRanges& uidRanges) {
    int ret = RouteController::removeUsersFromUnreachableNetwork(mNetId, uidRanges);
    if (ret) {
        ALOGE("failed to remove users from unreachable network");
        return ret;
    }
    mUidRanges.remove(uidRanges);
    return 0;
}

Network::Type UnreachableNetwork::getType() const {
    return UNREACHABLE;
}

}  // namespace net
}  // namespace android
