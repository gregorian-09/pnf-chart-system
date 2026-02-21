/// \file pnf_java.cpp
/// \brief JNI bindings for the Java API.

#include <jni.h>
#include "pnf/pnf.hpp"
#include <string>
#include <vector>
#include <cstring>

using namespace pnf;

extern "C" {

// ==============================================
// Chart Native Methods
// ==============================================

JNIEXPORT jlong JNICALL
Java_com_pnf_Chart_nativeCreate(JNIEnv* env, jclass clazz) {
    Chart* chart = new Chart();
    return reinterpret_cast<jlong>(chart);
}

JNIEXPORT jlong JNICALL
Java_com_pnf_Chart_nativeCreateWithConfig(JNIEnv* env, jclass clazz,
                                          jint method, jint boxSizeMethod,
                                          jdouble boxSize, jint reversal) {
    ChartConfig config;
    config.method = static_cast<ConstructionMethod>(method);
    config.box_size_method = static_cast<BoxSizeMethod>(boxSizeMethod);
    config.box_size = boxSize;
    config.reversal = reversal;

    Chart* chart = new Chart(config);
    return reinterpret_cast<jlong>(chart);
}

JNIEXPORT void JNICALL
Java_com_pnf_Chart_nativeDestroy(JNIEnv* env, jclass clazz, jlong ptr) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    delete chart;
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Chart_nativeAddData(JNIEnv* env, jclass clazz, jlong ptr,
                                 jdouble high, jdouble low, jdouble close, jlong timestamp) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    Timestamp time = Timestamp(std::chrono::seconds(timestamp));
    return chart->add_data(high, low, close, time);
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Chart_nativeAddPrice(JNIEnv* env, jclass clazz, jlong ptr,
                                  jdouble price, jlong timestamp) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    Timestamp time = Timestamp(std::chrono::seconds(timestamp));
    return chart->add_data(price, time);
}

JNIEXPORT jint JNICALL
Java_com_pnf_Chart_nativeColumnCount(JNIEnv* env, jclass clazz, jlong ptr) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    return static_cast<jint>(chart->column_count());
}

JNIEXPORT jint JNICALL
Java_com_pnf_Chart_nativeXColumnCount(JNIEnv* env, jclass clazz, jlong ptr) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    return static_cast<jint>(chart->x_column_count());
}

JNIEXPORT jint JNICALL
Java_com_pnf_Chart_nativeOColumnCount(JNIEnv* env, jclass clazz, jlong ptr) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    return static_cast<jint>(chart->o_column_count());
}

JNIEXPORT jdouble JNICALL
Java_com_pnf_Chart_nativeBoxSize(JNIEnv* env, jclass clazz, jlong ptr) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    return chart->current_box_size();
}

JNIEXPORT jint JNICALL
Java_com_pnf_Chart_nativeColumnType(JNIEnv* env, jclass clazz, jlong ptr, jint index) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    Column* col = chart->column(index);
    if (col) {
        return static_cast<jint>(col->type());
    }
    return static_cast<jint>(ColumnType::X);
}

JNIEXPORT jint JNICALL
Java_com_pnf_Chart_nativeColumnBoxCount(JNIEnv* env, jclass clazz, jlong ptr, jint index) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    Column* col = chart->column(index);
    if (col) {
        return static_cast<jint>(col->box_count());
    }
    return 0;
}

JNIEXPORT jdouble JNICALL
Java_com_pnf_Chart_nativeColumnHighest(JNIEnv* env, jclass clazz, jlong ptr, jint index) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    Column* col = chart->column(index);
    if (col) {
        return col->highest_price();
    }
    return 0.0;
}

JNIEXPORT jdouble JNICALL
Java_com_pnf_Chart_nativeColumnLowest(JNIEnv* env, jclass clazz, jlong ptr, jint index) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    Column* col = chart->column(index);
    if (col) {
        return col->lowest_price();
    }
    return 0.0;
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Chart_nativeHasBullishBias(JNIEnv* env, jclass clazz, jlong ptr) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    return chart->has_bullish_bias();
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Chart_nativeHasBearishBias(JNIEnv* env, jclass clazz, jlong ptr) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    return chart->has_bearish_bias();
}

JNIEXPORT void JNICALL
Java_com_pnf_Chart_nativeClear(JNIEnv* env, jclass clazz, jlong ptr) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    chart->clear();
}

JNIEXPORT jstring JNICALL
Java_com_pnf_Chart_nativeToString(JNIEnv* env, jclass clazz, jlong ptr) {
    Chart* chart = reinterpret_cast<Chart*>(ptr);
    std::string str = chart->to_string();
    return env->NewStringUTF(str.c_str());
}

