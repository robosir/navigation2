// License: Apache 2.0. See LICENSE file in root directory.
// Copyright 2018 Intel Corporation. All Rights Reserved.

#ifndef TASK__CONTROLTASK_HPP_
#define TASK__CONTROLTASK_HPP_

#include "task/RobotTask.hpp"

class ControlTask : public RobotTask
{
public:
  explicit ControlTask(Robot * robot);
  ControlTask() = delete;
  ~ControlTask();

  // executePlan()
};

#endif  // TASK__CONTROLTASK_HPP_