#pragma once

#include <ostream>
#include <map>

class Context;

class Expression {
public:
    int64_t number_;

    Expression() = default;
    explicit Expression(int64_t n);

    virtual void print(std::ostream& os) const = 0;
    virtual int64_t eval() const = 0;
    virtual int64_t eval(const Context&) const = 0;

    virtual ~Expression() = default;
};

class Val : public Expression {
public:
    explicit Val(int64_t n);

    void print(std::ostream& os) const final;
    int64_t eval() const final;
    int64_t eval(const Context&) const final;

    ~Val() override = default;
private:
};

class Add : public Expression {
public:
    Add(Expression* ex_l, Expression* ex_r);

    void print(std::ostream& os) const final;
    int64_t eval() const final;
    int64_t eval(const Context& ctx) const final;

    ~Add() override;
private:
    Expression* ex_l_;
    Expression* ex_r_;
};

class Mul : public Expression {
public:
    Mul(Expression* ex_l, Expression* ex_r);

    void print(std::ostream& os) const final;
    int64_t eval() const final;
    int64_t eval(const Context& ctx) const final;

    ~Mul() override;
private:
    Expression* ex_l_;
    Expression* ex_r_;
};

class Var : public Expression {
public:
    Var() = default;
    explicit Var(const std::string& var_name);

    void print(std::ostream& os) const final ;
    int64_t eval() const final;
    int64_t eval(const Context& ctx) const final;

    ~Var() override = default;

private:
    std::string name_;
};

class Context {
public:
    int64_t getVar(std::string const& name) const;
    void setVar(std::string const& name, int64_t value);
    bool varIsSet(std::string const& name) const;

private:
    std::map<std::string, Var> data_;
};