#include "BsScriptParticleForce.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../bsf/Source/Foundation/bsfCore/Particles/BsParticleEvolver.h"
#include "BsScriptPARTICLE_FORCE_DESC.generated.h"

namespace bs
{
	ScriptParticleForce::ScriptParticleForce(MonoObject* managedInstance, const SPtr<ParticleForce>& value)
		:ScriptObject(managedInstance)
	{
		mInternal = value;
	}

	SPtr<ParticleForce> ScriptParticleForce::getInternal() const 
	{
		return std::static_pointer_cast<ParticleForce>(mInternal);
	}

	void ScriptParticleForce::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_setOptions", (void*)&ScriptParticleForce::Internal_setOptions);
		metaData.scriptClass->addInternalCall("Internal_getOptions", (void*)&ScriptParticleForce::Internal_getOptions);

	}

	MonoObject* ScriptParticleForce::create(const SPtr<ParticleForce>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleForce>()) ScriptParticleForce(managedInstance, value);
		return managedInstance;
	}
	void ScriptParticleForce::Internal_setOptions(ScriptParticleForce* thisPtr, __PARTICLE_FORCE_DESCInterop* options)
	{
		PARTICLE_FORCE_DESC tmpoptions;
		tmpoptions = ScriptPARTICLE_FORCE_DESC::fromInterop(*options);
		thisPtr->getInternal()->setOptions(tmpoptions);
	}

	void ScriptParticleForce::Internal_getOptions(ScriptParticleForce* thisPtr, __PARTICLE_FORCE_DESCInterop* __output)
	{
		PARTICLE_FORCE_DESC tmp__output;
		tmp__output = thisPtr->getInternal()->getOptions();

		__PARTICLE_FORCE_DESCInterop interop__output;
		interop__output = ScriptPARTICLE_FORCE_DESC::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptPARTICLE_FORCE_DESC::getMetaData()->scriptClass->_getInternalClass());
	}
}
