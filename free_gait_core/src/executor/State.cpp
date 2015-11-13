/*
 * State.cpp
 *
 *  Created on: Oct 22, 2015
 *      Author: Péter Fankhauser
 *   Institute: ETH Zurich, Autonomous Systems Lab
 */
#include <free_gait_core/executor/State.hpp>
#include <quadruped_model/common/topology_conversions.hpp>

namespace free_gait {

State::State()
    : QuadrupedState(),
      robotExecutionStatus_(false)
{
}

State::~State()
{
}

void State::initialize(const std::vector<LimbEnum>& limbs, const std::vector<BranchEnum>& branches)
{
  for (const auto& limb : limbs) {
    isSupportLegs_[limb] = false;
    ignoreContact_[limb] = false;
    ignoreForPoseAdaptation_[limb] = false;
  }

  for (const auto& branch : branches) {
    setEmptyControlSetup(branch);
  }
}

bool State::getRobotExecutionStatus() const
{
  return robotExecutionStatus_;
}

void State::setRobotExecutionStatus(bool robotExecutionStatus)
{
  robotExecutionStatus_ = robotExecutionStatus;
}

bool State::isSupportLeg(const LimbEnum& limb) const
{
  return isSupportLegs_.at(limb);
}

void State::setSupportLeg(const LimbEnum& limb, bool isSupportLeg)
{
  isSupportLegs_[limb] = isSupportLeg;
}

unsigned int State::getNumberOfSupportLegs() const
{
  unsigned int nLegs = 0;
  for (const auto& supportLeg : isSupportLegs_) {
    if (supportLeg.second) ++nLegs;
  }
  return nLegs;
}

bool State::isIgnoreContact(const LimbEnum& limb) const
{
  return ignoreContact_.at(limb);
}

void State::setIgnoreContact(const LimbEnum& limb, bool ignoreContact)
{
  ignoreContact_[limb] = ignoreContact;
}

bool State::hasSurfaceNormal(const LimbEnum& limb) const
{
  return (surfaceNormals_.count(limb) > 0);
}

const Vector& State::getSurfaceNormal(const LimbEnum& limb) const
{
  return surfaceNormals_.at(limb);
}

void State::setSurfaceNormal(const LimbEnum& limb, const Vector& surfaceNormal)
{
  surfaceNormals_[limb] = surfaceNormal;
}

void State::removeSurfaceNormal(const LimbEnum& limb)
{
  surfaceNormals_.erase(limb);
}

bool State::isIgnoreForPoseAdaptation(const LimbEnum& limb) const
{
  return ignoreForPoseAdaptation_.at(limb);
}

void State::setIgnoreForPoseAdaptation(const LimbEnum& limb, bool ignorePoseAdaptation)
{
  ignoreForPoseAdaptation_[limb] = ignorePoseAdaptation;
}

const JointPositions State::getJointPositions(const LimbEnum& limb) const
{
  // TODO This is not nice.
  unsigned int startIndex = 3 * quadruped_model::getLimbUIntFromLimbEnum(limb);
  return JointPositions(quadruped_model::QuadrupedState::getJointPositions().vector().segment<3>(startIndex));
}

void State::setJointPositions(const LimbEnum& limb, const JointPositions& jointPositions)
{
  // TODO This is not nice.
  unsigned int startIndex = 3 * quadruped_model::getLimbUIntFromLimbEnum(limb);
  for (unsigned int i = 0; i < 3; ++i) {
    quadruped_model::QuadrupedState::getJointPositions()(startIndex + i) = jointPositions(i);
  }
}

void State::setAllJointPositions(const JointPositions& jointPositions)
{
  quadruped_model::QuadrupedState::setJointPositions(quadruped_model::JointPositions(jointPositions.vector()));
}

void State::setAllJointVelocities(const JointVelocities& jointVelocities)
{
  quadruped_model::QuadrupedState::setJointVelocities(quadruped_model::JointVelocities(jointVelocities.vector()));
}

const ControlSetup& State::getControlSetup(const BranchEnum& branch) const
{
  return controlSetups_.at(branch);
}

const ControlSetup& State::getControlSetup(const LimbEnum& limb) const
{
  return controlSetups_.at(quadruped_model::getBranchEnumFromLimbEnum(limb));
}

void State::setControlSetup(const BranchEnum& branch, const ControlSetup& controlSetup)
{
  controlSetups_[branch] = controlSetup;
}

void State::setControlSetup(const LimbEnum& limb, const ControlSetup& controlSetup)
{
  controlSetups_[quadruped_model::getBranchEnumFromLimbEnum(limb)] = controlSetup;
}

void State::setEmptyControlSetup(const BranchEnum& branch)
{
  ControlSetup emptyControlSetup;
  emptyControlSetup[ControlLevel::Position] = false;
  emptyControlSetup[ControlLevel::Velocity] = false;
  emptyControlSetup[ControlLevel::Acceleration] = false;
  emptyControlSetup[ControlLevel::Effort] = false;
  controlSetups_[branch] = emptyControlSetup;
}

void State::setEmptyControlSetup(const LimbEnum& limb)
{
  setEmptyControlSetup(quadruped_model::getBranchEnumFromLimbEnum(limb));
}

} /* namespace free_gait */

