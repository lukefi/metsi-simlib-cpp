#define BOOST_TEST_MODULE lua_tester
#include <boost/test/unit_test.hpp>
#include <lua.hpp>

BOOST_AUTO_TEST_CASE(test_lua_function_call) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L, "test.lua");
    lua_getglobal(L, "increment");
    lua_pushinteger(L, 1);
    lua_pcall(L, 1, 1, 0);
    long result = lua_tointeger(L, -1);
    BOOST_CHECK(result == 2);
    lua_close(L);
}