// ==============================================
// Indicators Native Methods
// ==============================================

JNIEXPORT jlong JNICALL
Java_com_pnf_Indicators_nativeCreate(JNIEnv* env, jclass clazz) {
    Indicators* indicators = new Indicators();
    return reinterpret_cast<jlong>(indicators);
}

JNIEXPORT jlong JNICALL
Java_com_pnf_Indicators_nativeCreateWithConfig(JNIEnv* env, jclass clazz,
        jint smaShortPeriod, jint smaMediumPeriod, jint smaLongPeriod,
        jint bollingerPeriod, jdouble bollingerStdDevs,
        jint rsiPeriod, jdouble rsiOverbought, jdouble rsiOversold,
        jdouble bullishAlertThreshold, jdouble bearishAlertThreshold,
        jdouble supportResistanceThreshold, jint congestionMinColumns, jdouble congestionPriceRange) {

    IndicatorConfig config;
    config.sma_short_period = smaShortPeriod;
    config.sma_medium_period = smaMediumPeriod;
    config.sma_long_period = smaLongPeriod;
    config.bollinger_period = bollingerPeriod;
    config.bollinger_std_devs = bollingerStdDevs;
    config.rsi_period = rsiPeriod;
    config.rsi_overbought = rsiOverbought;
    config.rsi_oversold = rsiOversold;
    config.bullish_alert_threshold = bullishAlertThreshold;
    config.bearish_alert_threshold = bearishAlertThreshold;
    config.support_resistance_threshold = supportResistanceThreshold;
    config.congestion_min_columns = congestionMinColumns;
    config.congestion_price_range = congestionPriceRange;

    Indicators* indicators = new Indicators(config);
    return reinterpret_cast<jlong>(indicators);
}

JNIEXPORT void JNICALL
Java_com_pnf_Indicators_nativeDestroy(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    delete indicators;
}

JNIEXPORT void JNICALL
Java_com_pnf_Indicators_nativeConfigure(JNIEnv* env, jclass clazz, jlong ptr,
        jint smaShortPeriod, jint smaMediumPeriod, jint smaLongPeriod,
        jint bollingerPeriod, jdouble bollingerStdDevs,
        jint rsiPeriod, jdouble rsiOverbought, jdouble rsiOversold,
        jdouble bullishAlertThreshold, jdouble bearishAlertThreshold,
        jdouble supportResistanceThreshold, jint congestionMinColumns, jdouble congestionPriceRange) {

    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    IndicatorConfig config;
    config.sma_short_period = smaShortPeriod;
    config.sma_medium_period = smaMediumPeriod;
    config.sma_long_period = smaLongPeriod;
    config.bollinger_period = bollingerPeriod;
    config.bollinger_std_devs = bollingerStdDevs;
    config.rsi_period = rsiPeriod;
    config.rsi_overbought = rsiOverbought;
    config.rsi_oversold = rsiOversold;
    config.bullish_alert_threshold = bullishAlertThreshold;
    config.bearish_alert_threshold = bearishAlertThreshold;
    config.support_resistance_threshold = supportResistanceThreshold;
    config.congestion_min_columns = congestionMinColumns;
    config.congestion_price_range = congestionPriceRange;

    indicators->configure(config);
}

JNIEXPORT void JNICALL
Java_com_pnf_Indicators_nativeCalculate(JNIEnv* env, jclass clazz, jlong ptr, jlong chartPtr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    Chart* chart = reinterpret_cast<Chart*>(chartPtr);
    indicators->calculate(*chart);
}

// Moving Averages
JNIEXPORT jdouble JNICALL
Java_com_pnf_Indicators_nativeSmaShort(JNIEnv* env, jclass clazz, jlong ptr, jint column) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->sma_short()->value(column);
}

JNIEXPORT jdouble JNICALL
Java_com_pnf_Indicators_nativeSmaMiddle(JNIEnv* env, jclass clazz, jlong ptr, jint column) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->sma_medium()->value(column);
}

JNIEXPORT jdouble JNICALL
Java_com_pnf_Indicators_nativeSmaLong(JNIEnv* env, jclass clazz, jlong ptr, jint column) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->sma_long()->value(column);
}

JNIEXPORT jdoubleArray JNICALL
Java_com_pnf_Indicators_nativeSmaShortValues(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    const std::vector<double>& values = indicators->sma_short()->values();

    jdoubleArray result = env->NewDoubleArray(values.size());
    if (result) {
        env->SetDoubleArrayRegion(result, 0, values.size(), values.data());
    }
    return result;
}

