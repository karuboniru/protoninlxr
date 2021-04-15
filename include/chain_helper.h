#pragma once

#include <TChain.h>
#include <TBranch.h>
#include <vector>
#include <string>
#include <is_cont.h>
#include <TObjString.h>

template <typename... branch_type>
class root_chain
{
private:
    TChain *chain{};
    std::tuple<decltype(new branch_type)...> data;
    std::array<TBranch *, sizeof...(branch_type)> b_add;
    const char *tree_name;

    template <class T, T I, T... J>
    inline void do_delete(std::integer_sequence<T, I, J...>)
    {
        do_delete(std::integer_sequence<std::size_t, I>{});
        do_delete(std::integer_sequence<std::size_t, J...>{});
    }
    template <class T, T I>
    inline void do_delete(std::integer_sequence<T, I>)
    {
        if (!is_cont<typename std::tuple_element<I, std::tuple<branch_type...>>::type>::value)
            delete[] std::get<I>(data);
    }

    template <class T, T I, T... J>
    inline void do_setaddress(std::integer_sequence<T, I, J...>, const std::array<const char *, sizeof...(branch_type)> names)
    {
        do_setaddress(std::integer_sequence<std::size_t, I>{}, names);
        do_setaddress(std::integer_sequence<std::size_t, J...>{}, names);
    }
    template <class T, T I>
    inline void do_setaddress(std::integer_sequence<T, I>, const std::array<const char *, sizeof...(branch_type)> names)
    {
        b_add[I] = chain->GetBranch(std::get<I>(names));
        assert(b_add[I]);
        if (!is_cont<typename std::tuple_element<I, std::tuple<branch_type...>>::type>::value)
        {
            // normal case, assign space and SetBranchAddress, the ugly expression
            // is for properly dealing with arrays, the assigned space is then freed
            // by me (not ROOT)
            std::get<I>(data) = new typename std::tuple_element<I, std::tuple<branch_type...>>::type;
            chain->SetBranchAddress(std::get<I>(names), std::get<I>(data), &b_add[I]);
            // folling method is broken, don't know why:
            // b_add[I]->SetAddress(std::get<I>(data));
        }
        else
        {
            // ok, seems for STL containers, CERN ROOT will only accept T** pointer, and
            // assign space on its own, free on its own
            std::get<I>(data) = nullptr;
            chain->SetBranchAddress(std::get<I>(names), &std::get<I>(data), &b_add[I]);
            // b_add[I]->SetAddress(std::get<I>(data));
        }
    }

public:
    root_chain(const std::vector<std::string> file_list, const char *mtree_name, const std::array<const char *, sizeof...(branch_type)> names) : tree_name(mtree_name)
    {
        chain = new TChain(tree_name);
        for (const auto &i : file_list)
        {
            chain->Add(i.c_str());
        }
        do_setaddress(std::make_integer_sequence<std::size_t, sizeof...(branch_type)>{}, names);
    }

    root_chain(const std::string file_list_path, const char *mtree_name, const std::array<const char *, sizeof...(branch_type)> names) : tree_name(mtree_name)
    {
        std::vector<std::string> file_list;
        std::fstream file(file_list_path);
        for (std::string i; file >> i; file_list.push_back(i))
        {
        }

        chain = new TChain(tree_name);
        for (const auto &i : file_list)
        {
            chain->Add(i.c_str());
        }
        do_setaddress(std::make_integer_sequence<std::size_t, sizeof...(branch_type)>{}, names);
    }

    ~root_chain()
    {
        chain->ResetBranchAddresses();
        // for (auto i : b_add)
        //     delete i;
        do_delete(std::make_integer_sequence<std::size_t, sizeof...(branch_type)>{});
        delete chain;
    }

    std::tuple<decltype(new branch_type)...> &get(std::size_t id)
    {
        auto curr = chain->LoadTree(id);
        if (curr < 0)
        {
            throw;
        }
        for (auto &i : b_add)
            i->GetEntry(curr);
        return data;
    }
    auto get_up()
    {
        return chain->GetEntries();
    }
    // used by range-based-for
    class iter
    {
    private:
        long num;
        root_chain *se;

    public:
        iter() {}
        iter &operator=(const iter &rhs)
        {
            num = rhs.num;
            se = rhs.se;
            return *this;
        }
        iter(int m_num, root_chain *root_chain_self)
        {
            se = root_chain_self;
            num = m_num;
        }
        iter &operator++()
        {
            num++;
            return *this;
        }
        bool operator!=(const iter &other) const
        {
            return num != other.num;
        }
        auto &operator*()
        {
            return se->get(num);
        }
    };
    auto begin()
    {
        return iter(0, this);
    }
    auto end()
    {
        return iter(get_up(), this);
    }
};

template <>
struct is_cont<TObjString> : std::integral_constant<bool, true> // lets treat TString as container (let ROOT to assign space)
{
};


typedef root_chain<int, int[100], int[100], double[100][4], double[100][4], TObjString>::iter my_iter_t;
// static auto chain_iter = chain.begin();
// static auto chain_iter_end = chain.end();

#include <mutex>
static std::mutex iterator_mutex;