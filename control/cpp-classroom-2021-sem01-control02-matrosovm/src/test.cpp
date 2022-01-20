#include <algorithm>
#include <iostream>
#include <sstream>
#include <type_traits>

#include "expression.hpp"

#define task_1
#define task_1_5
#define task_2
//#define task_3

using std::stringstream;
using std::string;

void assertWithThrow(bool condition, string const& msg) {
    if (!condition) {
        throw std::runtime_error(msg);
    }
}

string printToString(const Expression* e) {
    stringstream ss;
    e->print(ss);
    return ss.str();
}

template<typename T>
string wrapIfNotArithmetic(const T& val) {
    stringstream valueRepr;

    if constexpr (std::is_arithmetic_v<T>) {
        valueRepr << val;
    } else {
        valueRepr << '"' << val << '"';
    }

    return valueRepr.str();
}

template<typename T>
void assertEqualValues(const T& actual, const T& expected, const string& repr, const string& msg) {
    if (actual != expected) {
        stringstream reason;
        reason << "[expr " << repr << "]: " << msg;
        reason << ", expected " << wrapIfNotArithmetic(expected) << ", got " << wrapIfNotArithmetic(actual);
        throw std::runtime_error(reason.str());
    }
}

void assertEqualPrints(const string& actual, const string& expected, const string& repr) {
    assertEqualValues(actual, expected, repr, "print() mismatch");
}

void assertEqualEvals(int64_t actual, int64_t expected, const string& expressionRepr, const string& ctxRepr) {
    assertEqualValues(actual, expected, expressionRepr, "eval(<ctx>) mismatch with ctx=" + ctxRepr);
}

void assertEqualSimplifiedPrint(const string& actual, const string& expected, const string& expressionRepr, const string& ctxRepr) {
    assertEqualValues(actual, expected, expressionRepr, "simplified(<ctx>) print result mismatch with ctx=" + ctxRepr);
}


void task_1_smoke() {
#ifdef task_1
    {
        stringstream ss;
        Val val(10);

        val.print(ss);

        assertEqualPrints(ss.str(), "10", "Val(10)");
    }

    {
        Expression const* val = new Add(new Val(10), new Val(20));

        assertEqualPrints(printToString(val), "(10 + 20)", "Add(Val(10), Val(20))");

        delete val;
    }

    {
        Expression const* val = new Mul(new Val(10), new Val(20));

        assertEqualPrints(printToString(val), "10 * 20", "Mul(Val(10), Val(20))");

        delete val;
    }

    {
        Expression* sum = new Add(new Val(20), new Val(30));
        Expression const* mul = new Mul(new Val(10), sum);

        assertEqualPrints(printToString(mul),
                          "10 * (20 + 30)",
                          "Mul(Val(10), Add(Val(20), Val(30))");

        delete mul;
    }

    std::cout << "Smokes for task 1 passed" << std::endl;
#endif
}

void task_1_5_smoke() {
#ifdef task_1_5
    {
        Expression* val = new Val(10);

        assertEqualEvals(val->eval(), 10, "Val(10)", "{}");

        delete val;
    }

    {
        Expression* val = new Mul(new Val(10), new Val(20));

        assertEqualEvals(val->eval(), 200, "Mul(Val(10), Val(20))", "{}");

        delete val;
    }

    {
        Expression* sum = new Add(new Val(3), new Val(7));
        Expression* mul = new Mul(sum, new Val(20));

        assertEqualEvals(mul->eval(), 200, "Mul(Add(Val(3), Val(7)), Val(20))", "{}");

        delete mul;
    }

    std::cout << "Smokes for task 1.5 passed" << std::endl;
#endif
}

void task_2_smoke() {
#ifdef task_2
    {
        Expression* val = new Val(10);

        assertEqualEvals(val->eval(Context()), 10, "Val(10)", "{}");

        delete val;
    }

    {
        Context ctx;
        ctx.setVar("x", 100);

        Expression* var = new Var("x");
        assertEqualEvals(var->eval(ctx), 100, "Var(x)", "{x: 100}");

        delete var;
    }

    {
        Expression* sum = new Add(new Val(2), new Var("x"));
        Expression* mul = new Mul(sum, new Var("x"));

        assertEqualPrints(printToString(mul),
                          "(2 + x) * x",
                          "Mul(Add(Val(2), Var(\"x\")), Var(\"x\"))");

        Context ctx;
        ctx.setVar("x", 10);

        assertEqualEvals(mul->eval(ctx),
                         120,
                         "Mul(Add(Val(2), Var(\"x\")), Var(\"x\"))",
                         "{x: 10}");

        delete mul;
    }

    std::cout << "Smokes for task 2 passed" << std::endl;
#endif
}

void task_3_smoke() {
#ifdef task_3
    {
        Expression* val = new Val(10);
        Expression* simplifiedVal = val->simplified(Context());

        assertEqualSimplifiedPrint(printToString(simplifiedVal),
                                   "10",
                                   "Val(10)",
                                   "{}");

        assertWithThrow(val->eval() == 10, "Val(10), simplified in empty context, should be evaled to 10");

        delete simplifiedVal;
        delete val;
    }

    {
        Context ctx;
        ctx.setVar("x", 100);

        Expression* var = new Var("x");
        Expression* simplifiedVar = var->simplified(ctx);

        assertEqualSimplifiedPrint(printToString(simplifiedVar),
                                   "100",
                                   "Var(x)",
                                   "{x: 100}");

        assertWithThrow(
            simplifiedVar->eval() == 100,
            "Var(x) simplified with context, where x = 100, should be evaled to 100"
        );

        delete simplifiedVar;
        delete var;
    }

    {
        Context x_ctx;
        x_ctx.setVar("x", 2);

        Context y_ctx;
        y_ctx.setVar("y", 3);

        Expression* mul = new Mul(new Var("x"), new Var("y"));
        Expression* sum = new Add(mul, new Var("y"));
        const string exprRepr = "Add(Mul(Var(x), Var(y)), Var(y))";

        Expression* simplifiedWithX = sum->simplified(x_ctx);
        Expression* simplifiedWithY = sum->simplified(y_ctx);

        assertEqualSimplifiedPrint(printToString(simplifiedWithX),
                                   "(2 * y + y)",
                                   exprRepr,
                                   "{x: 2}");

        assertWithThrow(
            simplifiedWithX->eval(y_ctx) == 9,
            "x * y + y, simplified with {x = 2}, should be evaled to 5 in context {y = 3}"
        );

        assertEqualSimplifiedPrint(printToString(simplifiedWithY),
                                   "(x * 3 + 3)",
                                   exprRepr,
                                   "{y: 3}");

        assertWithThrow(
            simplifiedWithY->eval(x_ctx) == 9,
            "x * y + y, simplified with {y = 3}, should be evaled to 5 in context {x = 2}"
        );

        delete simplifiedWithY;
        delete simplifiedWithX;
        delete sum;
    }

    std::cout << "Smokes for task 3 passed" << std::endl;
#endif
}


int main() {
    task_1_smoke();
    task_1_5_smoke();
    task_2_smoke();
    task_3_smoke();
}
