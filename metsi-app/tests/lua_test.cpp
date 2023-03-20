#define BOOST_TEST_MODULE lua_tester
#include <boost/test/unit_test.hpp>
#include <lua.hpp>

BOOST_AUTO_TEST_CASE(test_lua_function_call) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L, "test_resources/test.lua"); // execute given lua script file
    lua_getglobal(L, "increment"); // get a function to call into the local lua stack
    lua_pushinteger(L, 1); // push function argument to lua stack
    lua_pcall(L, 1, 1, 0); // call lua "increment"
    long result = lua_tointeger(L, -1); // value from top of stack as returned by lua "increment" function
    BOOST_CHECK(result == 2);
    lua_close(L);
}

BOOST_AUTO_TEST_CASE(test_host_function_call) {
    auto fn = [](lua_State* L) {
        long num = lua_tointeger(L, 1);
        long result = num + 1;
        lua_pushinteger(L, result); // push to lua stack
        return 1; // number of results
    };
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_register(L, "host_fn", fn); // register local function into lua namespace
    luaL_dofile(L, "test_resources/test.lua");
    lua_getglobal(L, "call_host_fn");
    lua_pushinteger(L, 1); // push value to stack. will be the argument for "fn" closure
    lua_pcall(L, 1, 1, 0); // call lua "call_host_fn", which will call our "fn" closure
    long result = lua_tointeger(L, -1); // value from top of stack, as returned by "call_host_fn", pushed in "fn"
    BOOST_CHECK(result == 2); // fn increments value by 1
    lua_close(L);
}