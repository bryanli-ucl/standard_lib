#include <unity.h>

#include <string.h>
#include <string.hpp>

void test_string_constructors() {
    // 测试默认构造函数
    msd::string<10> str1;
    TEST_ASSERT_EQUAL(0, str1.size());
    TEST_ASSERT_EQUAL(10, str1.capacity());
    TEST_ASSERT_TRUE(str1.empty());
    TEST_ASSERT_EQUAL_STRING("", str1.c_str());

    // 测试从C字符串构造
    msd::string<10> str2("Hello");
    TEST_ASSERT_EQUAL(5, str2.size());
    TEST_ASSERT_EQUAL_STRING("Hello", str2.c_str());

    // 测试拷贝构造函数
    msd::string<10> str3(str2);
    TEST_ASSERT_EQUAL(5, str3.size());
    TEST_ASSERT_EQUAL_STRING("Hello", str3.c_str());

    // 测试移动构造函数
    msd::string<10> str4(msd::move(str2));
    TEST_ASSERT_EQUAL(5, str4.size());
    TEST_ASSERT_EQUAL_STRING("Hello", str4.c_str());
    TEST_ASSERT_EQUAL(0, str2.size());
    TEST_ASSERT_EQUAL_STRING("", str2.c_str());

    // 测试从C字符串构造时超过容量
    msd::string<5> str5("Hello World");
    TEST_ASSERT_EQUAL(5, str5.size());
    TEST_ASSERT_EQUAL_STRING("Hello", str5.c_str());
}

void test_string_assignment() {
    msd::string<10> str1("Hello");
    msd::string<10> str2;

    // 测试拷贝赋值
    str2 = str1;
    TEST_ASSERT_EQUAL(5, str2.size());
    TEST_ASSERT_EQUAL_STRING("Hello", str2.c_str());

    // 测试自赋值
    str2 = str2;
    TEST_ASSERT_EQUAL(5, str2.size());
    TEST_ASSERT_EQUAL_STRING("Hello", str2.c_str());

    // 测试移动赋值
    msd::string<10> str3;
    str3 = msd::move(str2);
    TEST_ASSERT_EQUAL(5, str3.size());
    TEST_ASSERT_EQUAL_STRING("Hello", str3.c_str());
    TEST_ASSERT_EQUAL(0, str2.size());

    // 测试从C字符串赋值
    msd::string<10> str4;
    str4 = "World";
    TEST_ASSERT_EQUAL(5, str4.size());
    TEST_ASSERT_EQUAL_STRING("World", str4.c_str());

    // 测试赋值时超过容量
    msd::string<5> str5;
    str5 = "Hello World";
    TEST_ASSERT_EQUAL(5, str5.size());
    TEST_ASSERT_EQUAL_STRING("Hello", str5.c_str());
}

void test_string_access() {
    msd::string<10> str("Hello");

    // 测试operator[]
    TEST_ASSERT_EQUAL('H', str[0]);
    TEST_ASSERT_EQUAL('e', str[1]);
    TEST_ASSERT_EQUAL('l', str[2]);
    TEST_ASSERT_EQUAL('l', str[3]);
    TEST_ASSERT_EQUAL('o', str[4]);

    // 测试修改通过operator[]
    str[0] = 'h';
    TEST_ASSERT_EQUAL('h', str[0]);
    TEST_ASSERT_EQUAL_STRING("hello", str.c_str());

    // 测试const operator[]
    const msd::string<10>& const_str = str;
    TEST_ASSERT_EQUAL('h', const_str[0]);

    // 测试at()
    TEST_ASSERT_EQUAL('h', str.at(0));
    TEST_ASSERT_EQUAL('e', str.at(1));

    // 测试at()越界访问
    char dummy = str.at(10);
    TEST_ASSERT_EQUAL('\0', dummy);

    // 测试const at()
    TEST_ASSERT_EQUAL('h', const_str.at(0));
}

