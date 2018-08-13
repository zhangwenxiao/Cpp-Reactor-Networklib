#ifndef __CALLBACK_H__
#define __CALLBACK_H__

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace muduo {
    using TimerCallback = boost::function<void()>;
}

#endif
