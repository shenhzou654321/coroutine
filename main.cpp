#include <iostream>
#include <sstream>

#include "coroutine.h"

using namespace std;

struct UserParam
{
    UserParam()
        : str("")
        , i(0)
        , d(0.0)
    {
    }

    std::string print()
    {
        std::ostringstream os;

        os << "str:" << str
           << ", i:" << i
           << ", d:" <<d
           << endl;

        return os.str();
    }

    std::string str;
    int i;
    double d;
};

void routine(void* args)
{
    cout << __func__ << ", args:" << args << endl;

    UserParam* param = (UserParam*)args;

    cout << param->print() << endl;

    CoroutineContext* ctx = get_cur_ctx();
    Yield(ctx);

    {
        cout << "ctx:" << ctx << endl;
        cout << "stack:" << endl;
        uint64_t* p = (uint64_t*)ctx->stack_;
        for (int i = 0; i != 8; ++i)
        {
            cout << "[" << i << "] " << hex << *p++ << endl;
        }
    }

    {
        CoroutineContext* ctx = get_thread_schedule_ctx();
        cout << "ctx:" << ctx << endl;
        cout << "stack:" << endl;
        uint64_t* p = (uint64_t*)ctx->stack_;
        for (int i = 0; i != 8; ++i)
        {
            cout << "[" << i << "] " << hex << *p++ << endl;
        }
    }
}

int main(int argc, char* argv[], char* env[])
{
    {
        CoroutineContext* ctx = get_thread_schedule_ctx();
        cout << "ctx:" << ctx << endl;
        cout << "stack:" << endl;
        uint64_t* p = (uint64_t*)ctx->stack_;
        for (int i = 0; i != 8; ++i)
        {
            cout << "[" << i << "] " << hex << *p++ << endl;
        }
    }

    UserParam* param = new UserParam();
    CoroutineContext* ctx = CoroutineCreate(routine, param);

    //ctx->StoreRegister();
    cout << __func__ << ":" << __LINE__ << endl;
    Resume(ctx);

    return 0;
}