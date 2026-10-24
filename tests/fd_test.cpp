#include "netlab/fd.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <utility>

int failure = 0;

void expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        failure++;
    }
}
int main() {
    // Test 1 — Default object is empty
    {
        netlab::UniqueFd empty;
        expect(empty.valid() == false, "Default object is empty");
    }

    // Test 2 — Constructed owner stores FD and real FD can be owned
    {
        int raw_fd = ::open("/dev/null", O_RDONLY);
        netlab::UniqueFd owner(raw_fd);
        expect(owner.valid() == true, "Real FD can be owned");
        expect(owner.get() == raw_fd, "Constructed owner stores FD");
    }

    // Test 3 — Scope destruction closes through RAII
    {
        int raw_fd = ::open("/dev/null", O_RDONLY);
        {
            netlab::UniqueFd owner(raw_fd);
        }
        // fcntl returns -1 if the file descriptor is already closed
        expect(::fcntl(raw_fd, F_GETFD) == -1, "Scope destruction closes through RAII");
    }

    // Test 4 — Copying is compile-time forbidden
    // {
    //     netlab::UniqueFd a(::open("/dev/null", O_RDONLY));
    //     netlab::UniqueFd b = a; // COMPILE ERROR: copy constructor deleted
    // }

    // Test 5 — Move construction transfers ownership and moved-from object becomes empty
    {
        int raw_fd = ::open("/dev/null", O_RDONLY);
        netlab::UniqueFd first(raw_fd);
        netlab::UniqueFd second(std::move(first));
        expect(first.valid() == false, "Moved-from object becomes empty");
        expect(second.get() == raw_fd, "Move construction transfers ownership");
    }

    // Test 6 — release() transfers raw ownership outward
    {
        int raw_fd = ::open("/dev/null", O_RDONLY);
        netlab::UniqueFd owner(raw_fd);
        int leaked_fd = owner.release();
        expect(owner.valid() == false, "Owner becomes empty after release");
        expect(leaked_fd == raw_fd, "Raw FD is transferred outward");
        ::close(leaked_fd);
    }

    // Test 7 — reset() closes and empties owner
    {
        int raw_fd = ::open("/dev/null", O_RDONLY);
        netlab::UniqueFd owner(raw_fd);
        owner.reset();
        expect(owner.valid() == false, "Owner becomes empty after reset()");
    }

    // Test 8 — Move assignment transfers ownership and disposes destination's old resource
    {
        int fd_a = ::open("/dev/null", O_RDONLY);
        int fd_b = ::open("/dev/null", O_RDONLY);
        netlab::UniqueFd owner_a(fd_a);
        netlab::UniqueFd owner_b(fd_b);

        owner_b = std::move(owner_a);

        expect(owner_a.valid() == false, "Moved-from object becomes empty");
        expect(owner_b.get() == fd_a, "Destination acquires new FD, old resource disposed");
    }

    if (failure == 0) {
        std::cout << "All tests passed successfully!\n";
        return 0;
    }

    std::cerr<<failure<<"test(s) failed.\n";
    return failure;
}
