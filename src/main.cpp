#include <Arduino.h>

#include <tuple>

// ============================================================
// 各种返回方法定义
// ============================================================

// 1. 结构体
struct SensorData {
    uint16_t temperature;
    uint16_t humidity;
    uint32_t timestamp;
    uint8_t status;
    float voltage;
};

// 2. 数组包装结构
struct ArrayData {
    uint8_t data[12]; // 与结构体大小匹配
};

// 3. 多个单独变量
struct SeparateVars {
    uint16_t temp;
    uint16_t hum;
    uint32_t ts;
    uint8_t stat;
    float volt;
};

// 4. 指针参数（修改现有变量）
void get_data_by_pointer(uint16_t* temp, uint16_t* hum, uint32_t* ts, uint8_t* stat, float* volt) {
    *temp = 1024;
    *hum  = 512;
    *ts   = millis();
    *stat = 1;
    *volt = 3.3f;
}

// 5. 引用参数（修改现有变量）
void get_data_by_reference(uint16_t& temp, uint16_t& hum, uint32_t& ts, uint8_t& stat, float& volt) {
    temp = 1024;
    hum  = 512;
    ts   = millis();
    stat = 1;
    volt = 3.3f;
}

// 6. 返回结构体指针（动态分配 - 不推荐但用于对比）
SensorData* get_data_dynamic() {
    SensorData* data  = new SensorData;
    data->temperature = 1024;
    data->humidity    = 512;
    data->timestamp   = millis();
    data->status      = 1;
    data->voltage     = 3.3f;
    return data;
}

// 7. 全局变量（传统方法）
uint16_t g_temp, g_hum;
uint32_t g_ts;
uint8_t g_stat;
float g_volt;

void get_data_global() {
    g_temp = 1024;
    g_hum  = 512;
    g_ts   = millis();
    g_stat = 1;
    g_volt = 3.3f;
}

// ============================================================
// 性能测试函数
// ============================================================

