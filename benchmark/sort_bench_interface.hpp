#ifndef AOS_LAB2_SORT_BENCH_INTERFACE
#define AOS_LAB2_SORT_BENCH_INTERFACE

#include <vector>
#include <tuple>
#include <array>
#include <chrono>

namespace srtbch
{
struct CmpAsgn
{
    size_t cmp;
    size_t asgn;
};

using SortStats = std::vector<std::tuple<
                  size_t,                     // array length
                  std::chrono::duration,      // sorting duration
                  CmpAsgn                     // comparisons and assignments
                  >>

                  template <typename T, typename GenFunc, template <typename> typename SortFunctor>
// set limits on types and maybe even concepts
// look forward to thread safety
// look forward to array + - and so on operations on ArrayElement, for sortings like RadixSort
// write own generating function
class SortBench {
    SortStats stats;

    Functor<T> time_sort;
    Functor<ArrayElement<T>> cmp_asgn_sort;
public:

    operator()(std::vector<size_t> arrays_sizes)
    {
        for (auto size : arrays_sizes)
        {
            measure(size);
        }
    }

    operator()(size_t array_size, size_t msr_num)
    {
        while (msr_num--)
        {
            measure(array_size);
        }
    }

private:
    void measure(size_t size)
    {
        auto tm          {test_single_time(size)};
        auto [cmp, asgn] {test_single_cmp_asgn(size)};

        stats.push_back({size, tm, {cmp, asgn}});
    }

    std::pair<size_t, size_t> test_single_cmp_asgn(size_t len)
    {
        std::array<ArrayElement<T>, len> arr {GenFunc(len)};
        cmp_asgn_sort(arr.data(), arr.size());

        return {ArrayElement<T>::comparisons,
                ArrayElement<T>::assignments};
    }

    auto test_single_time(size_t len)
    {
        std::array<T, len> arr {GenFunc(len)};

        std::chrono::steady_clock::time_point start {std::chrono::steady_clock::now()};
        time_sort(arr.data(), arr.size());
        std::chrono::steady_clock::time_point end {std::chrono::steady_clock::now()};

        return end - start; // std::chrono:duration
    }
    
    template <typename T>
    class GenFunc
    {
        static constexpr size_t Tsz       {sizeof(T)};
        static constexpr size_t ui32sz    {sizeof(uint32_t)};
        static constexpr size_t buff_len  {Tsz < ui32sz ? 1 : Tsz / ui32sz};
        static constexpr size_t mersenne_twister_full_length_seed {624};

        std::mt19937 generator;
    public:

        GenFunc()
        {
            std::random_device rd;

            uint32_t numbers[mersenne_twister_full_length_seed];
            std::generate(numbers, std::end(numbers), std::ref(rd));

            SeedSeq sseq(numbers, std::end(numbers));
            generator = std::mt19937{sseq};
        }

        T operator()()
        {
            auto buffer {gen_buffer().data()};

            return *reinterpret_cast<T *>(buffer);
        }
    private:
        auto gen_buffer()
        {
            std::array<uint32_t, buff_len> arr {};
            std::generate(std::begin(arr), std::end(arr), std::ref(generator));

            return arr;
        }

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
    };
};

template <typename T>
class GenFunctor
{

}

};

#endif