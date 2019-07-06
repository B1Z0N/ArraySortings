#ifndef GENFUNC
#define GENFUNC

#include <random>

#include <algorithm>
#include <functional>
#include <array>

#include <cassert>
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
// class for generating values of type OutT with help of seeded
// generator of type SeededPRNG_T thata generates values of type InT

// InT and OuT should be of numeric types (real or integral)
// SeedePRNG_T must meets the requirements of RandomNumberEngine(named requirement)

// for classes that use heap, or mask their actual size(with pointers, for example)
// write your own manual generator, using this one
// class meets the requirements of RandomNumberEngine(named requirement) 
class GenFunctor_basic
{
    static constexpr size_t outsz   {sizeof(OutT)};
    static constexpr size_t insz    {sizeof(InT)};
    static constexpr size_t buff_len {outsz < insz ? 1 : outsz / insz + 1};

    SeededPRNG_T generator {};
public:

    OutT operator()()
    // call for generating values
    {
        auto buffer {gen_buffer().data()};

        OutT value;
        std::memcpy(&value, buffer, sizeof(value));
        return value;
    }
private:

    auto gen_buffer()
    // function that generates buffer of InT 
    // long enough for covering type OutT
    {
        std::array<InT, buff_len> arr {};
        std::generate(std::begin(arr), std::end(arr), std::ref(generator));

        return arr;
    }
};


template <typename T, typename DistribT, typename GeneratorT>
class LimGenFunctor
// class for generating limited values of different types
// with help of:
// T - type being generated
// DistribT - distribution of generation, that meets the requirements
// of RandomNumberDistribution(named requirement)
// GeneratorT - generator, that meets the requirements 
// of RandomNumberEngine(named requirement)
{
    GeneratorT gen  {};
    DistribT dis;
public:
    LimGenFunctor(T from, T to)
    // construct distribution
    : dis{from, to} {}
    
    T operator()()
    // call for generating
    {
        return dis(gen);
    }
};


//----------------------------USEFUL TYPE ALIASES FOR CONCRETE USAGE--------------------------


class __MT19937Seeder {
// helper class for seeding MT19937
// satisfies RandomNumberEngine(named requirement)

    template <class It>
    class SeedSeq
    // class for passing to mt19937 constructor
    // stores seeded values
    {
        It from;
        It to;
    public:

        SeedSeq(It from, It to)
            : from{from}, to{to} {}

        template <typename It2>
        void generate(It2 b, It2 e)
        // fill given range [b, e] with
        // seed values
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
    // constructs seeded mt19937
    // start, end - iterators, that delimit seed sequence
    {
        SeedSeq sequence {start, end};
        gen = std::mt19937 {sequence};
    }    

    auto min() { return gen.min(); }
    auto max() { return gen.max(); }

    auto operator()()
    // call for generation
    {
        return gen();
    }
};

static constexpr size_t mt19937_seed_length {624};

using SeededMT = SeededPRNG<uint32_t, __MT19937Seeder, mt19937_seed_length>;
// mt19937 ready for generation, seeded with std::random_device

template <typename InT, typename OutT, typename PRNG, size_t seedlen>
using GenFunctor = GenFunctor_basic<InT, OutT, SeededPRNG<InT, PRNG, seedlen>>;
// template class for creating own generators

template <typename OutT>
using unlimited_mtgenf = GenFunctor_basic<uint32_t, OutT, SeededMT>;
// mt19937 ready for generation of values, of type OutT
// OutT - should be numeric (real or integral)

template <typename IntT>
using lim_unif_int_mtgenf = LimGenFunctor<
                                IntT,
                                std::uniform_int_distribution<IntT>,
                                SeededMT
                                >;
// class that generates limited range values of type IntT
// IntT should be of integral type

template <typename RealT> 
using lim_unif_real_mtgenf = LimGenFunctor<
                                RealT,
                                std::uniform_real_distribution<RealT>,
                                SeededMT
                                >;
// class that generates limited range values of type RealT
// IntT should be of real type


};

#endif