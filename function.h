#ifndef FUNCTIONAL_FUNCTIONAL_H
#define FUNCTIONAL_FUNCTIONAL_H

#include <cstddef>
#include <memory>
#include <utility>

template<typename>
class function;

template<typename retT, typename ... argT>
class function<retT(argT ...)> {
private:
    class f_holder_base {
    public:
        f_holder_base() = default;

        virtual ~f_holder_base() = default;

        virtual retT call(argT ... args) = 0;
    };

    typedef std::unique_ptr<f_holder_base> call_t;

//_______________________________________________________________
    template<typename fT>
    class f_holder : public f_holder_base {
    public:
        f_holder(fT f) : f_holder_base(), _fun(f) {} //NOLINT

        retT call(argT ... args) override {
            return _fun(std::forward<argT>(args) ...);
        }

    private:
        fT _fun;
    };
//________________________________________________________________
private:
    static const size_t GAP_BYTE_SIZE = 32;

    union any_data {
        call_t _call;
        unsigned char buffer[GAP_BYTE_SIZE];

        any_data() {} //NOLINT
        ~any_data() {} //NOLINT
    } _data;

    bool is_small;
//________________________________________________________________
public:
    static size_t get_gap_size() {
        return GAP_BYTE_SIZE;
    }
//________________________________________________________________
public:
    function() noexcept {
        is_small = false;
        new(&_data._call) call_t(nullptr);
    }

    function(std::nullptr_t) noexcept { //NOLINT
        function();
    }

    function(const function &other) : is_small(other.is_small) {
        if (is_small) {
            std::copy(other._data.buffer,
                      other._data.buffer + GAP_BYTE_SIZE, _data.buffer);
        } else {
            new(&_data._call) call_t(other._data._call.get());
        }
    }

    function(function &&other) noexcept : is_small(other.is_small) {
        if (is_small) {
            std::copy(other._data.buffer,
                      other._data.buffer + GAP_BYTE_SIZE, _data.buffer);
        } else {
            new(&_data._call) call_t(std::move(other._data._call));
            new(&other._data._call) call_t(nullptr);
        }
    }

    template<typename fT>
    function(fT f) { //NOLINT
        if /*constexpr*/ (sizeof(f) <= GAP_BYTE_SIZE) {
            is_small = true;
            new(&_data.buffer) f_holder<fT>(f);
        } else {
            is_small = false;
            new(&_data._call) std::unique_ptr<f_holder<fT>>(new f_holder<fT>(f));
        }
    }

    ~function() {
        if (is_small) {
            (reinterpret_cast<f_holder_base *>(_data.buffer))->~f_holder_base();
        }
    }

    function &operator=(const function &other) {
        function tmp(other);
        swap(tmp);
        return *this;
    }

    function &operator=(function &&other) noexcept {
        if (!is_small) {
            _data._call.reset();
        }
        swap(other);
        return *this;
    }

    void swap(function &rhs) noexcept {
        if (is_small && rhs.is_small) {
            for (size_t i = 0; i < GAP_BYTE_SIZE; i++) {
                std::swap(_data.buffer[i], rhs._data.buffer[i]);
            }
        } else if (!is_small && !rhs.is_small) {
            _data._call.swap(rhs._data._call);
        } else if (is_small) {
            swap_small_big(_data, rhs._data);
        } else {
            swap_small_big(rhs._data, _data);
        }
        std::swap(is_small, rhs.is_small);
    }

    explicit operator bool() const noexcept {
        return is_small || _data._call;
    }

    retT operator()(argT ... args) const {
        if (is_small) {
            return (reinterpret_cast<f_holder_base *>(const_cast<unsigned char *>(_data.buffer)))->call(
                    std::forward<argT>(args)...);
        } else {
            return _data._call->call(std::forward<argT>(args) ...);
        }
    }
//_______________________________________________________________
private:
    void swap_small_big(any_data &small, any_data &big) {
        unsigned char tmp[GAP_BYTE_SIZE];
        std::copy(small.buffer, small.buffer + GAP_BYTE_SIZE, tmp);
        new(&small._call) call_t(big._call.get());
        big._call.reset();
        std::copy(tmp, tmp + GAP_BYTE_SIZE, big.buffer);
    }
};

#endif //FUNCTIONAL_FUNCTIONAL_H
