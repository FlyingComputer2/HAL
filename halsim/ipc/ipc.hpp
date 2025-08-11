#ifndef __HALSIM_IPC_IPC_HPP__
#define __HALSIM_IPC_IPC_HPP__

#include <ipc/sync.hpp>

namespace hal
{

struct command_s
{
    size_t data_sz;
    std::byte data[1024];
    bool satisfied;
    mutex m;
    condition_variable cv;
};

class ipc
{
public:
    command_s& wait_command()
    {
        scoped_lock lg(cmd_queue_mtx);
        while (head == tail)
        {
            cmd_queue_new_entry_cv.wait(lg);
        }
        return cmd_queue[head];
    }

    void complete_first_execution()
    {
        scoped_lock lg(cmd_queue_mtx);
        auto& c = cmd_queue[head];
        c.satisfied = true;
        c.cv.notify_one();
    }

    void execute(const std::byte* in, size_t in_sz, std::byte* out, size_t& out_sz)
    {
        if (in_sz > sizeof(command_s::data))
        {
            out_sz = 0;
            return;
        }
        
        size_t i = 0;

        // allocate slot
        {
            scoped_lock lg(cmd_queue_mtx);
            while (n == N)
            {
                cmd_queue_cv.wait(lg);
            }

            i = tail;
            tail = (tail+1)%N;
            n++;
        }

        auto& c = cmd_queue[i];

        {
            scoped_lock cmd_lg(c.m);  // Lock command during setup
            c.data_sz = in_sz;
            c.satisfied = false;
            std::memcpy(c.data, in, in_sz);
        }

        // notify waiting executor
        cmd_queue_new_entry_cv.notify_one();

        // wait to be executed
        {
            scoped_lock lg(c.m);
            while (!c.satisfied)
            {
                c.cv.wait(lg);
            }

            out_sz = c.data_sz;
            std::memcpy(out, c.data, c.data_sz);
        }

        // free slot
        {
            scoped_lock lg(cmd_queue_mtx);
            head = (head+1)%N;
            n--;
            cmd_queue_cv.notify_all();
        }
    }

private:
    constexpr static size_t N = 128;
    command_s cmd_queue[N];
    size_t head = 0;
    size_t tail = 0;
    size_t n = 0;
    // cmd_queue
    mutex cmd_queue_mtx;
    condition_variable cmd_queue_cv;
    condition_variable cmd_queue_new_entry_cv;
    // head
};

} // namespace hal

#endif // __HALSIM_IPC_IPC_HPP__
