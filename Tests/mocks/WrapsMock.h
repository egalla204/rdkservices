#pragma once

#include <gmock/gmock.h>
#include <mntent.h>

#include "Wraps.h"

extern "C" FILE* __real_fopen(const char* filename, const char* mode);
extern "C" FILE* __real_setmntent(const char* command, const char* type);

class WrapsImplMock : public WrapsImpl {
public:
    WrapsImplMock():WrapsImpl()
    {
       /*Setting up Default behavior for fopen: 
        * We are mocking fopen in this file below with __wrap_fopen,
        * and the actual fopen will be called via this interface */
        ON_CALL(*this, fopen(::testing::_, ::testing::_))
        .WillByDefault(::testing::Invoke(
           [](const char* filename, const char* mode) -> FILE* {
                FILE *fp = __real_fopen(filename,mode);
                return fp;
            }));
			
	    /*Setting up Default behavior for setmntent: 
        * We are mocking setmntent in this file below with __wrap_setmntent,
        * and the actual setmntent will be called via this interface */
        ON_CALL(*this, setmntent(::testing::_, ::testing::_))
        .WillByDefault(::testing::Invoke(
            [&](const char* command, const char* type) -> FILE* {
                return __real_setmntent(command, type);
            }));
    }
    virtual ~WrapsImplMock() = default;

    MOCK_METHOD(int, system, (const char* command), (override));
    MOCK_METHOD(FILE*, popen, (const char* command, const char* type), (override));
    MOCK_METHOD(int, pclose, (FILE* pipe), (override));
    MOCK_METHOD(void, syslog, (int pri, const char* fmt, va_list args), (override));
    MOCK_METHOD(FILE*, fopen, (const char* filename, const char* mode), (override));
    MOCK_METHOD(FILE*, setmntent, (const char* command, const char* type), (override));
    MOCK_METHOD(struct mntent*, getmntent, (FILE* pipe), (override));
};
