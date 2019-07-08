/** @file 
classes for generating random values of numeric and real type
all other types can be deduced from this types */

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
/** class that generates seed values for PRNG\n
 * T - type for generating\n
 * PRNG_T - type of generator should have a constructor\n
 * that accepts two iterators of seed iterable\n
 * seed_length - length of seed iterable\n
 * class meets the requirements of RandomNumberEngine(named requirement)
 */
{
    PRNG_T generator;
public:
    using result_type = T;

    T min() { return generator.min(); }
    T max() { return generator.max(); }

    /** constructor, seeds our PRNG_T object with values of appropriate type*/
    SeededPRNG()
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

    /** call for generating values*/
    T operator()()
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
/** class for generating values of type OutT with help of seeded\n
 *  generator of type SeededPRNG_T thata generates values of type InT\n\n
 *  
 *  InT and OuT should be of numeric types (real or integral)\n
 *  SeedePRNG_T must meets the requirements of RandomNumberEngine(named requirement)\n\n
 *  
 *  for classes that use heap, or mask their actual size(with pointers, for example)\n
 *  write your own manual generator, using this one\n
 *  class meets the requirements of RandomNumberEngine(named requirement)
 */
{
    static constexpr size_t outsz   {sizeof(OutT)};
    static constexpr size_t insz    {sizeof(InT)};
    static constexpr size_t buff_len {outsz < insz ? 1 : outsz / insz + 1};

    SeededPRNG_T generator {};
public:

    /** call for generating values*/
    OutT operator()()
    {
        auto buffer {gen_buffer().data()};

        OutT value;
        std::memcpy(&value, buffer, sizeof(value));
        return value;
    }
private:

    /** function that generates buffer of InT\n
     *  long enough for covering type OutT\n
     */
    auto gen_buffer()
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
/** class for generating limited values of different types\n
 *  with help of:\n
 *  T - type being generated\n
 *  DistribT - distribution of generation, that meets the requirements\n
 *  of RandomNumberDistribution(named requirement)\n
 *  GeneratorT - generator, that meets the requirements\n
 *  of RandomNumberEngine(named requirement)\n
 *  base class, that sets interface to other classes
 */
{
    GeneratorT gen  {};
    DistribT dis;
public:
    /** construct distribution */
    LimGenFunctor_base(const T& from, const T& to)
        : dis{from, to} {}

    /** call for generating*/
    T operator()()
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
/** class for generating limited values of different types\n
 *  in range [from, to]\n
 *  with help of:\n
 *  T - type being generated\n
 *  DistribT - distribution of generation, that meets the requirements\n
 *  of RandomNumberDistribution(named requirement)\n
 *  GeneratorT - generator, that meets the requirements\n
 *  of RandomNumberEngine(named requirement)\n
 */
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
/** class for generating limited values of different types\n
 *  with help of:\n
 *  T - type being generated\n
 *  DistribT - distribution of generation, that meets the requirements\n
 *  of RandomNumberDistribution(named requirement)\n
 *  GeneratorT - generator, that meets the requirements\n
 *  of RandomNumberEngine(named requirement)\n
 *  classes From and To should have static member "value" of type T\n
 *  that limits the range [From::value, To::value]\n
 *  this class is used when limitters of type T, couldn't\n
 *  be passed as nontype template arguments(e.g. double, const char*)
 */
{
    using parent = LimGenFunctor_base<T, DistribT, GeneratorT>;
public:
    /** construct distribution */
    LimGenFunctorType()
        : parent{From::value, To::value} {}
};


//----------------------------USEFUL TYPE ALIASES FOR CONCRETE USAGE--------------------------


class __MT19937Seeder {
/** helper class for seeding MT19937\n
 *  satisfies RandomNumberEngine(named requirement)
 */

    template <class It>
    class SeedSeq
    /** class for passing to mt19937 constructor\n
     *  stores seeded values
     */
    {
        It from;
        It to;
    public:

        SeedSeq(It from, It to)
            : from{from}, to{to} {}

        /** fill given range [b, e] with seed values */
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

    /** constructs seeded mt19937\n
     *  start, end - iterators, that delimit seed sequence
     */
    template <typename It>
    __MT19937Seeder(It start, It end)
    {
        SeedSeq sequence {start, end};
        gen = std::mt19937 {sequence};
    }

    auto min() { return gen.min(); }
    auto max() { return gen.max(); }

    /** call for generation */
    auto operator()()
    {
        return gen();
    }
};

static constexpr size_t mt19937_seed_length {624};

/** mt19937 ready for generation, seeded with std::random_device*/
using SeededMT = SeededPRNG<uint32_t, __MT19937Seeder, mt19937_seed_length>;

/** template class for creating own generators*/
template <typename InT, typename OutT, typename PRNG_T, size_t seedlen>
using GenFunctor = GenFunctor_base<InT, OutT, SeededPRNG<InT, PRNG_T, seedlen>>;

/** mt19937 ready for generation of values, of type OutT\n
 *  OutT - should be numeric (real or integral)
 */
template <typename OutT>
using unlimited_mtgenf = GenFunctor_base<uint32_t, OutT, SeededMT>;



/** class that generates limited range values of type IntT\n
 *  IntT should be of integral type
 */
template <typename IntT, IntT from, IntT to>
using lim_unif_int_mtgenf = 
    LimGenFunctorVal < IntT,
        std::uniform_int_distribution<IntT>,
        SeededMT, from, to
    >;


// next classes trying to get around the fact that
// we couldn't pass real numbers through template
// nontype arguments


/** class that generates limited range values of type RealT\n
 *  RealT should be of real type\n
 *  for using this class with SortBench you should overload it\n
 *  and pass default compiletime arguments to constructor
 */
template <typename RealT>
using lim_unif_real_mtgenf = 
    LimGenFunctor_base <
        RealT,
        std::uniform_real_distribution<RealT>,
        SeededMT
    >;


/** class that generates limited range values of type RealT\n
 *  RealT should be of real type\n
 *  classes From and To should have static member "value" of type RealT\n
 *  that shows the range [From::value, To::value]
 */
template <typename RealT, typename From, typename To>
using lim_unif_real_mtgenf_type = 
    LimGenFunctorType < RealT,
        std::uniform_real_distribution<RealT>,
        SeededMT, From, To
    >;


};

#endif