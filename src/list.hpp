#pragma once
#include <memory>
#include <iostream>
#include <exception>
#include <iterator>

template <class T>
class List {
    private:
        struct ListNode {
            std::unique_ptr<ListNode> _next;
            T _val;
            T& get(size_t idx);
            void insert(ListNode &prev, size_t idx,const T& val);
            void erase(ListNode &prev, size_t idx);
        };
        std::unique_ptr<ListNode> _head;
        size_t _size;
    public:
        using value_type = T;
        class ListIterator {
            private: 
                List& _list;
                size_t _idx;
                friend class List;
            public:
                using difference_type = int;
                using value_type = List::value_type;
                using reference = List::value_type&;
                using pointer = List::value_type*;
                using iterator_category = std::forward_iterator_tag;
                ListIterator(List& l, size_t idx) : _list(l), _idx(idx) {}
                ListIterator& operator++();
                reference  operator*();
                pointer operator->();
                bool operator!=(const ListIterator& other);
                bool operator==(const ListIterator& other);
        }; 
        List<T>::ListIterator begin();
        List<T>::ListIterator end();
        ListIterator insert(ListIterator it, T& val);
        ListIterator erase(ListIterator it);
        T& operator[](size_t idx);
        size_t size();
};

template <class T>
size_t List<T>::size() {
    return _size;
}

template <class T>
typename List<T>::ListIterator& List<T>::ListIterator::operator++() {
    ++_idx;
    return *this;
}

template <class T>
typename List<T>::ListIterator::reference List<T>::ListIterator::operator*() {
    return _list[_idx];
}

template <class T>
typename List<T>::ListIterator::pointer List<T>::ListIterator::operator->(){
    return &_list[_idx];
}

template <class T>
bool List<T>::ListIterator::operator!=(const ListIterator& other){
    if(_idx != other._idx) {
        return true;
    }
    if(&_list != &(other._list)) {
        return true;
    }
    return false;
}

template <class T>
bool List<T>::ListIterator::operator==(const ListIterator& other){
    if(_idx != other._idx) {
        return false;
    }
    if(&_list != &(other._list)) {
        return false;
    }
    return true;
}

template <class T>
T& List<T>::ListNode::get(size_t idx){
    if(idx == 0) {
        return _val;
    }
    if(_next) {
        return _next->get(--idx);
    }
    throw std::logic_error("Out of bounds"); 
}

template <class T>
void List<T>::ListNode::insert(ListNode &prev, size_t idx, const T& val) {
    if(idx == 0) {
        prev._next = std::make_unique<ListNode>(ListNode{std::move(prev._next), val});
        return;
    } else {
        return _next->insert(*this, --idx, val);
    }
    throw std::logic_error("Out of bounds"); 
}

template <class T>
void List<T>::ListNode::erase(ListNode &prev, size_t idx) {
    if(idx == 0) {
        prev._next = std::move(_next);
        return;
    } else {
        if(_next) {
            return _next->erase(*this, --idx);
        }
        throw std::logic_error("Out of bounds"); 
    }
}

template<class T>
typename List<T>::ListIterator List<T>::begin() {
    return ListIterator(*this, 0);
}

template<class T>
typename List<T>::ListIterator List<T>::end() {
    return ListIterator(*this, _size);
}

template<class T>
typename List<T>::ListIterator List<T>::insert(typename List<T>::ListIterator it, T& val) {
    if (it._idx > size()) {
        throw std::logic_error("Out of bounds");
    }
    if (it._idx == 0) {
        _head = std::make_unique<ListNode>(ListNode{std::move(_head), val});
        _size++;
    } else {
        _head->_next->insert(*_head,it._idx-1, val);
        _size++;
    }
    if (it._idx <= size()) {
        return it;
    }
    return ListIterator(*this, size());
}

template<class T>
T& List<T>::operator[](size_t idx){
    if(!_head) {
        throw std::logic_error("Out of bounds");
    }
    return _head->get(idx);
}

template <class T>
typename List<T>::ListIterator List<T>::erase(ListIterator iter) {
    if (iter._idx >= size()) {
        throw std::logic_error("Out of bounds");
    }
    if (iter._idx == 0){
        _head = std::move(_head->_next);
        _size--;
    } else {
        if (_head->_next) {
            _head->_next->erase(*_head,iter._idx-1);
            _size--;
        }
    }
    if(iter._idx < size()) {
        return iter;
    }
    return ListIterator(*this, size());
}
          