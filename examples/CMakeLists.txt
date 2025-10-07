add_library(ChartSystemExamples STATIC
        example_usage.cpp
)

target_link_libraries(ChartSystemExamples PUBLIC
        ChartSystemCore
)