void benchmark_all_methods() {
    Serial.println(F("\n╔══════════════════════════════════════════════╗"));
    Serial.println(F("║     Multiple Return Value Methods Benchmark  ║"));
    Serial.println(F("╚══════════════════════════════════════════════╝"));

    const int ITERATIONS = 10000;
    volatile long total  = 0; // 防止优化

    // 预热
    delay(100);

    // ============ 1. 结构体返回值 ============
    {
        auto get_struct = []() -> SensorData {
            return { 1024, 512, (uint32_t)millis(), 1, 3.3f };
        };

        uint32_t start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            auto data = get_struct();
            total += data.temperature + data.humidity;
        }
        uint32_t end = micros();
        Serial.print(F("1. Struct return:        "));
        Serial.print(end - start);
        Serial.print(F(" us ("));
        Serial.print((float)(end - start) / ITERATIONS, 3);
        Serial.println(F(" us per call)"));
    }

    // ============ 2. 数组返回值 ============
    {
        auto get_array = []() -> ArrayData {
            ArrayData arr;
            memcpy(arr.data, "\x00\x04\x00\x02\x00\x00\x00\x01\x00\x00\x00\x00", 12);
            return arr;
        };

        uint32_t start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            auto arr = get_array();
            total += arr.data[0] + arr.data[2];
        }
        uint32_t end = micros();
        Serial.print(F("2. Array return:         "));
        Serial.print(end - start);
        Serial.print(F(" us ("));
        Serial.print((float)(end - start) / ITERATIONS, 3);
        Serial.println(F(" us per call)"));
    }

    // ============ 3. 单独变量（通过结构体） ============
    {
        auto get_separate = []() -> SeparateVars {
            return { 1024, 512, (uint32_t)millis(), 1, 3.3f };
        };

        uint32_t start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            auto vars = get_separate();
            total += vars.temp + vars.hum;
        }
        uint32_t end = micros();
        Serial.print(F("3. Separate vars:        "));
        Serial.print(end - start);
        Serial.print(F(" us ("));
        Serial.print((float)(end - start) / ITERATIONS, 3);
        Serial.println(F(" us per call)"));
    }

    // ============ 4. 指针参数 ============
    {
        uint32_t start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            uint16_t temp, hum;
            uint32_t ts;
            uint8_t stat;
            float volt;
            get_data_by_pointer(&temp, &hum, &ts, &stat, &volt);
            total += temp + hum;
        }
        uint32_t end = micros();
        Serial.print(F("4. Pointer parameters:   "));
        Serial.print(end - start);
        Serial.print(F(" us ("));
        Serial.print((float)(end - start) / ITERATIONS, 3);
        Serial.println(F(" us per call)"));
    }

    // ============ 5. 引用参数 ============
    {
        uint32_t start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            uint16_t temp, hum;
            uint32_t ts;
            uint8_t stat;
            float volt;
            get_data_by_reference(temp, hum, ts, stat, volt);
            total += temp + hum;
        }
        uint32_t end = micros();
        Serial.print(F("5. Reference params:     "));
        Serial.print(end - start);
        Serial.print(F(" us ("));
        Serial.print((float)(end - start) / ITERATIONS, 3);
        Serial.println(F(" us per call)"));
    }

    // ============ 6. MSD Tuple 返回值 ============
    {
        auto get_tuple = []() {
            return msd::make_tuple(
            (uint16_t)1024,
            (uint16_t)512,
            (uint32_t)millis(),
            (uint8_t)1,
            3.3f);
        };

        uint32_t start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            auto tup = get_tuple();
            total += msd::get<0>(tup) + msd::get<1>(tup);
        }
        uint32_t end = micros();
        Serial.print(F("6. MSD Tuple return:     "));
        Serial.print(end - start);
        Serial.print(F(" us ("));
        Serial.print((float)(end - start) / ITERATIONS, 3);
        Serial.println(F(" us per call)"));
    }

    // ============ 7. 动态分配 ============
    {
        uint32_t start = micros();
        for (int i = 0; i < ITERATIONS / 10; i++) { // 减少迭代，因为慢
            SensorData* data = get_data_dynamic();
            total += data->temperature + data->humidity;
            delete data;
        }
        uint32_t end = micros();
        Serial.print(F("7. Dynamic allocation:   "));
        Serial.print(end - start);
        Serial.print(F(" us (for "));
        Serial.print(ITERATIONS / 10);
        Serial.print(F(" iters, "));
        Serial.print((float)(end - start) / (ITERATIONS / 10), 3);
        Serial.println(F(" us per call)"));
    }

    // ============ 8. 全局变量 ============
    {
        uint32_t start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            get_data_global();
            total += g_temp + g_hum;
        }
        uint32_t end = micros();
        Serial.print(F("8. Global variables:     "));
        Serial.print(end - start);
        Serial.print(F(" us ("));
        Serial.print((float)(end - start) / ITERATIONS, 3);
        Serial.println(F(" us per call)"));
    }

    // ============ 9. Inline 结构体 ============
    {
        uint32_t start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            // 直接在调用处创建结构体
            struct InlineData {
                uint16_t t, h;
                uint32_t ts;
                uint8_t s;
                float v;
            } data = { 1024, 512, (uint32_t)millis(), 1, 3.3f };
            total += data.t + data.h;
        }
        uint32_t end = micros();
        Serial.print(F("9. Inline struct:        "));
        Serial.print(end - start);
        Serial.print(F(" us ("));
        Serial.print((float)(end - start) / ITERATIONS, 3);
        Serial.println(F(" us per call)"));
    }

    Serial.println(F("----------------------------------------"));
    Serial.print(F("Total (prevent optimization): "));
    Serial.println(total);
}

// ============================================================
// Tuple 详细性能测试
// ============================================================

void benchmark_tuple_detailed() {
    Serial.println(F("\n=== Detailed Tuple Benchmark ==="));

    const int ITERATIONS = 10000;
    volatile long total  = 0;

    // 测试1：创建时间
    {
        uint32_t start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            auto t = msd::make_tuple(i, i * 1.5f, i * 2.0, "test", i % 256);
            total += (int)msd::get<0>(t);
        }
        uint32_t end = micros();
        Serial.print(F("Tuple create time:       "));
        Serial.print(end - start);
        Serial.println(F(" us total"));
    }

    // 测试2：访问时间（各元素）
    {
        auto t         = msd::make_tuple(42, 3.14f, 2.718, "hello", (uint8_t)255);
        uint32_t start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            total += msd::get<0>(t);
            total += (int)msd::get<1>(t);
            total += (int)msd::get<2>(t);
            total += (int)msd::get<4>(t);
        }
        uint32_t end = micros();
        Serial.print(F("Tuple access time:       "));
        Serial.print(end - start);
        Serial.println(F(" us total"));
    }

    // 测试3：修改时间
    {
        auto t         = msd::make_tuple(0, 0.0f, 0.0, "", (uint8_t)0);
        uint32_t start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            msd::get<0>(t) = i;
            msd::get<1>(t) = i * 0.1f;
            msd::get<4>(t) = i % 256;
            total += msd::get<0>(t);
        }
        uint32_t end = micros();
        Serial.print(F("Tuple modify time:       "));
        Serial.print(end - start);
        Serial.println(F(" us total"));
    }

    // 测试4：拷贝时间
    {
        auto t1        = msd::make_tuple(1, 2.0f, 3.0, "src", (uint8_t)4);
        uint32_t start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            auto t2 = t1; // 拷贝构造
            total += msd::get<0>(t2);
        }
        uint32_t end = micros();
        Serial.print(F("Tuple copy time:         "));
        Serial.print(end - start);
        Serial.println(F(" us total"));
    }

    Serial.print(F("Total check: "));
    Serial.println(total);
}

