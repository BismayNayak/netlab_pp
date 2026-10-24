#pragma once
#include <unistd.h>
#include <utility>

namespace netlab {
    class UniqueFd{
    private :
        int fd_=-1;
    public:
        UniqueFd() noexcept = default;
        explicit UniqueFd(int fd) noexcept;
        ~UniqueFd();

        UniqueFd(const UniqueFd&)=delete;
        UniqueFd& operator=(const UniqueFd&)= delete;
        UniqueFd(UniqueFd&& other) noexcept;
        UniqueFd& operator=(UniqueFd&& other) noexcept;

        int get() const noexcept;
        bool valid() const  noexcept;

        int release() noexcept;
        void reset(int new_fd=-1) noexcept;

    };
}
