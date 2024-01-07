#ifndef MODEX_REFLECTOR_H
#define MODEX_REFLECTOR_H

#define RF_CLASS(...)
#define RF_SERIALIZE(...)

namespace codex {
	class NativeBehaviour;
} // namespace codex

extern "C" CODEX_EXPORT codex::NativeBehaviour* Rf_CreateInstance(const char* className);
extern "C" CODEX_EXPORT bool             Rf_DoesInstanceExist(const char* className);
extern "C" CODEX_EXPORT void             Rf_Dummy();

#endif // MODEX_REFLECTOR_H