// ============================================================
// Tuple vs Struct 对比测试
// ============================================================

void benchmark_tuple_vs_struct() {
    Serial.println(F("\n=== Tuple vs Struct Detailed Comparison ==="));

    const int ITERATIONS  = 5000;
    volatile long t_total = 0, s_total = 0;

    // 相同的数据类型
    using TupleType = decltype(msd::make_tuple(0, 0.0f, 0));

    struct StructType {
        int a;
        float b;
        int c;
    };

    // 创建性能
    {
        uint32_t t_start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            auto t = msd::make_tuple(i, i * 0.5f, i * 2);
            t_total += msd::get<0>(t);
        }
        uint32_t t_end = micros();

        uint32_t s_start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            StructType s = { i, i * 0.5f, i * 2 };
            s_total += s.a;
        }
        uint32_t s_end = micros();

        Serial.println(F("Creation performance:"));
        Serial.print(F("  Tuple: "));
        Serial.print(t_end - t_start);
        Serial.println(F(" us"));
        Serial.print(F("  Struct: "));
        Serial.print(s_end - s_start);
        Serial.println(F(" us"));
    }

    // 访问性能
    {
        TupleType t  = msd::make_tuple(42, 3.14f, 100);
        StructType s = { 42, 3.14f, 100 };

        uint32_t t_start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            t_total += msd::get<0>(t) + (int)msd::get<1>(t) + msd::get<2>(t);
        }
        uint32_t t_end = micros();

        uint32_t s_start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            s_total += s.a + (int)s.b + s.c;
        }
        uint32_t s_end = micros();

        Serial.println(F("Access performance:"));
        Serial.print(F("  Tuple: "));
        Serial.print(t_end - t_start);
        Serial.println(F(" us"));
        Serial.print(F("  Struct: "));
        Serial.print(s_end - s_start);
        Serial.println(F(" us"));
    }

    // 修改性能
    {
        TupleType t  = msd::make_tuple(0, 0.0f, 0);
        StructType s = { 0, 0.0f, 0 };

        uint32_t t_start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            msd::get<0>(t) = i;
            msd::get<1>(t) = i * 0.1f;
            msd::get<2>(t) = i * 2;
            t_total += msd::get<0>(t);
        }
        uint32_t t_end = micros();

        uint32_t s_start = micros();
        for (int i = 0; i < ITERATIONS; i++) {
            s.a = i;
            s.b = i * 0.1f;
            s.c = i * 2;
            s_total += s.a;
        }
        uint32_t s_end = micros();

        Serial.println(F("Modification performance:"));
        Serial.print(F("  Tuple: "));
        Serial.print(t_end - t_start);
        Serial.println(F(" us"));
        Serial.print(F("  Struct: "));
        Serial.print(s_end - s_start);
        Serial.println(F(" us"));
    }

    // 内存大小对比
    {
        Serial.println(F("Memory usage:"));
        Serial.print(F("  Tuple size: "));
        Serial.print(sizeof(TupleType));
        Serial.println(F(" bytes"));
        Serial.print(F("  Struct size: "));
        Serial.print(sizeof(StructType));
        Serial.println(F(" bytes"));

        // 对齐情况
        Serial.println(F("Alignment check:"));
        Serial.print(F("  Tuple alignment: "));
        Serial.println(alignof(TupleType));
        Serial.print(F("  Struct alignment: "));
        Serial.println(alignof(StructType));
    }

    Serial.print(F("Tuple total: "));
    Serial.println(t_total);
    Serial.print(F("Struct total: "));
    Serial.println(s_total);
}

