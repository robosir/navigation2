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

#ifndef ASTAR_PLANNER__ASTARPLANNER_HPP_
#define ASTAR_PLANNER__ASTARPLANNER_HPP_

#include <string>
#include "nav2_tasks/ComputePathToPoseTaskServer.hpp"

namespace astar_planner
{

class AStarPlanner : public nav2_tasks::ComputePathToPoseTaskServer
{
public:
  explicit AStarPlanner(const std::string & name);
  AStarPlanner() = delete;
  ~AStarPlanner();

  nav2_tasks::TaskStatus executeAsync(
    const nav2_tasks::ComputePathToPoseCommand::SharedPtr command) override;
};

}  // namespace astar_planner

#endif  // ASTAR_PLANNER__ASTARPLANNER_HPP_