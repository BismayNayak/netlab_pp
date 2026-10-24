#include "netlab/fd.hpp"

namespace netlab {
    UniqueFd::UniqueFd (int fd) noexcept: fd_(fd) {}

    UniqueFd::~UniqueFd(){
        if (fd_>=0){
            ::close(fd_);
        }
    }

    UniqueFd::UniqueFd(UniqueFd&& other) noexcept
        :fd_(std::exchange(other.fd_,-1)){}

    UniqueFd& UniqueFd::operator=(UniqueFd&& other) noexcept{
        if (this!= &other) reset(other.release());
        return *this;
    }

    int UniqueFd::get() const noexcept {
        return fd_;
    }

    bool UniqueFd::valid()const noexcept{
        return fd_ != -1;
    }

    int UniqueFd::release() noexcept{
        return std::exchange(fd_,-1);
    }

    void UniqueFd::reset(int new_fd) noexcept{
        if(fd_ >=0 && fd_ !=new_fd){
            ::close(fd_);
        }
        fd_=new_fd;
    }

}
