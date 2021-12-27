#pragma once

#include "DebugLevel.hpp"

#include "../../../BuildSetting.inl"
#include "../../../BuildDebugCalibration.inl"

#include "CompilerDirective.hpp"
#include "DebugCalibration.hpp"
#include "EnvironmentDefinition.hpp"

#include "BuildinType.hpp"
#include "TypeDeduction.hpp"

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

#include "Functional/Functional.hpp"

// inline implementation
#include "Operation/Assert_Implementation.inl"
#include "Operation/Atom_Implementation.inl"
#include "Operation/CString_Implementation.inl"
#include "Operation/CStringConversion_Implementation.inl"
#include "Operation/Date_Implementation.inl"
#include "Operation/Directory_Implementation.inl"
#include "Operation/File_Implementation.inl"
#include "Operation/Lock_Implementation.inl"
#include "Operation/Math_Implementation.inl"
#include "Operation/Memory_Implementation.inl"
#include "Operation/Network_Implementation.inl"
#include "Operation/New_Implementation.inl"
#include "Operation/Spin_Implementation.inl"
#include "Operation/Thread_Implementation.inl"
#include "Operation/Timer_Implementation.inl"

#include "Reference/Context_Implementation.inl"
#include "Reference/Ref_Implementation.inl"
#include "Reference/Obs_Implementation.inl"
