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

//Uses the 25.5 bit radix as recommended in the Curve25519 paper
using fe_25519 = std::array<uint32_t, 10>;

[[nodiscard]] ECCSDEF constexpr auto operator+(const fe_25519& a, const fe_25519& b);
[[nodiscard]] ECCSDEF constexpr auto operator-(const fe_25519& a, const fe_25519& b);
[[nodiscard]] ECCSDEF constexpr auto operator*(const fe_25519& a, const fe_25519& b);
[[nodiscard]] ECCSDEF constexpr auto operator^(const fe_25519& a, const fe_25519& b);
[[nodiscard]] ECCSDEF constexpr auto operator^(const fe_25519& a, uint32_t b);

} //namespace ecc

#endif //ECC_TOOLKIT_INCLUDE

#ifdef ECC_TOOLKIT_IMPLEMENTATION

namespace ecc::impl {

[[nodiscard]] ECCSDEF constexpr uint32_t mux(uint32_t c, uint32_t a, uint32_t b) {
    return b ^ (-c & (a ^ b));
}

[[nodiscard]] ECCSDEF constexpr uint32_t cmp(unsigned char *a, unsigned char *b, size_t len) {
    uint32_t r = 0;
    for (size_t i = 0; i < len; ++i) {
        r |= a[i] ^ b[i];
    }
    return (r == 0);
}

[[nodiscard]] ECCSDEF constexpr uint32_t max_limb_val(uint8_t limb_bit_length) {
    return (1 << limb_bit_length) - 1;
}

[[nodiscard]] ECCSDEF constexpr auto operator+(const fe_25519& a, const fe_25519& b) {
    fe_25519 r{0};
    uint32_t carry = 0;
    for (size_t i = 0; i < 9; ++i) {
        uint32_t res = a[i] + b[i] + carry;
        r[i] = res % max_limb_val(26 - (i & 1));
        carry = mux(r[i] == res, 0, 1);
    }
    r[10] = carry;
    return r;
}


} //namespace ecc::impl

#endif //ECC_TOOLKIT_IMPLEMENTATION

