#pragma once

class spin final
{
public:
    spin();

    spin(const spin& rhs);

    spin& operator =(const spin& rhs);

    ~spin() = default;

public:
    boole init();

    boole is_init();

    boole uninit();

    boole try_acquire();

    boole wait_acquire();

    boole release();

private:
    atom<s64>  _v;
};
