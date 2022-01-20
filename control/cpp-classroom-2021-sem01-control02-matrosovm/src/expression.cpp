#include <stdexcept>

#include "expression.hpp"

Expression::Expression(int64_t n) : number_(n) { }

Val::Val(int64_t n) : Expression(n) { }

void Val::print(std::ostream& os) const {
    os << number_;
}

int64_t Val::eval() const {
    return number_;
}

int64_t Val::eval(const Context&) const {
    return number_;
}

Add::Add(Expression* ex_l, Expression* ex_r) : ex_l_(ex_l), ex_r_(ex_r) {
    number_ = ex_l_->eval() + ex_r_->eval();
}

void Add::print(std::ostream& os) const {
    os << "(";
    ex_l_->print(os);
    os << " + ";
    ex_r_->print(os);
    os << ")";
}

int64_t Add::eval() const {
    return number_;
}

int64_t Add::eval(const Context& ctx) const {
    return ex_l_->eval(ctx) + ex_r_->eval(ctx);
}

Add::~Add() {
    delete ex_l_;
    delete ex_r_;
}

Mul::Mul(Expression *ex_l, Expression *ex_r) : ex_l_(ex_l), ex_r_(ex_r) {
    number_ = ex_l_->eval() * ex_r_->eval();
}

void Mul::print(std::ostream& os) const {
    ex_l_->print(os);
    os << " * ";
    ex_r_->print(os);
}

int64_t Mul::eval() const {
    return number_;
}

int64_t Mul::eval(const Context& ctx) const {
    return ex_l_->eval(ctx) * ex_r_->eval(ctx);
}

Mul::~Mul() {
    delete ex_l_;
    delete ex_r_;
}

Var::Var(const std::string& var_name) : name_(var_name) { }

void Var::print(std::ostream& os) const {
    os << name_;
}

int64_t Var::eval() const {
    return number_;
}

int64_t Var::eval(const Context& ctx) const {
    if (ctx.varIsSet(name_)) {
        return ctx.getVar(name_);
    }
    return 0;
}

int64_t Context::getVar(std::string const& name) const {
    try {
        return data_.at(name).number_;
    } catch (...) {
        throw std::runtime_error("var isn't in context");
    }
}

void Context::setVar(std::string const& name, int64_t value) {
    data_[name] = Var(name);
    data_[name].number_ = value;
}

bool Context::varIsSet(std::string const& name) const {
    try {
        data_.at(name);
        return true;
    } catch (...) {
        return false;
    }
}




