JNIEXPORT jdoubleArray JNICALL
Java_com_pnf_Indicators_nativeSmaMediumValues(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    const std::vector<double>& values = indicators->sma_medium()->values();

    jdoubleArray result = env->NewDoubleArray(values.size());
    if (result) {
        env->SetDoubleArrayRegion(result, 0, values.size(), values.data());
    }
    return result;
}

JNIEXPORT jdoubleArray JNICALL
Java_com_pnf_Indicators_nativeSmaLongValues(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    const std::vector<double>& values = indicators->sma_long()->values();

    jdoubleArray result = env->NewDoubleArray(values.size());
    if (result) {
        env->SetDoubleArrayRegion(result, 0, values.size(), values.data());
    }
    return result;
}

// Bollinger Bands
JNIEXPORT jdouble JNICALL
Java_com_pnf_Indicators_nativeBollingerMiddle(JNIEnv* env, jclass clazz, jlong ptr, jint column) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->bollinger()->middle(column);
}

JNIEXPORT jdouble JNICALL
Java_com_pnf_Indicators_nativeBollingerUpper(JNIEnv* env, jclass clazz, jlong ptr, jint column) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->bollinger()->upper(column);
}

JNIEXPORT jdouble JNICALL
Java_com_pnf_Indicators_nativeBollingerLower(JNIEnv* env, jclass clazz, jlong ptr, jint column) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->bollinger()->lower(column);
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Indicators_nativeHasBollingerValue(JNIEnv* env, jclass clazz, jlong ptr, jint column) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->bollinger()->has_value(column);
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Indicators_nativeIsAboveUpperBand(JNIEnv* env, jclass clazz, jlong ptr, jint column, jdouble price) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->bollinger()->is_above_upper(column, price);
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Indicators_nativeIsBelowLowerBand(JNIEnv* env, jclass clazz, jlong ptr, jint column, jdouble price) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->bollinger()->is_below_lower(column, price);
}

JNIEXPORT jdoubleArray JNICALL
Java_com_pnf_Indicators_nativeBollingerMiddleValues(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    const std::vector<double>& values = indicators->bollinger()->middle_band();

    jdoubleArray result = env->NewDoubleArray(values.size());
    if (result) {
        env->SetDoubleArrayRegion(result, 0, values.size(), values.data());
    }
    return result;
}

JNIEXPORT jdoubleArray JNICALL
Java_com_pnf_Indicators_nativeBollingerUpperValues(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    const std::vector<double>& values = indicators->bollinger()->upper_band();

    jdoubleArray result = env->NewDoubleArray(values.size());
    if (result) {
        env->SetDoubleArrayRegion(result, 0, values.size(), values.data());
    }
    return result;
}

JNIEXPORT jdoubleArray JNICALL
Java_com_pnf_Indicators_nativeBollingerLowerValues(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    const std::vector<double>& values = indicators->bollinger()->lower_band();

    jdoubleArray result = env->NewDoubleArray(values.size());
    if (result) {
        env->SetDoubleArrayRegion(result, 0, values.size(), values.data());
    }
    return result;
}

// RSI
JNIEXPORT jdouble JNICALL
Java_com_pnf_Indicators_nativeRsi(JNIEnv* env, jclass clazz, jlong ptr, jint column) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->rsi()->value(column);
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Indicators_nativeHasRsiValue(JNIEnv* env, jclass clazz, jlong ptr, jint column) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->rsi()->has_value(column);
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Indicators_nativeIsOverbought(JNIEnv* env, jclass clazz, jlong ptr, jint column) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->rsi()->is_overbought(column);
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Indicators_nativeIsOversold(JNIEnv* env, jclass clazz, jlong ptr, jint column) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->rsi()->is_oversold(column);
}

JNIEXPORT jdoubleArray JNICALL
Java_com_pnf_Indicators_nativeRsiValues(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    const std::vector<double>& values = indicators->rsi()->values();

    jdoubleArray result = env->NewDoubleArray(values.size());
    if (result) {
        env->SetDoubleArrayRegion(result, 0, values.size(), values.data());
    }
    return result;
}

// OBV
JNIEXPORT jdouble JNICALL
Java_com_pnf_Indicators_nativeObv(JNIEnv* env, jclass clazz, jlong ptr, jint column) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->obv()->value(column);
}

JNIEXPORT jdoubleArray JNICALL
Java_com_pnf_Indicators_nativeObvValues(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    const std::vector<double>& values = indicators->obv()->values();

    jdoubleArray result = env->NewDoubleArray(values.size());
    if (result) {
        env->SetDoubleArrayRegion(result, 0, values.size(), values.data());
    }
    return result;
}

