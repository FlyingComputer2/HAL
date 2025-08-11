#ifndef __HALSIM_IPC_SYNC_HPP__
#define __HALSIM_IPC_SYNC_HPP__

#include <cstdint>
#include <string>

#include <pthread.h>
#include <errno.h>

#include <cstring>
#include <stdexcept>

namespace hal
{

template<typename... Ts>
std::string format(const char* fmt, Ts&&... ts)
{
    char out[1024*64];
    snprintf(out, sizeof(out), fmt, ts...);
    return out;
}

class mutex
{
public:
    mutex()
    {
        pthread_mutexattr_t attr;
        if (0 != pthread_mutexattr_init(&attr))
        {
            throw std::runtime_error(format("Failed to initialize mutex attributes error=%s", strerror(errno)));
        }

        if (0 != pthread_mutex_init(&m_mutex, &attr))
        {
            throw std::runtime_error(format("Failed to initialize mutex error=%s", strerror(errno)));
        }

        pthread_mutexattr_destroy(&attr);
    }

    ~mutex()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    mutex(const mutex&) = delete;
    mutex& operator=(const mutex&) = delete;

    void lock();
    void unlock();
    bool try_lock();

private:
    pthread_mutex_t m_mutex;
};

template<typename mutex>
class scoped_lock
{
public:
    scoped_lock(mutex& m)
        : m(&m)
    {
        m.lock();
    }

    ~scoped_lock()
    {
        m->unlock();
    }

    void lock()
    {
        m->lock();
    }

    void unlock()
    {
        m->unlock();
    }

private:
    mutex* m;
};

class condition_variable 
{
public:
    condition_variable()
    {
        pthread_cond_init(&cond_, nullptr);
        pthread_mutex_init(&internal_mutex_, nullptr);
        waiters_ = 0;
        pending_signals_ = 0;
    }

    ~condition_variable()
    {
        pthread_cond_destroy(&cond_);
        pthread_mutex_destroy(&internal_mutex_);
    }

    condition_variable(const condition_variable&) = delete;
    condition_variable& operator=(const condition_variable&) = delete;

    template <typename Lock>
    void wait(Lock& lk)
    {
        pthread_mutex_lock(&internal_mutex_);
        ++waiters_;
        pthread_mutex_unlock(&internal_mutex_);

        lk.unlock();

        pthread_mutex_lock(&internal_mutex_);
        while (pending_signals_ == 0)
        {
            pthread_cond_wait(&cond_, &internal_mutex_);
        }
        if (pending_signals_ > 0)
        {
            --pending_signals_;
        }
        --waiters_;
        pthread_mutex_unlock(&internal_mutex_);

        lk.lock();
    }

    void notify_one() noexcept {
        pthread_mutex_lock(&internal_mutex_);
        if (waiters_ > pending_signals_)
        {
            ++pending_signals_;
            pthread_cond_signal(&cond_);
        }
        pthread_mutex_unlock(&internal_mutex_);
    }

    void notify_all() noexcept {
        pthread_mutex_lock(&internal_mutex_);
        if (waiters_ > 0)
        {
            pending_signals_ = waiters_;
            pthread_cond_broadcast(&cond_);
        }
        pthread_mutex_unlock(&internal_mutex_);
    }

private:
    pthread_cond_t cond_;
    pthread_mutex_t internal_mutex_;
    unsigned waiters_;
    unsigned pending_signals_;
};

} // namespace hal

#endif // __HALSIM_IPC_SYNC_HPP__
