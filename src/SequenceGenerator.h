#pragma once

template<typename T>
struct SequenceGenerator
{
    virtual ~SequenceGenerator() = default;

    virtual void reset(T start) = 0;
    virtual T next() = 0;
};

template<typename T>
struct LinearSequenceGenerator : public SequenceGenerator {
    T current;

    LinearSequenceGenerator(T start) : current(start) {}

    virtual T next() {
        int ret = current;
        current = current + 1;
        return ret;
    }
};