// Bullish Percent
JNIEXPORT jdouble JNICALL
Java_com_pnf_Indicators_nativeBullishPercent(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->bullish_percent()->value();
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Indicators_nativeIsBullishAlert(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->bullish_percent()->is_bullish_alert();
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Indicators_nativeIsBearishAlert(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->bullish_percent()->is_bearish_alert();
}

// Signals
JNIEXPORT jint JNICALL
Java_com_pnf_Indicators_nativeCurrentSignal(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return static_cast<jint>(indicators->signals()->current_signal());
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Indicators_nativeHasBuySignal(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->signals()->has_buy_signal();
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Indicators_nativeHasSellSignal(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->signals()->has_sell_signal();
}

JNIEXPORT jint JNICALL
Java_com_pnf_Indicators_nativeSignalCount(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return static_cast<jint>(indicators->signals()->signals().size());
}

JNIEXPORT jint JNICALL
Java_com_pnf_Indicators_nativeBuySignalCount(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return static_cast<jint>(indicators->signals()->buy_count());
}

JNIEXPORT jint JNICALL
Java_com_pnf_Indicators_nativeSellSignalCount(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return static_cast<jint>(indicators->signals()->sell_count());
}

JNIEXPORT jobjectArray JNICALL
Java_com_pnf_Indicators_nativeGetSignals(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    const std::vector<Signal>& signals = indicators->signals()->signals();

    jclass signalClass = env->FindClass("com/pnf/Signal");
    jclass signalTypeClass = env->FindClass("com/pnf/SignalType");
    jmethodID signalConstructor = env->GetMethodID(signalClass, "<init>", "(Lcom/pnf/SignalType;ID)V");
    jmethodID signalTypeValues = env->GetStaticMethodID(signalTypeClass, "values", "()[Lcom/pnf/SignalType;");

    jobjectArray typeEnums = (jobjectArray)env->CallStaticObjectMethod(signalTypeClass, signalTypeValues);

    jobjectArray result = env->NewObjectArray(signals.size(), signalClass, nullptr);
    for (size_t i = 0; i < signals.size(); i++) {
        jobject signalType = env->GetObjectArrayElement(typeEnums, static_cast<jint>(signals[i].type));
        jobject signal = env->NewObject(signalClass, signalConstructor,
                                       signalType, signals[i].column_index, signals[i].price);
        env->SetObjectArrayElement(result, i, signal);
        env->DeleteLocalRef(signal);
        env->DeleteLocalRef(signalType);
    }

    env->DeleteLocalRef(typeEnums);
    return result;
}

JNIEXPORT jobjectArray JNICALL
Java_com_pnf_Indicators_nativeGetBuySignals(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::vector<Signal> signals = indicators->signals()->buy_signals();

    jclass signalClass = env->FindClass("com/pnf/Signal");
    jclass signalTypeClass = env->FindClass("com/pnf/SignalType");
    jmethodID signalConstructor = env->GetMethodID(signalClass, "<init>", "(Lcom/pnf/SignalType;ID)V");
    jmethodID signalTypeValues = env->GetStaticMethodID(signalTypeClass, "values", "()[Lcom/pnf/SignalType;");

    jobjectArray typeEnums = (jobjectArray)env->CallStaticObjectMethod(signalTypeClass, signalTypeValues);

    jobjectArray result = env->NewObjectArray(signals.size(), signalClass, nullptr);
    for (size_t i = 0; i < signals.size(); i++) {
        jobject signalType = env->GetObjectArrayElement(typeEnums, static_cast<jint>(signals[i].type));
        jobject signal = env->NewObject(signalClass, signalConstructor,
                                       signalType, signals[i].column_index, signals[i].price);
        env->SetObjectArrayElement(result, i, signal);
        env->DeleteLocalRef(signal);
        env->DeleteLocalRef(signalType);
    }

    env->DeleteLocalRef(typeEnums);
    return result;
}

JNIEXPORT jobjectArray JNICALL
Java_com_pnf_Indicators_nativeGetSellSignals(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::vector<Signal> signals = indicators->signals()->sell_signals();

    jclass signalClass = env->FindClass("com/pnf/Signal");
    jclass signalTypeClass = env->FindClass("com/pnf/SignalType");
    jmethodID signalConstructor = env->GetMethodID(signalClass, "<init>", "(Lcom/pnf/SignalType;ID)V");
    jmethodID signalTypeValues = env->GetStaticMethodID(signalTypeClass, "values", "()[Lcom/pnf/SignalType;");

    jobjectArray typeEnums = (jobjectArray)env->CallStaticObjectMethod(signalTypeClass, signalTypeValues);

    jobjectArray result = env->NewObjectArray(signals.size(), signalClass, nullptr);
    for (size_t i = 0; i < signals.size(); i++) {
        jobject signalType = env->GetObjectArrayElement(typeEnums, static_cast<jint>(signals[i].type));
        jobject signal = env->NewObject(signalClass, signalConstructor,
                                       signalType, signals[i].column_index, signals[i].price);
        env->SetObjectArrayElement(result, i, signal);
        env->DeleteLocalRef(signal);
        env->DeleteLocalRef(signalType);
    }

    env->DeleteLocalRef(typeEnums);
    return result;
}

// Patterns
JNIEXPORT jint JNICALL
Java_com_pnf_Indicators_nativePatternCount(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->patterns()->pattern_count();
}

JNIEXPORT jint JNICALL
Java_com_pnf_Indicators_nativeBullishPatternCount(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->patterns()->bullish_count();
}

JNIEXPORT jint JNICALL
Java_com_pnf_Indicators_nativeBearishPatternCount(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->patterns()->bearish_count();
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Indicators_nativeHasPattern(JNIEnv* env, jclass clazz, jlong ptr, jint patternType) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->patterns()->has_pattern(static_cast<PatternType>(patternType));
}

JNIEXPORT jobjectArray JNICALL
Java_com_pnf_Indicators_nativeGetPatterns(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    const std::vector<Pattern>& patterns = indicators->patterns()->patterns();

    jclass patternClass = env->FindClass("com/pnf/Pattern");
    jclass patternTypeClass = env->FindClass("com/pnf/PatternType");
    jmethodID patternConstructor = env->GetMethodID(patternClass, "<init>", "(Lcom/pnf/PatternType;IDZ)V");
    jmethodID patternTypeValues = env->GetStaticMethodID(patternTypeClass, "values", "()[Lcom/pnf/PatternType;");

    jobjectArray typeEnums = (jobjectArray)env->CallStaticObjectMethod(patternTypeClass, patternTypeValues);

    jobjectArray result = env->NewObjectArray(patterns.size(), patternClass, nullptr);
    for (size_t i = 0; i < patterns.size(); i++) {
        jobject patternType = env->GetObjectArrayElement(typeEnums, static_cast<jint>(patterns[i].type));
        jboolean isBullish = is_bullish_pattern(patterns[i].type);
        jobject pattern = env->NewObject(patternClass, patternConstructor,
                                        patternType, patterns[i].start_column, patterns[i].price, isBullish);
        env->SetObjectArrayElement(result, i, pattern);
        env->DeleteLocalRef(pattern);
        env->DeleteLocalRef(patternType);
    }

    env->DeleteLocalRef(typeEnums);
    return result;
}

JNIEXPORT jobjectArray JNICALL
Java_com_pnf_Indicators_nativeGetBullishPatterns(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::vector<Pattern> patterns = indicators->patterns()->bullish_patterns();

    jclass patternClass = env->FindClass("com/pnf/Pattern");
    jclass patternTypeClass = env->FindClass("com/pnf/PatternType");
    jmethodID patternConstructor = env->GetMethodID(patternClass, "<init>", "(Lcom/pnf/PatternType;IDZ)V");
    jmethodID patternTypeValues = env->GetStaticMethodID(patternTypeClass, "values", "()[Lcom/pnf/PatternType;");

    jobjectArray typeEnums = (jobjectArray)env->CallStaticObjectMethod(patternTypeClass, patternTypeValues);

    jobjectArray result = env->NewObjectArray(patterns.size(), patternClass, nullptr);
    for (size_t i = 0; i < patterns.size(); i++) {
        jobject patternType = env->GetObjectArrayElement(typeEnums, static_cast<jint>(patterns[i].type));
        jobject pattern = env->NewObject(patternClass, patternConstructor,
                                        patternType, patterns[i].start_column, patterns[i].price, JNI_TRUE);
        env->SetObjectArrayElement(result, i, pattern);
        env->DeleteLocalRef(pattern);
        env->DeleteLocalRef(patternType);
    }

    env->DeleteLocalRef(typeEnums);
    return result;
}

JNIEXPORT jobjectArray JNICALL
Java_com_pnf_Indicators_nativeGetBearishPatterns(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::vector<Pattern> patterns = indicators->patterns()->bearish_patterns();

    jclass patternClass = env->FindClass("com/pnf/Pattern");
    jclass patternTypeClass = env->FindClass("com/pnf/PatternType");
    jmethodID patternConstructor = env->GetMethodID(patternClass, "<init>", "(Lcom/pnf/PatternType;IDZ)V");
    jmethodID patternTypeValues = env->GetStaticMethodID(patternTypeClass, "values", "()[Lcom/pnf/PatternType;");

    jobjectArray typeEnums = (jobjectArray)env->CallStaticObjectMethod(patternTypeClass, patternTypeValues);

    jobjectArray result = env->NewObjectArray(patterns.size(), patternClass, nullptr);
    for (size_t i = 0; i < patterns.size(); i++) {
        jobject patternType = env->GetObjectArrayElement(typeEnums, static_cast<jint>(patterns[i].type));
        jobject pattern = env->NewObject(patternClass, patternConstructor,
                                        patternType, patterns[i].start_column, patterns[i].price, JNI_FALSE);
        env->SetObjectArrayElement(result, i, pattern);
        env->DeleteLocalRef(pattern);
        env->DeleteLocalRef(patternType);
    }

    env->DeleteLocalRef(typeEnums);
    return result;
}

// Support/Resistance
JNIEXPORT jobjectArray JNICALL
Java_com_pnf_Indicators_nativeGetLevels(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    const std::vector<SupportResistanceLevel>& levels = indicators->support_resistance()->levels();

    jclass levelClass = env->FindClass("com/pnf/SupportResistanceLevel");
    jclass levelTypeClass = env->FindClass("com/pnf/LevelType");
    jmethodID levelConstructor = env->GetMethodID(levelClass, "<init>", "(Lcom/pnf/LevelType;DIII)V");
    jfieldID supportField = env->GetStaticFieldID(levelTypeClass, "SUPPORT", "Lcom/pnf/LevelType;");
    jfieldID resistanceField = env->GetStaticFieldID(levelTypeClass, "RESISTANCE", "Lcom/pnf/LevelType;");

    jobject supportType = env->GetStaticObjectField(levelTypeClass, supportField);
    jobject resistanceType = env->GetStaticObjectField(levelTypeClass, resistanceField);

    jobjectArray result = env->NewObjectArray(levels.size(), levelClass, nullptr);
    for (size_t i = 0; i < levels.size(); i++) {
        jobject levelType = levels[i].is_support ? supportType : resistanceType;
        jobject level = env->NewObject(levelClass, levelConstructor,
                                      levelType, levels[i].price, levels[i].touch_count,
                                      levels[i].first_column, levels[i].last_column);
        env->SetObjectArrayElement(result, i, level);
        env->DeleteLocalRef(level);
    }

    env->DeleteLocalRef(supportType);
    env->DeleteLocalRef(resistanceType);
    return result;
}

JNIEXPORT jobjectArray JNICALL
Java_com_pnf_Indicators_nativeGetSupportLevels(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::vector<SupportResistanceLevel> levels = indicators->support_resistance()->support_levels();

    jclass levelClass = env->FindClass("com/pnf/SupportResistanceLevel");
    jclass levelTypeClass = env->FindClass("com/pnf/LevelType");
    jmethodID levelConstructor = env->GetMethodID(levelClass, "<init>", "(Lcom/pnf/LevelType;DIII)V");
    jfieldID supportField = env->GetStaticFieldID(levelTypeClass, "SUPPORT", "Lcom/pnf/LevelType;");

    jobject supportType = env->GetStaticObjectField(levelTypeClass, supportField);

    jobjectArray result = env->NewObjectArray(levels.size(), levelClass, nullptr);
    for (size_t i = 0; i < levels.size(); i++) {
        jobject level = env->NewObject(levelClass, levelConstructor,
                                      supportType, levels[i].price, levels[i].touch_count,
                                      levels[i].first_column, levels[i].last_column);
        env->SetObjectArrayElement(result, i, level);
        env->DeleteLocalRef(level);
    }

    env->DeleteLocalRef(supportType);
    return result;
}

JNIEXPORT jobjectArray JNICALL
Java_com_pnf_Indicators_nativeGetResistanceLevels(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::vector<SupportResistanceLevel> levels = indicators->support_resistance()->resistance_levels();

    jclass levelClass = env->FindClass("com/pnf/SupportResistanceLevel");
    jclass levelTypeClass = env->FindClass("com/pnf/LevelType");
    jmethodID levelConstructor = env->GetMethodID(levelClass, "<init>", "(Lcom/pnf/LevelType;DIII)V");
    jfieldID resistanceField = env->GetStaticFieldID(levelTypeClass, "RESISTANCE", "Lcom/pnf/LevelType;");

    jobject resistanceType = env->GetStaticObjectField(levelTypeClass, resistanceField);

    jobjectArray result = env->NewObjectArray(levels.size(), levelClass, nullptr);
    for (size_t i = 0; i < levels.size(); i++) {
        jobject level = env->NewObject(levelClass, levelConstructor,
                                      resistanceType, levels[i].price, levels[i].touch_count,
                                      levels[i].first_column, levels[i].last_column);
        env->SetObjectArrayElement(result, i, level);
        env->DeleteLocalRef(level);
    }

    env->DeleteLocalRef(resistanceType);
    return result;
}

JNIEXPORT jobjectArray JNICALL
Java_com_pnf_Indicators_nativeGetSignificantLevels(JNIEnv* env, jclass clazz, jlong ptr, jint minTouches) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::vector<SupportResistanceLevel> levels = indicators->support_resistance()->significant_levels(minTouches);

    jclass levelClass = env->FindClass("com/pnf/SupportResistanceLevel");
    jclass levelTypeClass = env->FindClass("com/pnf/LevelType");
    jmethodID levelConstructor = env->GetMethodID(levelClass, "<init>", "(Lcom/pnf/LevelType;DIII)V");
    jfieldID supportField = env->GetStaticFieldID(levelTypeClass, "SUPPORT", "Lcom/pnf/LevelType;");
    jfieldID resistanceField = env->GetStaticFieldID(levelTypeClass, "RESISTANCE", "Lcom/pnf/LevelType;");

    jobject supportType = env->GetStaticObjectField(levelTypeClass, supportField);
    jobject resistanceType = env->GetStaticObjectField(levelTypeClass, resistanceField);

    jobjectArray result = env->NewObjectArray(levels.size(), levelClass, nullptr);
    for (size_t i = 0; i < levels.size(); i++) {
        jobject levelType = levels[i].is_support ? supportType : resistanceType;
        jobject level = env->NewObject(levelClass, levelConstructor,
                                      levelType, levels[i].price, levels[i].touch_count,
                                      levels[i].first_column, levels[i].last_column);
        env->SetObjectArrayElement(result, i, level);
        env->DeleteLocalRef(level);
    }

    env->DeleteLocalRef(supportType);
    env->DeleteLocalRef(resistanceType);
    return result;
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Indicators_nativeIsNearSupport(JNIEnv* env, jclass clazz, jlong ptr, jdouble price, jdouble tolerance) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->support_resistance()->is_near_support(price, tolerance);
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Indicators_nativeIsNearResistance(JNIEnv* env, jclass clazz, jlong ptr, jdouble price, jdouble tolerance) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->support_resistance()->is_near_resistance(price, tolerance);
}

JNIEXPORT jdoubleArray JNICALL
Java_com_pnf_Indicators_nativeGetSupportPrices(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::vector<double> prices = indicators->support_resistance()->support_prices();

    jdoubleArray result = env->NewDoubleArray(prices.size());
    if (result) {
        env->SetDoubleArrayRegion(result, 0, prices.size(), prices.data());
    }
    return result;
}

JNIEXPORT jdoubleArray JNICALL
Java_com_pnf_Indicators_nativeGetResistancePrices(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::vector<double> prices = indicators->support_resistance()->resistance_prices();

    jdoubleArray result = env->NewDoubleArray(prices.size());
    if (result) {
        env->SetDoubleArrayRegion(result, 0, prices.size(), prices.data());
    }
    return result;
}

// Price Objectives
JNIEXPORT jobjectArray JNICALL
Java_com_pnf_Indicators_nativeGetPriceObjectives(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    const std::vector<PriceObjective>& objectives = indicators->objectives()->objectives();

    jclass objectiveClass = env->FindClass("com/pnf/PriceObjective");
    jmethodID objectiveConstructor = env->GetMethodID(objectiveClass, "<init>", "(DIZ)V");

    jobjectArray result = env->NewObjectArray(objectives.size(), objectiveClass, nullptr);
    for (size_t i = 0; i < objectives.size(); i++) {
        jobject objective = env->NewObject(objectiveClass, objectiveConstructor,
                                          objectives[i].target_price, objectives[i].base_column,
                                          objectives[i].is_bullish);
        env->SetObjectArrayElement(result, i, objective);
        env->DeleteLocalRef(objective);
    }

    return result;
}

JNIEXPORT jobjectArray JNICALL
Java_com_pnf_Indicators_nativeGetBullishObjectives(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::vector<PriceObjective> objectives = indicators->objectives()->bullish_objectives();

    jclass objectiveClass = env->FindClass("com/pnf/PriceObjective");
    jmethodID objectiveConstructor = env->GetMethodID(objectiveClass, "<init>", "(DIZ)V");

    jobjectArray result = env->NewObjectArray(objectives.size(), objectiveClass, nullptr);
    for (size_t i = 0; i < objectives.size(); i++) {
        jobject objective = env->NewObject(objectiveClass, objectiveConstructor,
                                          objectives[i].target_price, objectives[i].base_column, JNI_TRUE);
        env->SetObjectArrayElement(result, i, objective);
        env->DeleteLocalRef(objective);
    }

    return result;
}

JNIEXPORT jobjectArray JNICALL
Java_com_pnf_Indicators_nativeGetBearishObjectives(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::vector<PriceObjective> objectives = indicators->objectives()->bearish_objectives();

    jclass objectiveClass = env->FindClass("com/pnf/PriceObjective");
    jmethodID objectiveConstructor = env->GetMethodID(objectiveClass, "<init>", "(DIZ)V");

    jobjectArray result = env->NewObjectArray(objectives.size(), objectiveClass, nullptr);
    for (size_t i = 0; i < objectives.size(); i++) {
        jobject objective = env->NewObject(objectiveClass, objectiveConstructor,
                                          objectives[i].target_price, objectives[i].base_column, JNI_FALSE);
        env->SetObjectArrayElement(result, i, objective);
        env->DeleteLocalRef(objective);
    }

    return result;
}

JNIEXPORT jdoubleArray JNICALL
Java_com_pnf_Indicators_nativeGetBullishTargets(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::vector<double> targets = indicators->objectives()->bullish_targets();

    jdoubleArray result = env->NewDoubleArray(targets.size());
    if (result) {
        env->SetDoubleArrayRegion(result, 0, targets.size(), targets.data());
    }
    return result;
}

JNIEXPORT jdoubleArray JNICALL
Java_com_pnf_Indicators_nativeGetBearishTargets(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::vector<double> targets = indicators->objectives()->bearish_targets();

    jdoubleArray result = env->NewDoubleArray(targets.size());
    if (result) {
        env->SetDoubleArrayRegion(result, 0, targets.size(), targets.data());
    }
    return result;
}

// Congestion Zones
JNIEXPORT jobjectArray JNICALL
Java_com_pnf_Indicators_nativeGetCongestionZones(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    const std::vector<CongestionDetector::CongestionZone>& zones = indicators->congestion()->zones();

    jclass zoneClass = env->FindClass("com/pnf/CongestionZone");
    jmethodID zoneConstructor = env->GetMethodID(zoneClass, "<init>", "(IIDDI)V");

    jobjectArray result = env->NewObjectArray(zones.size(), zoneClass, nullptr);
    for (size_t i = 0; i < zones.size(); i++) {
        jobject zone = env->NewObject(zoneClass, zoneConstructor,
                                      zones[i].start_column, zones[i].end_column,
                                      zones[i].high_price, zones[i].low_price,
                                      zones[i].column_count);
        env->SetObjectArrayElement(result, i, zone);
        env->DeleteLocalRef(zone);
    }

    return result;
}

JNIEXPORT jboolean JNICALL
Java_com_pnf_Indicators_nativeIsInCongestion(JNIEnv* env, jclass clazz, jlong ptr, jint column) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    return indicators->congestion()->is_in_congestion(column);
}

JNIEXPORT jobject JNICALL
Java_com_pnf_Indicators_nativeGetLargestCongestionZone(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    CongestionDetector::CongestionZone zone = indicators->congestion()->largest_zone();

    jclass zoneClass = env->FindClass("com/pnf/CongestionZone");
    jmethodID zoneConstructor = env->GetMethodID(zoneClass, "<init>", "(IIDDI)V");

    return env->NewObject(zoneClass, zoneConstructor,
                         zone.start_column, zone.end_column,
                         zone.high_price, zone.low_price,
                         zone.column_count);
}

// Summary
JNIEXPORT jstring JNICALL
Java_com_pnf_Indicators_nativeSummary(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::string summary = indicators->summary();
    return env->NewStringUTF(summary.c_str());
}

JNIEXPORT jstring JNICALL
Java_com_pnf_Indicators_nativeToString(JNIEnv* env, jclass clazz, jlong ptr) {
    Indicators* indicators = reinterpret_cast<Indicators*>(ptr);
    std::string str = indicators->to_string();
    return env->NewStringUTF(str.c_str());
}

} // extern "C"
