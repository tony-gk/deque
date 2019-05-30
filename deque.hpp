#pragma once

#include <cstddef>
#include <unordered_map>

template<typename T>
struct node
{
    explicit node(T const &data,
                  node *prev = nullptr,
                  node *next = nullptr)
            : data(data), prev(prev), next(next)
    {};

    T data;
    node<T> *prev;
    node<T> *next;
};


template<typename T>
class deque
{
public:

    class iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T *;
        using reference         = T &;

        iterator() : node_(nullptr)
        {}

        explicit iterator(node<T> *node) : node_(node)
        {}

        reference operator*() const
        {
            return node_->data;
        }

        pointer operator->() const
        {
            return &node_->data;
        }

        bool operator==(iterator const &other) const
        {
            return node_ == other.node_;
        }

        bool operator!=(iterator const &other) const
        {
            return !operator==(other);
        }

        iterator &operator++()
        {
            node_ = node_->next;
            return *this;
        }

        const iterator operator++(int)
        {
            iterator tmp = *this;
            node_ = node_->next;
            return tmp;
        }

        iterator &operator--()
        {
            node_ = node_->prev;
            return *this;
        }

        const iterator operator--(int)
        {
            iterator tmp = *this;
            node_ = node_->prev;
            return tmp;
        }

        void swap(iterator &lhs, iterator &rhs)
        {
            node<T> temp = lhs.node_;
            lhs.node_ = rhs.node_;
            rhs.node_ = temp;
        }

    private:
        node<T> *node_;
    };

    deque() : size_(0)
    {
        past_the_end_ = new node<T>(0);
        head_ = tail_ = past_the_end_;
    };

    ~deque()
    {
        node<T> *cur = head_;
        while (cur->next)
        {
            cur = cur->next;
            delete cur->prev;
        }
        delete past_the_end_;
    }

    void push_front(T const &val)
    {
        if (size_++ == 0)
        {
            push_first(val);
            return;
        }
        auto new_node = new node<T>(val, nullptr, head_);
        head_->prev = new_node;
        head_ = new_node;
    }

    void push_back(T const &val)
    {
        if (size_++ == 0)
        {
            push_first(val);
            return;
        }
        auto new_node = new node<T>(val, tail_);
        tail_->next = new_node;
        tail_ = new_node;
        update_pte();
    }

    void pop_front()
    {
        if (size_-- == 1)
        {
            pop_last();
            return;
        }

        head_ = head_->next;
        delete head_->prev;
        head_->prev = nullptr;
    }

    void pop_back()
    {
        if (size_-- == 1)
        {
            pop_last();
            return;
        }

        tail_ = tail_->prev;
        delete tail_->next;
        update_pte();
    }

    T &front()
    {
        return head_->data;
    }

    T &back()
    {
        return tail_->data;
    }

    iterator begin() const
    {
        return deque::iterator(head_);
    }

    iterator end() const
    {
        return deque::iterator(past_the_end_);
    }

private:
    node<T> *head_;
    node<T> *tail_;
    node<T> *past_the_end_;
    size_t size_;

    void push_first(T const &val)
    {
        auto new_node = new node<T>(val);
        head_ = tail_ = new_node;
        update_pte();
    }

    void pop_last()
    {
        delete head_;
        head_ = tail_ = past_the_end_;
    }

    void update_pte()
    {
        tail_->next = past_the_end_;
        past_the_end_->prev = tail_;
    }
};
