project(openlrr-injector)

add_executable(openlrr-injector
        openlrr-injector.cpp

        ../../resources/OpenLRR-Injector.rc
        ../../resources/resource-injector.h
)

target_compile_definitions(openlrr-injector PUBLIC
        "$<$<CONFIG:Debug>:_DEBUG>"
        "$<$<NOT:$<CONFIG:Debug>>:NDEBUG>"
)

set_target_properties(openlrr-injector PROPERTIES
        DEBUG_POSTFIX "-d"
        OUTPUT_NAME "OpenLRR"
)
