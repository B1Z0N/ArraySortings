#ifndef GENFUNC_INTERFACE
#define GENFUNC_INTERFACE

#include <random>
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>
#include <array>
#include <iterator>
#include <cmath>
#include <cstring>

namespace srtbch {


//--------------GENERAL FASCILITIES FOR PSEUDO RANDOM VALUE GENERATION-------------


template <
    typename T, 
    typename PRNG_T,
    size_t seed_length
    >
class SeededPRNG
// class that generates seed values for PRNG
// T - type for generating
// PRNG_T - type of generator should have a constructor 
// that accepts two iterators of seed iterable
// seed_length - length of seed iterable
// class meets the requirements of RandomNumberEngine(named requirement) 
{
    PRNG_T generator;
public:
    using result_type = T;

    T min() { return generator.min(); }
    T max() { return generator.max(); }

    SeededPRNG()
    // constructor, seeds our PRNG_T object with values of appropriate type
    {
        std::independent_bits_engine<
                                     std::random_device,
                                     sizeof(uint16_t),
                                     uint16_t
                                     > rd {};
        constexpr auto enough_size {
            static_cast<int>(std::ceil(0.5 * sizeof(T) * seed_length))
        };

        uint16_t numbers[enough_size];
        std::generate(numbers, std::end(numbers), std::ref(rd));

        T elements[seed_length];
        std::memcpy(elements, numbers, sizeof(elements));

        // SeedSeq sseq(elements, std::end(elements));

        generator = PRNG_T {elements, std::end(elements)};
    }

    T operator()()
    // call for generating values
    {
        return generator();
    }
};


template <
    typename InT,
    typename OutT,
    typename SeededPRNG_T
    >
// inT and ouT should be of numeric types
// what if type allocated on the heap, and we don't know actual size of it's type?
class GenFunctor_basic
{
    static constexpr size_t outsz   {sizeof(OutT)};
    static constexpr size_t insz    {sizeof(InT)};
    static constexpr size_t buff_len {outsz < insz ? 1 : outsz / insz + 1};

    SeededPRNG_T generator {};
public:

    OutT operator()()
    {
        auto buffer {gen_buffer().data()};

        OutT value;
        std::memcpy(&value, buffer, sizeof(value));
        return value;
    }
private:

    auto gen_buffer()
    {
        std::array<InT, buff_len> arr {};
        std::generate(std::begin(arr), std::end(arr), std::ref(generator));

        return arr;
    }
};


template <typename T, typename DistribT, typename GeneratorT>
class LimGenFunctor
{
    GeneratorT gen  {};
    DistribT dis;
public:
    LimGenFunctor(T from, T to)
    : dis{from, to} {}
    
    T operator()()
    {
        return dis(gen);
    }
};


//----------------------------USEFUL TYPE ALIASES FOR CONCRETE USAGE--------------------------


class __MT19937Seeder {

    template <class It>
    class SeedSeq
    {
        It from;
        It to;
    public:

        SeedSeq(It from, It to)
            : from{from}, to{to} {}

        template <typename It2>
        void generate(It2 b, It2 e)
        {
            assert((e - b) <= (to - from));
            std::copy(from, from + (e - b), b);
        }
    };

    std::mt19937 gen;
public:
    __MT19937Seeder() = default;

    template <typename It>
    __MT19937Seeder(It start, It end)
    {
        SeedSeq sequence {start, end};
        gen = std::mt19937 {sequence};
    }    

    auto min() { return gen.min(); }
    auto max() { return gen.max(); }

    auto operator()()
    {
        return gen();
    }
};

static constexpr size_t mt19937_seed_length {624};

using SeededMT = SeededPRNG<uint32_t, __MT19937Seeder, mt19937_seed_length>;

template <typename InT, typename OutT, typename PRNG, size_t seedlen>
using GenFunctor = GenFunctor_basic<InT, OutT, SeededPRNG<InT, PRNG, seedlen>>;

template <typename OutT>
using unlimited_mtgenf = GenFunctor_basic<uint32_t, OutT, SeededMT>;

template <typename IntT>
using lim_unif_int_mtgenf = LimGenFunctor<
                                IntT,
                                std::uniform_int_distribution<IntT>,
                                SeededMT
                                >;

template <typename RealT> 
using lim_unif_real_mtgenf = LimGenFunctor<
                                RealT,
                                std::uniform_real_distribution<RealT>,
                                SeededMT
                                >;


};

#endif