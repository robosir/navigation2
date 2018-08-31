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

#include <string>
#include <memory>
#include <exception>
#include <chrono>
#include "nav2_bt_navigator/bt_navigator.hpp"

using namespace std::chrono_literals;
using nav2_tasks::TaskStatus;

namespace nav2_bt_navigator
{

BtNavigator::BtNavigator(const std::string & name)
: nav2_tasks::NavigateToPoseTaskServer(name)
{
  RCLCPP_INFO(get_logger(), "BtNavigator::BtNavigator");
  planner_ = std::make_unique<nav2_tasks::ComputePathToPoseTaskClient>("AStarPlanner", this);
  controller_ = std::make_unique<nav2_tasks::FollowPathTaskClient>("DwaController", this);
}

BtNavigator::~BtNavigator()
{
  RCLCPP_INFO(get_logger(), "BtNavigator::~BtNavigator");
}

TaskStatus
BtNavigator::execute(const nav2_tasks::NavigateToPoseCommand::SharedPtr command)
{
  RCLCPP_INFO(get_logger(), "BtNavigator::execute");

  // Compose the PathEndPoints message for Navigation
  auto endpoints = std::make_shared<nav2_tasks::ComputePathToPoseCommand>();
  // TODO(mjeronimo): get the starting pose from Localization (fake it out for now)
  endpoints->start = command->pose;
  endpoints->goal = command->pose;

  RCLCPP_INFO(get_logger(), "BtNavigator::execute: getting the path from the planner");
  auto path = std::make_shared<nav2_tasks::ComputePathToPoseResult>();
  planner_->executeAsync(endpoints);

  // Loop until the subtasks are completed
  for (;; ) {
    // Check to see if this task (navigation) has been canceled. If so, cancel any child
    // tasks and then cancel this task
    if (cancelRequested()) {
      RCLCPP_INFO(get_logger(), "BtNavigator::execute: task has been canceled");
      planner_->cancel();
      setCanceled();
      return TaskStatus::CANCELED;
    }

    // Check if the planning task has completed
    TaskStatus status = planner_->waitForResult(path, 100ms);

    switch (status) {
      case TaskStatus::SUCCEEDED:
        RCLCPP_INFO(get_logger(), "BtNavigator::execute: planning task completed");
        goto here;

      case TaskStatus::FAILED:
        return TaskStatus::FAILED;

      case TaskStatus::RUNNING:
        RCLCPP_INFO(get_logger(), "BtNavigator::execute: planning task still running");
        break;

      default:
        RCLCPP_ERROR(get_logger(), "BtNavigator::executeAsync: invalid status value");
        throw std::logic_error("BtNavigator::executeAsync: invalid status value");
    }
  }

here:
  RCLCPP_INFO(get_logger(),
    "BtNavigator::execute: sending the path to the controller to execute");

  controller_->executeAsync(path);

  // Loop until the control task completes
  for (;; ) {
    // Check to see if this task (navigation) has been canceled. If so, cancel any child
    // tasks and then cancel this task
    if (cancelRequested()) {
      RCLCPP_INFO(get_logger(), "BtNavigator::execute: task has been canceled");
      controller_->cancel();
      setCanceled();
      return TaskStatus::CANCELED;
    }

    // Check if the control task has completed
    auto controlResult = std::make_shared<nav2_tasks::FollowPathResult>();
    TaskStatus status = controller_->waitForResult(controlResult, 10ms);

    switch (status) {
      case TaskStatus::SUCCEEDED:
        {
          RCLCPP_INFO(get_logger(), "BtNavigator::execute: control task completed");
          nav2_tasks::NavigateToPoseResult navigationResult;
          setResult(navigationResult);

          return TaskStatus::SUCCEEDED;
        }

      case TaskStatus::FAILED:
        return TaskStatus::FAILED;

      case TaskStatus::RUNNING:
        RCLCPP_INFO(get_logger(), "BtNavigator::execute: control task still running");
        break;

      default:
        RCLCPP_ERROR(get_logger(), "BtNavigator::executeAsync: invalid status value");
        throw std::logic_error("BtNavigator::executeAsync: invalid status value");
    }
  }
}

}  // namespace nav2_bt_navigator