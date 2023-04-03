#include <array>
#include <iostream>

struct CustomType {
    int val = 9;
    float a = 1.0f;
};

class CustomIterator {
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = CustomType;
    using pointer = CustomType*;
    using reference = CustomType&;

    pointer value_ptr;
public:
    struct sentinel{};
    explicit CustomIterator(pointer p) : value_ptr(p) {}
    reference operator*() const { return *value_ptr; }
    pointer operator->() { return value_ptr; }
    CustomIterator& operator++() { value_ptr++; return *this; }
    CustomIterator operator++(int) { CustomIterator tmp = *this; ++(*this); return tmp; }
    friend bool operator==(const CustomIterator& a, const CustomIterator& b) { return a.value_ptr == b.value_ptr; }
    friend bool operator!=(const CustomIterator& a, const CustomIterator& b) { return a.value_ptr != b.value_ptr; }

};

class CustomIterable {
    std::array<CustomType, 4> values{1, 2.3f, 3, 4.2f};
public:
    CustomIterator begin() { return CustomIterator(&values[0]); }
    CustomIterator end() { return CustomIterator(&values[3]+1); }
};

int main() {
    CustomIterable c;
    for(auto i : c) {
        std::cout << i.val << " " << i.a << "\n";
    }
    return 0;
}