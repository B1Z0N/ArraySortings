// classes for generating random values of numeric and real type
// all other types can be deduced from this types

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
        std::independent_bits_engine <
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
class GenFunctor_base
// class for generating values of type OutT with help of seeded
// generator of type SeededPRNG_T thata generates values of type InT

// InT and OuT should be of numeric types (real or integral)
// SeedePRNG_T must meets the requirements of RandomNumberEngine(named requirement)

// for classes that use heap, or mask their actual size(with pointers, for example)
// write your own manual generator, using this one
// class meets the requirements of RandomNumberEngine(named requirement)
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


template <
    typename T,
    typename DistribT,
    typename GeneratorT
    >
class LimGenFunctor_base
// class for generating limited values of different types
// with help of:
// T - type being generated
// DistribT - distribution of generation, that meets the requirements
// of RandomNumberDistribution(named requirement)
// GeneratorT - generator, that meets the requirements
// of RandomNumberEngine(named requirement)
// base class, that sets interface to other classes
{
    GeneratorT gen  {};
    DistribT dis;
public:
    LimGenFunctor_base(const T& from, const T& to)
    // construct distribution
        : dis{from, to} {}

    // LimGenFunctor_base(T from, T to)
    // // construct distribution
    //     : dis{from, to} {}

    T operator()()
    // call for generating
    {
        return dis(gen);
    }
};


template <
    typename T,
    typename DistribT,
    typename GeneratorT,
    T from,
    T to
    >
class LimGenFunctorVal : LimGenFunctor_base<T, DistribT, GeneratorT>
// class for generating limited values of different types
// in range [from, to]
// with help of:
// T - type being generated
// DistribT - distribution of generation, that meets the requirements
// of RandomNumberDistribution(named requirement)
// GeneratorT - generator, that meets the requirements
// of RandomNumberEngine(named requirement)
{
    using parent = LimGenFunctor_base<T, DistribT, GeneratorT>;
public:
    LimGenFunctorVal() : parent{from , to} {}
};


template <
    typename T,
    typename DistribT,
    typename GeneratorT,
    typename From,
    typename To
    >
class LimGenFunctorType : public LimGenFunctor_base<T, DistribT, GeneratorT>
// class for generating limited values of different types
// with help of:
// T - type being generated
// DistribT - distribution of generation, that meets the requirements
// of RandomNumberDistribution(named requirement)
// GeneratorT - generator, that meets the requirements
// of RandomNumberEngine(named requirement)
// classes From and To should have static member "value" of type T
// that limits the range [From::value, To::value]
// this class is used when limitters of type T, couldn't
// be passed as nontype template arguments(e.g. double, const char*)
{
    using parent = LimGenFunctor_base<T, DistribT, GeneratorT>;
public:
    LimGenFunctorType()
    // construct distribution
        : parent{From::value, To::value} {}
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

template <typename InT, typename OutT, typename PRNG_T, size_t seedlen>
using GenFunctor = GenFunctor_base<InT, OutT, SeededPRNG<InT, PRNG_T, seedlen>>;
// template class for creating own generators

template <typename OutT>
using unlimited_mtgenf = GenFunctor_base<uint32_t, OutT, SeededMT>;
// mt19937 ready for generation of values, of type OutT
// OutT - should be numeric (real or integral)


template <typename IntT, IntT from, IntT to>
using lim_unif_int_mtgenf = 
    LimGenFunctorVal < IntT,
        std::uniform_int_distribution<IntT>,
        SeededMT, from, to
    >;
// class that generates limited range values of type IntT
// IntT should be of integral type


// next classes trying to get around the fact that
// we couldn't pass real numbers through template
// nontype arguments


template <typename RealT>
using lim_unif_real_mtgenf = 
    LimGenFunctor_base <
        RealT,
        std::uniform_real_distribution<RealT>,
        SeededMT
    >;
// class that generates limited range values of type RealT
// RealT should be of real type
// for using this class with SortBench you should overload it
// and pass default compiletime arguments to constructor


template <typename RealT, typename From, typename To>
using lim_unif_real_mtgenf_type = 
    LimGenFunctorType < RealT,
        std::uniform_real_distribution<RealT>,
        SeededMT, From, To
    >;
// class that generates limited range values of type RealT
// RealT should be of real type
// classes From and To should have static member "value" of type RealT
// that shows the range [From::value, To::value]


};

#endif




// TODO: add keep_before, keep_after options to SortBench
// TODO: use template nontype arguments for passing to LimitedGenerator