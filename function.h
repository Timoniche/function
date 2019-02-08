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

        virtual void fill_stack(void *address) = 0;

        virtual void deploy_pointer(void *address) = 0;
    };

    typedef std::unique_ptr<f_holder_base> call_t;

//_______________________________________________________________
    template<typename fT>
    class f_holder : public f_holder_base {
    public:
        f_holder(fT const &f) : f_holder_base(), _fun(f) {} //NOLINT

        f_holder(fT &&f) : f_holder_base(), _fun(std::move(f)) {} //NOLINT

        retT call(argT ... args) override {
            return _fun(std::forward<argT>(args) ...);
        }

        void fill_stack(void *address) override {
            new(address) f_holder<fT>(_fun);
        }

        void deploy_pointer(void *address) override {
            new(address) std::unique_ptr<f_holder<fT>>(new f_holder<fT>(_fun));
        }

    private:
        fT _fun;
    };
//________________________________________________________________
private:
    static const size_t GAP_BYTE_SIZE = 64;

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

    function(std::nullptr_t) noexcept {
        function();
    }

    function(const function &other) : is_small(other.is_small) {
        auto f = reinterpret_cast<f_holder_base *>(const_cast<unsigned char *>(other._data.buffer));
        if (is_small) {
            f->fill_stack(&_data.buffer);
        } else {
            f->deploy_pointer(&_data.buffer);
        }
    }

    function(function &&other) noexcept : is_small(other.is_small) {
        if (is_small) {
            auto f = reinterpret_cast<f_holder_base *>(other._data.buffer);
            f->fill_stack(&_data.buffer);
            new(&other._data.buffer) call_t(nullptr);
            other.is_small = false;
        } else {
            new(&_data._call) call_t(std::move(other._data._call));
        }
    }

    template<typename fT>
    function(fT f) { //NOLINT
        if constexpr (sizeof(f) <= GAP_BYTE_SIZE) {
            is_small = true;
            new(&_data.buffer) f_holder<fT>(std::move(f));
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
        if (is_small) {
            auto f = reinterpret_cast<f_holder_base *>(_data.buffer);
            f->~f_holder_base();
        } else {
            _data._call.reset();
        }
        if (other.is_small) {
            auto f = reinterpret_cast<f_holder_base *>(other._data.buffer);
            f->fill_stack(&_data.buffer);
            new(&other._data._call) call_t(nullptr);
        } else {
            new(&_data._call) call_t(std::move(other._data._call));
        }
        is_small = other.is_small;
        other.is_small = false;
        return *this;
    }

    void swap(function &rhs) noexcept {
        function tmp(std::move(rhs));
        rhs = std::move(*this);
        *this = std::move(tmp);
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
};

#endif //FUNCTIONAL_FUNCTIONAL_H
