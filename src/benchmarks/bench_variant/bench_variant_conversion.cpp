/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014 - 2018 Axel Menzel <info@rttr.org>                           *
*                                                                                   *
*   This file is part of RTTR (Run Time Type Reflection)                            *
*   License: MIT License                                                            *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#include <rttr/type>
#include <rttr/registration>

#include <catch2/catch_all.hpp>

#include <locale>

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

static int setup_integer()
{
    return 12345;
}

/////////////////////////////////////////////////////////////////////////////////////////

static float setup_float()
{
    return 123.12345f;
}

/////////////////////////////////////////////////////////////////////////////////////////

static double setup_double()
{
    return 123456.123456;
}

/////////////////////////////////////////////////////////////////////////////////////////

static bool setup_bool()
{
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

static std::string setup_string_integer()
{
    return std::string("12345");
}

/////////////////////////////////////////////////////////////////////////////////////////

static std::string setup_string_float()
{
    return std::string("123.12345");
}

/////////////////////////////////////////////////////////////////////////////////////////

static std::string setup_string_double()
{
    return std::string("123456.123456");
}

/////////////////////////////////////////////////////////////////////////////////////////

static std::string setup_string_bool()
{
    return std::string("1");
}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*
nonius::benchmark bench_native_int_to_string()
{
    return nonius::benchmark("native", [](nonius::chronometer meter)
    {
        int var = setup_integer();
        std::string result;
        meter.measure([&]()
        {
            result = std::to_string(var);
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_int_to_string()
{
    return nonius::benchmark("rttr::variant", [](nonius::chronometer meter)
    {
        rttr::variant var = setup_integer();
        std::string result;
        meter.measure([&]()
        {
            result = var.to_string();
        });
    });
}
*/

TEST_CASE("bench_int_to_string") {

    BENCHMARK_ADVANCED("native")(Catch::Benchmark::Chronometer meter) {
        int var = setup_integer();
        std::string result;
        meter.measure([&]()
        {
            result = std::to_string(var);
        });
    };

    BENCHMARK_ADVANCED("rttr::variant")(Catch::Benchmark::Chronometer meter) {
        rttr::variant var = setup_integer();
        std::string result;
        meter.measure([&]()
        {
            result = var.to_string();
        });
    };
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*
nonius::benchmark bench_native_string_to_int()
{
    return nonius::benchmark("native", [](nonius::chronometer meter)
    {
        std::string number_string = setup_string_integer();
        int result = 0;
        meter.measure([&]()
        {
            std::size_t pos = 0;
            result = std::stoi(number_string, &pos);
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_string_to_int()
{
    return nonius::benchmark("rttr::variant", [](nonius::chronometer meter)
    {
        rttr::variant var = setup_string_integer();
        int result = 0;
        meter.measure([&]()
        {
            result = var.to_int();
        });
    });
}
*/

TEST_CASE("bench_string_to_int") {

    BENCHMARK_ADVANCED("native")(Catch::Benchmark::Chronometer meter) {
        std::string number_string = setup_string_integer();
        int result = 0;
        meter.measure([&]()
        {
            std::size_t pos = 0;
            result = std::stoi(number_string, &pos);
        });
    };

    BENCHMARK_ADVANCED("rttr::variant")(Catch::Benchmark::Chronometer meter) {
        rttr::variant var = setup_string_integer();
        int result = 0;
        meter.measure([&]()
        {
            result = var.to_int();
        });
    };
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/*
nonius::benchmark bench_native_float_to_string()
{
    return nonius::benchmark("native", [](nonius::chronometer meter)
    {
        float var = setup_float();
        std::string result;
        meter.measure([&]()
        {
            result = std::to_string(var);
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_float_to_string()
{
    return nonius::benchmark("rttr::variant", [](nonius::chronometer meter)
    {
        rttr::variant var = setup_float();
        std::string result;
        meter.measure([&]()
        {
            result = var.to_string();
        });
    });
}
*/

TEST_CASE("bench_float_to_string") {

    BENCHMARK_ADVANCED("native")(Catch::Benchmark::Chronometer meter) {
        float var = setup_float();
        std::string result;
        meter.measure([&]()
        {
            result = std::to_string(var);
        });
    };

    BENCHMARK_ADVANCED("rttr::variant")(Catch::Benchmark::Chronometer meter) {
        rttr::variant var = setup_float();
        std::string result;
        meter.measure([&]()
        {
            result = var.to_string();
        });
    };
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/*
nonius::benchmark bench_native_string_to_float()
{
    return nonius::benchmark("native", [](nonius::chronometer meter)
    {
        std::string number_string = setup_string_float();
        float result;
        meter.measure([&]()
        {
            std::size_t pos = 0;
            result = std::stof(number_string, &pos);
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_string_to_float()
{
    return nonius::benchmark("rttr::variant", [](nonius::chronometer meter)
    {
        rttr::variant var = setup_string_float();
        float result = 0.0f;
        meter.measure([&]()
        {
            result = var.to_float();
        });
    });
}
*/

TEST_CASE("bench_string_to_float") {

    BENCHMARK_ADVANCED("native")(Catch::Benchmark::Chronometer meter) {
        std::string number_string = setup_string_float();
        float result;
        meter.measure([&]()
        {
            std::size_t pos = 0;
            result = std::stof(number_string, &pos);
        });
    };

    BENCHMARK_ADVANCED("rttr::variant")(Catch::Benchmark::Chronometer meter) {
        rttr::variant var = setup_string_float();
        float result = 0.0f;
        meter.measure([&]()
        {
            result = var.to_float();
        });
    };
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*
nonius::benchmark bench_native_double_to_string()
{
    return nonius::benchmark("native", [](nonius::chronometer meter)
    {
        double var = setup_double();
        std::string result;
        meter.measure([&]()
        {
            result = std::to_string(var);
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_double_to_string()
{
    return nonius::benchmark("rttr::variant", [](nonius::chronometer meter)
    {
        rttr::variant var = setup_double();
        std::string result;
        meter.measure([&]()
        {
            result = var.to_string();
        });
    });
}
*/

TEST_CASE("bench_double_to_string") {

    BENCHMARK_ADVANCED("native")(Catch::Benchmark::Chronometer meter) {
        double var = setup_double();
        std::string result;
        meter.measure([&]()
        {
            result = std::to_string(var);
        });
    };

    BENCHMARK_ADVANCED("rttr::variant")(Catch::Benchmark::Chronometer meter) {
        rttr::variant var = setup_double();
        std::string result;
        meter.measure([&]()
        {
            result = var.to_string();
        });
    };
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*
nonius::benchmark bench_native_string_to_double()
{
    return nonius::benchmark("native", [](nonius::chronometer meter)
    {
        std::string number_string = setup_string_double();
        double result;
        meter.measure([&]()
        {
            std::size_t pos = 0;
            result = std::stod(number_string, &pos);
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_string_to_double()
{
    return nonius::benchmark("rttr::variant", [](nonius::chronometer meter)
    {
        rttr::variant var = setup_string_double();
        double result = 0.0;
        meter.measure([&]()
        {
            result = var.to_double();
        });
    });
}
*/

TEST_CASE("bench_string_to_double") {

    BENCHMARK_ADVANCED("native")(Catch::Benchmark::Chronometer meter) {
        std::string number_string = setup_string_double();
        double result;
        meter.measure([&]()
        {
            std::size_t pos = 0;
            result = std::stod(number_string, &pos);
        });
    };

    BENCHMARK_ADVANCED("rttr::variant")(Catch::Benchmark::Chronometer meter) {
        rttr::variant var = setup_string_double();
        double result = 0.0;
        meter.measure([&]()
        {
            result = var.to_double();
        });
    };
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/*
nonius::benchmark bench_native_bool_to_string()
{
    return nonius::benchmark("native", [](nonius::chronometer meter)
    {
        bool value = setup_bool();
        std::string result;
        meter.measure([&]()
        {
            result = value ? std::string("true") : std::string("false");
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_bool_to_string()
{
    return nonius::benchmark("rttr::variant", [](nonius::chronometer meter)
    {
        rttr::variant var = setup_bool();
        std::string result;
        meter.measure([&]()
        {
            result = var.to_string();
        });
    });
}
*/

TEST_CASE("bench_bool_to_string") {

    BENCHMARK_ADVANCED("native")(Catch::Benchmark::Chronometer meter) {
        bool value = setup_bool();
        std::string result;
        meter.measure([&]()
        {
            result = value ? std::string("true") : std::string("false");
        });
    };

    BENCHMARK_ADVANCED("rttr::variant")(Catch::Benchmark::Chronometer meter) {
        rttr::variant var = setup_bool();
        std::string result;
        meter.measure([&]()
        {
            result = var.to_string();
        });
    };
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

static bool string_to_bool(std::string text)
{
    std::transform(text.cbegin(), text.cend(), text.begin(), ::tolower);
    text.erase( std::remove_if( text.begin(), text.end(), []( char ch ) { return std::isspace<char>( ch, std::locale::classic() ); } ), text.end() );

    return !(text == "false" || text == "0" || text.empty());
}

/////////////////////////////////////////////////////////////////////////////////////////

/*
nonius::benchmark bench_native_string_to_bool()
{
    return nonius::benchmark("native", [](nonius::chronometer meter)
    {
        std::string bool_string = setup_string_bool();
        bool result = false;
        meter.measure([&]()
        {
            result = string_to_bool(bool_string);
        });
    });
}

/////////////////////////////////////////////////////////////////////////////////////////

nonius::benchmark bench_variant_string_to_bool()
{
    return nonius::benchmark("rttr::variant", [](nonius::chronometer meter)
    {
        rttr::variant var = setup_string_bool();
        bool result = false;
        meter.measure([&]()
        {
            result = var.to_bool();
        });
    });
}
*/

TEST_CASE("bench_string_to_bool") {

    BENCHMARK_ADVANCED("native")(Catch::Benchmark::Chronometer meter) {
        std::string bool_string = setup_string_bool();
        bool result = false;
        meter.measure([&]()
        {
            result = string_to_bool(bool_string);
        });
    };

    BENCHMARK_ADVANCED("rttr::variant")(Catch::Benchmark::Chronometer meter) {
        rttr::variant var = setup_string_bool();
        bool result = false;
        meter.measure([&]()
        {
            result = var.to_bool();
        });
    };
}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
