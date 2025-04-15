#pragma once

template<typename T>
struct SequenceGenerator
{
    virtual ~SequenceGenerator() = default;

    virtual void reset(T start) = 0;
    virtual T next() = 0;
};

template<typename T>
struct LinearSequenceGenerator : public SequenceGenerator<T> {
    T current;

    LinearSequenceGenerator(T start) : current(start) {}

    void reset(T start) override {
        current = start;
    }

    T next() override {
        int ret = current;
        current = current + 1;
        return ret;
    }
};