void test_string_capacity() {
    msd::string<10> str;

    // 测试初始状态
    TEST_ASSERT_EQUAL(0, str.size());
    TEST_ASSERT_EQUAL(10, str.capacity());
    TEST_ASSERT_TRUE(str.empty());
    TEST_ASSERT_FALSE(str.full());

    // 测试部分填充
    str = "Hello";
    TEST_ASSERT_EQUAL(5, str.size());
    TEST_ASSERT_FALSE(str.empty());
    TEST_ASSERT_FALSE(str.full());

    // 测试满容量
    str = "1234567890";
    TEST_ASSERT_EQUAL(10, str.size());
    TEST_ASSERT_TRUE(str.full());
    TEST_ASSERT_FALSE(str.empty());

    // 测试clear()
    str.clear();
    TEST_ASSERT_EQUAL(0, str.size());
    TEST_ASSERT_TRUE(str.empty());
    TEST_ASSERT_EQUAL_STRING("", str.c_str());
}

void test_string_modifiers() {
    msd::string<10> str;

    // 测试push_back
    str.push_back('H');
    str.push_back('e');
    str.push_back('l');
    str.push_back('l');
    str.push_back('o');
    TEST_ASSERT_EQUAL(5, str.size());
    TEST_ASSERT_EQUAL_STRING("Hello", str.c_str());

    // 测试push_back到满容量
    str = "123456789";
    str.push_back('0');
    TEST_ASSERT_EQUAL(10, str.size());
    TEST_ASSERT_TRUE(str.full());
    str.push_back('X'); // 应该失败

    // 测试pop_back
    str.pop_back();
    TEST_ASSERT_EQUAL(9, str.size());
    TEST_ASSERT_EQUAL_STRING("123456789", str.c_str());

    // 测试多次pop_back
    str.pop_back();
    str.pop_back();
    TEST_ASSERT_EQUAL(7, str.size());
    TEST_ASSERT_EQUAL_STRING("1234567", str.c_str());

    // 测试从空字符串pop_back
    str.clear();
    str.pop_back(); // 应该没有效果
    TEST_ASSERT_EQUAL(0, str.size());
    TEST_ASSERT_EQUAL_STRING("", str.c_str());
}

void test_string_concatenation() {
    // 测试operator+= with string
    msd::string<10> str1("Hello");
    msd::string<10> str2(" World");
    str1 += str2;
    TEST_ASSERT_NOT_EQUAL(11, str1.size()); // 超过容量，会被截断
    TEST_ASSERT_EQUAL(10, str1.size());     // 实际大小
    TEST_ASSERT_EQUAL_STRING("Hello Worl", str1.c_str());

    // 测试operator+= with C string
    msd::string<10> str3("Test");
    str3 += "ing";
    TEST_ASSERT_EQUAL(7, str3.size());
    TEST_ASSERT_EQUAL_STRING("Testing", str3.c_str());

    // 测试operator+= with character
    msd::string<10> str4("AB");
    str4 += 'C';
    TEST_ASSERT_EQUAL(3, str4.size());
    TEST_ASSERT_EQUAL_STRING("ABC", str4.c_str());

    // 测试operator+= with character到满容量
    str4 = "123456789";
    str4 += '0';
    TEST_ASSERT_EQUAL(10, str4.size());
    TEST_ASSERT_TRUE(str4.full());
    str4 += 'X'; // 应该无效
    TEST_ASSERT_EQUAL(10, str4.size());

    // 测试operator+ with two strings - 注意这里需要匹配容量
    msd::string<5> str5("Hello");
    msd::string<5> str6("World");
    auto str7 = str5 + str6; // str7的类型是msd::string<10>
    TEST_ASSERT_EQUAL(10, str7.capacity());
    TEST_ASSERT_EQUAL(10, str7.size());
    TEST_ASSERT_EQUAL_STRING("HelloWorld", str7.c_str());

    // 测试operator+ with string and C string
    auto str8 = str5 + " World"; // str5是msd::string<5>，但返回的是msd::string<5>
    // 需要修正：使用相同容量的字符串
    msd::string<10> temp_str5("Hello");
    msd::string<10> str8_2 = temp_str5 + " World";
    TEST_ASSERT_EQUAL(10, str8_2.capacity());
    TEST_ASSERT_NOT_EQUAL(11, str8_2.size()); // 超过容量
    TEST_ASSERT_EQUAL(10, str8_2.size());     // 实际大小
    TEST_ASSERT_EQUAL_STRING("Hello Worl", str8_2.c_str());

    // 测试operator+ with C string and string
    msd::string<10> str9 = "Prefix " + temp_str5;
    TEST_ASSERT_EQUAL(10, str9.capacity());
    TEST_ASSERT_NOT_EQUAL(12, str9.size()); // 超过容量
    TEST_ASSERT_EQUAL(10, str9.size());     // 实际大小
    TEST_ASSERT_EQUAL_STRING("Prefix Hel", str9.c_str());
}

