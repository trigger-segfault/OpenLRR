project(openlrr-makeexe)

add_executable(openlrr-makeexe
        common.h

        openlrr-makeexe.cpp

        pe/PECommon.h
        pe/PEFile.cpp
        pe/PEFile.h
        pe/PEResources.h
        pe/PESectionStream.cpp
        pe/PESectionStream.h
)

target_compile_definitions(openlrr-makeexe PUBLIC
        "$<$<CONFIG:Debug>:_DEBUG>"
        "$<$<NOT:$<CONFIG:Debug>>:NDEBUG>"
)

set_target_properties(openlrr-makeexe PROPERTIES
        DEBUG_POSTFIX "-d"
        OUTPUT_NAME "OpenLRR-MakeExe"
)