// ============================================================
// 结构化绑定测试（C++17风格）
// ============================================================

#ifdef __cpp_structured_bindings
void test_structured_binding() {
    Serial.println(F("\n=== Structured Binding Test ==="));

    auto get_values = []() {
        return msd::make_tuple(42, 3.14f, "Hello");
    };

    // // 结构化绑定（模拟）
    // auto t = get_values();
    // auto a = msd::get<0>(t);
    // auto b = msd::get<1>(t);
    // auto c = msd::get<2>(t);
    auto [a, b, c] = get_values();

    Serial.print(F("Values: "));
    Serial.print(a);
    Serial.print(F(", "));
    Serial.print(b);
    Serial.print(F(", "));
    Serial.println(c);
}
#endif

// ============================================================
// 内存使用对比
// ============================================================

void compare_memory_usage() {
    Serial.println(F("\n=== Memory Usage Comparison ==="));

    // 结构体大小
    Serial.print(F("Struct size:          "));
    Serial.print(sizeof(SensorData));
    Serial.println(F(" bytes"));

    // 数组大小
    Serial.print(F("Array wrapper:        "));
    Serial.print(sizeof(ArrayData));
    Serial.println(F(" bytes"));

    // 单独变量结构体大小
    Serial.print(F("Separate struct:      "));
    Serial.print(sizeof(SeparateVars));
    Serial.println(F(" bytes"));

    // Tuple 大小
    auto tuple_sample = msd::make_tuple(
    (uint16_t)0,
    (uint16_t)0,
    (uint32_t)0,
    (uint8_t)0,
    0.0f);
    Serial.print(F("MSD Tuple size:       "));
    Serial.print(sizeof(decltype(tuple_sample)));
    Serial.println(F(" bytes"));

    // 复杂 Tuple 大小
    auto complex_tuple = msd::make_tuple(1, 2.0f, 3.0, "test", 'A', (uint16_t)100);
    Serial.print(F("Complex Tuple size:   "));
    Serial.print(sizeof(decltype(complex_tuple)));
    Serial.println(F(" bytes"));

    // 当前内存使用
    extern int __heap_start, *__brkval;
    int v;
    int free_memory = (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
    Serial.print(F("\nFree RAM:             "));
    Serial.print(free_memory);
    Serial.println(F(" bytes"));
}

// ============================================================
// 主程序
// ============================================================

void setup() {
    Serial.begin(115200);
    while (!Serial);
    delay(2000);

    Serial.println(F("\n╔════════════════════════════════════════════════╗"));
    Serial.println(F("║  Multi-Return Method Performance (with Tuple) ║"));
    Serial.println(F("║           Arduino Uno 16MHz                  ║"));
    Serial.println(F("╚════════════════════════════════════════════════╝"));

    Serial.print(F("C++ version: "));
    Serial.println(__cplusplus);

    // 初始内存
    extern int __heap_start, *__brkval;
    int v;
    int initial_memory = (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
    Serial.print(F("Initial free RAM: "));
    Serial.print(initial_memory);
    Serial.println(F(" bytes"));

    // 1. 所有方法性能对比
    benchmark_all_methods();

    // 2. Tuple 详细性能测试
    benchmark_tuple_detailed();

    // 3. Tuple vs Struct 详细对比
    benchmark_tuple_vs_struct();

    // 4. 内存使用对比
    compare_memory_usage();

#ifdef __cpp_structured_bindings
    test_structured_binding();
#endif

    // 最终内存
    int final_memory = (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
    Serial.print(F("\nFinal free RAM: "));
    Serial.print(final_memory);
    Serial.println(F(" bytes"));
}

void loop() {
    // 简单演示循环
    static int counter = 0;

    if (counter % 1000 == 0) {
        // 使用不同方法获取数据

        // 1. 使用结构体
        SensorData struct_data = { 1024, 512, millis(), 1, 3.3f };

        // 2. 使用指针参数
        uint16_t p_temp, p_hum;
        uint32_t p_ts;
        uint8_t p_stat;
        float p_volt;
        get_data_by_pointer(&p_temp, &p_hum, &p_ts, &p_stat, &p_volt);

        // 3. 使用Tuple
        auto tuple_data = msd::make_tuple(1024, 512, millis(), (uint8_t)1, 3.3f);

        // 验证数据一致性
        if (struct_data.temperature == p_temp &&
        struct_data.temperature == msd::get<0>(tuple_data)) {
            // 数据一致
        }
    }

    counter++;
    delay(1);
}