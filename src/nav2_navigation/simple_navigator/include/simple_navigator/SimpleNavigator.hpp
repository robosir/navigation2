// Copyright (c) 2018 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SIMPLE_NAVIGATOR__SIMPLENAVIGATOR_HPP_
#define SIMPLE_NAVIGATOR__SIMPLENAVIGATOR_HPP_

#include <string>
#include <memory>
#include "nav2_tasks/NavigateToPoseTaskServer.hpp"
#include "nav2_tasks/ComputePathToPoseTaskClient.hpp"
#include "nav2_tasks/FollowPathTaskClient.hpp"

namespace simple_navigator
{

class SimpleNavigator : public nav2_tasks::NavigateToPoseTaskServer
{
public:
  explicit SimpleNavigator(const std::string & name);
  SimpleNavigator() = delete;
  ~SimpleNavigator();

  nav2_tasks::TaskStatus executeAsync(const nav2_tasks::NavigateToPoseCommand::SharedPtr command);

protected:
  std::unique_ptr<nav2_tasks::ComputePathToPoseTaskClient> planner_;
  std::unique_ptr<nav2_tasks::FollowPathTaskClient> controller_;
};

}  // namespace simple_navigator

#endif  // SIMPLE_NAVIGATOR__SIMPLENAVIGATOR_HPP_