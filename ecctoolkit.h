#ifndef ECC_TOOLKIT_INCLUDE
#include <array>
#include <cstdint>
#include <cstdlib>

#ifndef ECCSDEF
#ifdef ECC_LINK_STATIC
#define ECCSDEF static
#else
#define ECCSDEF extern
#endif
#endif

namespace ecc {

    //31-bit radix
    struct fe_25519 {
        std::array<uint32_t, 10> data;
        constexpr decltype(auto) operator[](size_t i) const {
            return data[i];
        }
        constexpr decltype(auto) operator[](size_t i) {
            return data[i];
        }
    };

    [[nodiscard]] ECCSDEF constexpr fe_25519 operator+(const fe_25519& a, const fe_25519& b);
    [[nodiscard]] ECCSDEF constexpr fe_25519 operator-(const fe_25519& a, const fe_25519& b);
    [[nodiscard]] ECCSDEF constexpr fe_25519 operator*(const fe_25519& a, const fe_25519& b);
    [[nodiscard]] ECCSDEF constexpr fe_25519 operator^(const fe_25519& a, const fe_25519& b);
    [[nodiscard]] ECCSDEF constexpr fe_25519 operator^(const fe_25519& a, uint32_t b);

} //namespace ecc

#endif //ECC_TOOLKIT_INCLUDE

#ifdef ECC_TOOLKIT_IMPLEMENTATION

namespace ecc::impl {
    [[nodiscard]] ECCSDEF constexpr fe_25519 zero_element() {
        return {0, 0, 0, 0, 0, 0, 0, 0, 0};
    }
    [[nodiscard]] ECCSDEF constexpr fe_25519 prime_element() {
        return {0xffffffed, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
                0x7fffffff, 0x00000000, 0x00000000};
    }

    [[nodiscard]] ECCSDEF constexpr uint32_t nonzero(uint32_t a) {
        return (a | -a) >> 31;
    }
    [[nodiscard]] ECCSDEF constexpr uint32_t zero(uint32_t a) {
        return 1 ^ nonzero(a);
    }
    [[nodiscard]] ECCSDEF constexpr uint32_t neq(uint32_t a, uint32_t b) {
        return ((a - b) | (b - a)) >> 31;
    }

    [[nodiscard]] ECCSDEF constexpr uint32_t eq(uint32_t a, uint32_t b) {
        return 1 ^ neq(a, b);
    }

    [[nodiscard]] ECCSDEF constexpr uint32_t lt(uint32_t a, uint32_t b) {
        return (a ^ ((a ^ b) | ((a - b) ^ b))) >> 31;
    }

    [[nodiscard]] ECCSDEF constexpr uint32_t gt(uint32_t a, uint32_t b) {
        return lt(b, a);
    }

    [[nodiscard]] ECCSDEF constexpr uint32_t lte(uint32_t a, uint32_t b) {
        return 1 ^ gt(a, b);
    }

    [[nodiscard]] ECCSDEF constexpr uint32_t gte(uint32_t a, uint32_t b) {
        return 1 ^ lt(a, b);
    }

#if 0
    [[nodiscard]] ECCSDEF constexpr uint32_t mux(uint32_t c, uint32_t a, uint32_t b) {
        return b ^ (-c & (a ^ b));
    }
#else
    template<typename T>
    [[nodiscard]] ECCSDEF constexpr T mux(uint32_t c, T a, T b) {
        return b ^ (-c & (a ^ b));
    }
#endif

    [[nodiscard]] ECCSDEF constexpr uint32_t cmp(const void* x, const void* y, size_t len) {
        const unsigned char* a = (const unsigned char*) x;
        const unsigned char* b = (const unsigned char*) y;
        uint32_t r = 0;
        for (size_t i = 0; i < len; ++i) {
            r |= a[i] ^ b[i];
        }
        return zero(r);
    }

    [[nodiscard]] ECCSDEF constexpr uint32_t max_limb_val(uint8_t limb_bit_length) {
        return (1 << limb_bit_length) - 1;
    }


} //namespace ecc::impl

namespace ecc {
    using namespace ecc::impl;

    [[nodiscard]] ECCSDEF constexpr fe_25519 operator+(const fe_25519& a, const fe_25519& b) {
        fe_25519 r{0};
        int carry = 0;
        for (size_t i = 0; i < 9; ++i) {
            uint32_t res = a[i] + b[i] + carry;
            r[i] = res % max_limb_val(31);
            carry = mux(eq(r[i], res), 0, 1);
        }
        r[10] = carry;
        return r;
    }

    [[nodiscard]] ECCSDEF constexpr fe_25519 operator-(const fe_25519& a, const fe_25519& b) {
        fe_25519 z = zero_element();
        fe_25519 p = prime_element();
        fe_25519 r = *reinterpret_cast<decltype(&z)>(mux(cmp(a.data.data(), b.data.data(), a.data.size()), reinterpret_cast<uintptr_t>(&z), reinterpret_cast<uintptr_t>(&p)));

        int carry = 0;
        for (size_t i = 0; i < 9; ++i) {
            uint32_t res = a[i] - b[i] + carry;
            r[i] = res % max_limb_val(31);
            carry = mux(nonzero(res), 0, -1);
        }
        r[10] = carry;
        return r;
    }
}

#endif //ECC_TOOLKIT_IMPLEMENTATION
