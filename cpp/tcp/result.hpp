#pragma once

#include <functional>
#include <variant>

template <typename T, typename E>
class Result {
public:
    constexpr auto inline is_ok() -> bool {
        return this->inner.index() == 0;
    };
    constexpr auto inline is_err() -> bool { return !this->is_ok(); };
    constexpr auto inline unwrap() -> T {
        return std::get<0>(this->inner);
    };
    constexpr auto inline unwrap_err() -> E {
        return std::get<1>(this->inner);
    };

    template <typename U>
    constexpr auto inline map(std::function<U(T)> functor) -> Result<U, E> {
        if (this->is_err()) {
            return Result<U, E>::Err(this->unwrap_err());
        }
        return Result<U, E>::Ok(functor(this->unwrap()));
    };

    template <typename U>
    constexpr auto inline map_err(std::function<U(E)> functor) -> Result<T, U> {
        if (this->is_ok()) {
            return Result<T, U>::Ok(this->unwrap());
        }
        return Result<T, U>::Err(functor(this->unwrap_err()));
    };

    constexpr explicit operator bool() {
        return this->is_ok();
    }

    constexpr auto inline static Ok(T value) -> Result<T, E> {
        return Result { std::variant<T, E> { value } };
    }
    constexpr auto inline static Err(E error) -> Result<T, E> {
        return Result { std::variant<T, E> { error } };
    }

private:
    constexpr Result(std::variant<T, E> inner)
        : inner(inner) { };
    std::variant<T, E> inner;
};
