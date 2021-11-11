#pragma once

#include "DebugLevel.hpp"

#include "../../../BuildSetting.inl"

#include "CompilerDirective.hpp"
#include "DebugCalibration.hpp"
#include "EnvironmentDefinition.hpp"

#include "BuildinType.hpp"

#include "Operation/Assert.hpp"
#include "Operation/Atom.hpp"
#include "Operation/Basic.hpp"
#include "Operation/CString.hpp"
#include "Operation/CStringConversion.hpp"
#include "Operation/Date.hpp"
#include "Operation/Directory.hpp"
#include "Operation/Event.hpp"
#include "Operation/File.hpp"
#include "Operation/Lock.hpp"
#include "Operation/Math.hpp"
#include "Operation/Memory.hpp"
#include "Operation/Network.hpp"
#include "Operation/New.hpp"
#include "Operation/Print.hpp"
#include "Operation/Spin.hpp"
#include "Operation/Thread.hpp"
#include "Operation/Timer.hpp"

#include "Reference/Context.hpp"
#include "Reference/Ref.hpp"
#include "Reference/Obs.hpp"


// inline implementation

#include "Operation/Atom_Implementation.inl"
#include "Operation/CString_Implementation.inl"
#include "Operation/CStringConversion_Implementation.inl"
#include "Operation/Lock_Implementation.inl"
#include "Operation/Math_Implementation.inl"
#include "Operation/Network_Implementation.inl"
#include "Operation/Spin_Implementation.inl"
#include "Operation/Thread_Implementation.inl"

#include "Reference/Context_Implementation.inl"
#include "Reference/Ref_Implementation.inl"
#include "Reference/Obs_Implementation.inl"
