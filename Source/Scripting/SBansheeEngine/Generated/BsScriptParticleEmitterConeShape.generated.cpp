#include "BsScriptParticleEmitterConeShape.generated.h"
#include "BsMonoMethod.h"
#include "BsMonoClass.h"
#include "BsMonoUtil.h"
#include "../../../bsf/Source/Foundation/bsfCore/Particles/BsParticleEmitter.h"
#include "BsScriptPARTICLE_CONE_SHAPE_DESC.generated.h"

namespace bs
{
	ScriptParticleEmitterConeShape::ScriptParticleEmitterConeShape(MonoObject* managedInstance, const SPtr<ParticleEmitterConeShape>& value)
		:ScriptObject(managedInstance)
	{
		mInternal = value;
	}

	SPtr<ParticleEmitterConeShape> ScriptParticleEmitterConeShape::getInternal() const 
	{
		return std::static_pointer_cast<ParticleEmitterConeShape>(mInternal);
	}

	void ScriptParticleEmitterConeShape::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_setOptions", (void*)&ScriptParticleEmitterConeShape::Internal_setOptions);
		metaData.scriptClass->addInternalCall("Internal_getOptions", (void*)&ScriptParticleEmitterConeShape::Internal_getOptions);

	}

	MonoObject* ScriptParticleEmitterConeShape::create(const SPtr<ParticleEmitterConeShape>& value)
	{
		if(value == nullptr) return nullptr; 

		bool dummy = false;
		void* ctorParams[1] = { &dummy };

		MonoObject* managedInstance = metaData.scriptClass->createInstance("bool", ctorParams);
		new (bs_alloc<ScriptParticleEmitterConeShape>()) ScriptParticleEmitterConeShape(managedInstance, value);
		return managedInstance;
	}
	void ScriptParticleEmitterConeShape::Internal_setOptions(ScriptParticleEmitterConeShape* thisPtr, __PARTICLE_CONE_SHAPE_DESCInterop* options)
	{
		PARTICLE_CONE_SHAPE_DESC tmpoptions;
		tmpoptions = ScriptPARTICLE_CONE_SHAPE_DESC::fromInterop(*options);
		thisPtr->getInternal()->setOptions(tmpoptions);
	}

	void ScriptParticleEmitterConeShape::Internal_getOptions(ScriptParticleEmitterConeShape* thisPtr, __PARTICLE_CONE_SHAPE_DESCInterop* __output)
	{
		PARTICLE_CONE_SHAPE_DESC tmp__output;
		tmp__output = thisPtr->getInternal()->getOptions();

		__PARTICLE_CONE_SHAPE_DESCInterop interop__output;
		interop__output = ScriptPARTICLE_CONE_SHAPE_DESC::toInterop(tmp__output);
		MonoUtil::valueCopy(__output, &interop__output, ScriptPARTICLE_CONE_SHAPE_DESC::getMetaData()->scriptClass->_getInternalClass());
	}
}