void test_string_iterators() {
    msd::string<10> str("Hello");

    // 测试非const迭代器
    size_t i = 0;
    for (auto it = str.begin(); it != str.end(); ++it) {
        TEST_ASSERT_EQUAL(str[i], *it);
        ++i;
    }
    TEST_ASSERT_EQUAL(5, i);

    // 测试const迭代器
    i = 0;
    for (auto it = str.begin(); it != str.end(); ++it) {
        TEST_ASSERT_EQUAL(str[i], *it);
        ++i;
    }

    // 测试基于范围的for循环
    i = 0;
    for (char c : str) {
        TEST_ASSERT_EQUAL(str[i], c);
        ++i;
    }

    // 测试空字符串的迭代器
    msd::string<10> empty_str;
    for (auto it = empty_str.begin(); it != empty_str.end(); ++it) {
        TEST_FAIL(); // 不应该执行到这里
    }
}

void test_string_edge_cases() {
    // 测试边界情况：空字符串的各种操作
    msd::string<10> str;
    TEST_ASSERT_EQUAL_STRING("", str.c_str());
    TEST_ASSERT_EQUAL('\0', str[0]);
    TEST_ASSERT_EQUAL('\0', str.at(0));

    // 测试单个字符的字符串
    str.push_back('A');
    TEST_ASSERT_EQUAL(1, str.size());
    TEST_ASSERT_EQUAL_STRING("A", str.c_str());

    // 测试正好满容量
    str = "1234567890";
    TEST_ASSERT_EQUAL(10, str.size());
    TEST_ASSERT_TRUE(str.full());
    TEST_ASSERT_EQUAL_STRING("1234567890", str.c_str());

    // 测试越界访问
    char c = str.at(10);
    TEST_ASSERT_EQUAL('\0', c);

    // 测试移动后源对象的有效性
    msd::string<10> str1("Hello");
    msd::string<10> str2(msd::move(str1));
    TEST_ASSERT_EQUAL(0, str1.size());
    TEST_ASSERT_EQUAL_STRING("", str1.c_str());
    TEST_ASSERT_EQUAL_STRING("Hello", str2.c_str());
}

void test_string_data_methods() {
    msd::string<10> str("Hello");

    // 测试data()方法
    char* data = str.data();
    TEST_ASSERT_EQUAL('H', data[0]);
    TEST_ASSERT_EQUAL('e', data[1]);
    TEST_ASSERT_EQUAL('l', data[2]);
    TEST_ASSERT_EQUAL('l', data[3]);
    TEST_ASSERT_EQUAL('o', data[4]);
    TEST_ASSERT_EQUAL('\0', data[5]);

    // 测试通过data()修改内容
    data[0] = 'h';
    TEST_ASSERT_EQUAL_STRING("hello", str.c_str());

    // 测试const data()
    const char* cdata = str.data();
    TEST_ASSERT_EQUAL('h', cdata[0]);

    // 测试c_str()
    const char* cstr = str.c_str();
    TEST_ASSERT_EQUAL_STRING("hello", cstr);
}